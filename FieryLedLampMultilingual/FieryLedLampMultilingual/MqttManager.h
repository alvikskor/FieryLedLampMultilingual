#ifdef USE_MQTT
/*
 * Библиотека асинхронных MQTT запросов https://github.com/marvinroger/async-mqtt-client
 * Не умеет автоматически восстанавливать разорванное соединение с MQTT брокером, поэтому требует периодической проверки подключения
 * Зависит от библиотек:
 *   ESPAsyncTCP https://github.com/me-no-dev/ESPAsyncTCP
 *   AsyncTCP https://github.com/me-no-dev/AsyncTCP
 *
 * Лампа підписана на топік: <TopicBase>/LedLamp_<ChipId>/cmnd, де <TopicBase> - базова (корнева) частина топіков. Наприклад "Test/LedLamp" (змінюєтся у веб интерфейсі);
 *   <ChipId> - getChipID(); payload - рядок, що містить ті ж команди, що відправляються додатком (реєстр важливий):
 *   P_ON / P_OFF - увімкнути / вимкнути матрицю
 *   FAV_ON / FAV_OFF - увімкнути / вимкнути режим "цикл" ("ізбранне")
 *   SO_ON / SO_OFF - увімкнути / вимкнути озвучування ефектів
 *   VOL15 - встановити гучність озвучування ефектів 15 [1...30]
 *   EFF10 - сделать активным эффект №10 (нумерация с нуля)
 *   BRI44 - встановити яскравість 44; діапазон [1..255]
 *   SPD33 - встановити швидкість 33; діапазон [1..255]
 *   SCA1 - встановити масштаб 1; діапазон [1..100]
 *   ALM_SET1 ON - завести будильник 1 (понедельник); ON - вкл, OFF - выкл.
 *   ALM_SET1 390 - установить время будильника 1 (понедельник) на 06:30 (количество минут от начала суток).
 *   DAWN1 - установить "рассвет" за 5 минут до будильника (1 = 5 минут - номер опции в выпадающем списке в приложении, нумерация с единицы).
 *   TMR_SET 1 3 300 - установить таймер; описание параметров - см. команду TMR ниже.
 *   FAV_SET 1 60 120 0 0 1 0 0 0 0 0 1 1 0 0 1 0 0 0 0 0 0 1 0 0 0 1 0 0 0 - установить режим "избранное", параметры - см. команду FAV ниже.
 *   BTN_ON / BTN_OFF - розблокувати / заблокувати  кнопку на лампі.
 *   STATE - отримати поточний стан лампи.
 *   RND_0 / RND_1 - вибрати випадкові налаштування поточного ефекту / повернути параметри за промовчанням поточного ефекту.
 *   RND_Z - повернути параметри за промовчанням всім ефектам.
 *   RND_C1 / RND_C0 - Включаємо / Виключаємо випадковий вибір ефектів у режимі Цикл
 *   RND_ON / RND_OFF - Увімкнути / Вимкнути вибір випадкових налаштувань у режимі Цикл
 *   TXT_ дали ваш текст для виводу рядком,що біжить.
 *   RUN_T15 - Періодичність виводу (хвилини) рядку,що біжить. [0...99] 0-не виводити;більше 60-виводити постійно.
 *   RUN_C80 - Колир рядку, що біжить. [0...255]
 *   RUN_F1 -  Фон рядку, що біжить. 0-чорний фон; 1-кольоровий фон.
 *   RUN_S240- Швидкисть рядку, що біжить. [0...255]
 *   SLEEP2 - Tаймер сну. SLEEP2 - 10 хвилин. SLEEP1 - 5 хвилин
 *   RESET_ALL - Скидання всіх налаштувань у початкові (за замовчуванням)
 *
 * Лампа відправляє свій стан відразу після включення і після кожної зміни до топіка <TopicBase>/LedLamp_<ChipId>/state; payload: 
 *  "CURR 7 14 4 50 1 1 1 0 1 21:25:50", где:
 *     CURR - идентификатор команды, CURR - текущее состояние лампы
 *     7 - номер текущего эффекта
 *     14 - яркость
 *     4 - скорость
 *     50 - масштаб
 *     1 - признак "матрица включена"
 *     1 - режим ESP_MODE
 *     1 - признак USE_NTP (пытаться синхронизировать время по серверам времени в интернете)
 *     0 - признак "работает таймер"
 *     1 - признак "кнопка разблокирована"
 *     21:25:50 - текущее время (если не синхронизировано, показывает время от старта модуля)
 *  "ALMS 1 0 0 0 0 0 0 0 390 0 0 0 0 0 0 1"
 *     ALMS - идентификатор команды, ALMS - настройки будильников
 *     первые 7 цифр - признак "будильник заведён" по дням недели, начиная с понедельника
 *     последующие 7 цифр - время в минутах от начала суток, на которое заведён будильник (по дням недели); 390 = 06:30
 *     последняя цифра - опция "рассвет за ... минут", цифра указывает на номер значения в выпадающем списке: 1 - 5 минут, 2 - 10 минут... (см. в приложении)
 *  "TMR 1 3 300"
 *     TMR - идентификатор команды, TMR - таймер
 *     1 - признак "таймер взведён"
 *     3 - опция "выключить лампу через ...", цифра указывает на номер значения в выпадающем списке: 1 - не выключать, 2 - 1 минута... (см. в приложении)
 *     300 - количество секунд, через которое выключится лампа (0 - не выключать)
 *  "FAV 1 60 120 0 0 1 0 0 0 0 0 1 1 0 0 1 0 0 0 0 0 0 1 0 0 0 1 0 0 0............МНОГО НОЛИКОВ И ЕДИНИЧЕК... 1 0"
 *     FAV - идентификатор команды, FAV - избранное
 *     1 - режим "избранное" включен
 *     60 - интервал смены эффектов в секундах
 *     120 - случайный разброс смены эффектов (применяется дополнительно к интервалу) в секундах
 *     0 - признак "запомнить состояние" вкл/выкл режима "избранное" в энергонезависимую память
 *     оставшиеся цифры - признак (0/1) "эффект №... добавлен в избранные", где номер цифры соотвтетсвует номеру эффекта в списке (см. приложение)
 *     КОЛИЧЕСТВО ЦИФЕРОК ДОЛЖНО СООТВЕТСТВОВАТЬ КОЛИЧЕСТВУ ЭФФЕКТОВ!!!!!!
 * Не зрозуміло навіщо це і як це парсити в НА (но може куму треба) , тому зробив наступне ...
 *
 * Також лампа видправляє свій стан у топік <TopicBase>/LedLamp_<ChipId>/snd у вигляді JSON;
 *    {"power":"P_ON","cycle":"FAV_OFF","effect":"111","bri":"15","spd":"33","sca":"58","runt":"10","runc":"123","runf":"1","runc":"220","rnde":"0","rndc":"1","rndf":"0","volume":"10","sound":"SO_ON"}
 *    Можливо додати (або замінити) ще якісь змінні (данні) для виводу у відповіді, але довжина радку відповіді повина бути меньша ниж 255 байт. Формування відповіді-дивись файл FieryLedLampMultilingual.ino
*/

#include <AsyncMqttClient.h>
#include "pgmspace.h"
#include "Constants.h"
#include "Types.h"

// #define PUBLISH_STATE_IN_OLD_FORMAT                              // Чи потрібно публіковати стан лампі у старому форматі

static const char TopicCmnd[]          PROGMEM = "cmnd";             // часть командных топиков (входящие команды лампе)
static const char TopicState[]         PROGMEM = "state";            // часть топиков состояния (ответ от лампы)
static const char TopicSnd[]           PROGMEM = "snd";              // частина топіков стану у JSON форматі (відповідь від лампи)

static bool mqttIPaddr              = true;                          // true - IP адреса, false - URL адреса
static IPAddress MqttServer         ;                                // Рядок з IP адресою MQTT брокера
static char MqttHost[33]            = "";                            // Рядок з URL адресою MQTT брокера
static uint16_t MqttPort            = 1883U;                         // порт MQTT брокера
static char TopicBase[33]           = "";                            // базова (корнева) частина топіков. Наприклад "LedLamp"
static char MqttUser[33]            = "";                            // пользователь MQTT брокера
static char MqttPassword[33]        = "";                            // пароль пользователя MQTT брокера
static uint8_t MqttOn               = 1U;                            // MQTT on / off
static uint8_t MqttPeriod           = 0U;                            // Період публікації відповіді лампи (0 - 60 секунд). Якщо 0-публіковати відповідь лише при наступу події (зміна параметрів).
static uint32_t mqtt_timer          = 0UL;                           // використовуєтся при периодичної публікації раз на 1 секунду коли запущений таймер
static const char MqttClientIdPrefix[] PROGMEM = "LedLamp_";         // id клиента MQTT брокера (к нему будет добавлен ESP.getChipId)


class MqttManager
{
  public:
    static uint32_t mqttLastConnectingAttempt;
    static void setupMqtt(AsyncMqttClient* mqttClient, char* lampInputBuffer, SendCurrentDelegate sendCurrentDelegate);
    static void mqttConnect();
    static void onMqttConnect(bool sessionPresent);
    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    static void onMqttMessage(char *topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    static bool publish(const char *topic, const char *value);
    static void publishState(uint8_t flag = 0);
    static bool needToPublish;
    static char mqttBuffer[255U];
    static char* clientId;

  private:
    static char* topicInput;                                                    // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicCmnd
    #ifdef PUBLISH_STATE_IN_OLD_FORMAT
    static char* topicOutput;                                                   // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicState
    #endif
    static char* topicOutputJSON;                                               // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicSnd
    static char* lampInputBuffer;                                               // ссылка на inputBuffer для записи в него пришедшей MQTT команды и последующей её обработки лампой
    static AsyncMqttClient* mqttClient;
    static const uint8_t qos = 0U;                                              // MQTT quality of service
    static const uint32_t connectionTimeout = MQTT_RECONNECT_TIME * 1000U;      // период времени для проверки (пере)подключения к MQTT брокеру, 10 секунд
    static char* byteToHex(char *out, uint8_t value);
    static bool allocStr(char **str, const char *src);
    //static bool allocStr_P(char **str, PGM_P src);
    static SendCurrentDelegate sendCurrentDelegate;
};


void MqttManager::setupMqtt(AsyncMqttClient* mqttClient, char* lampInputBuffer, SendCurrentDelegate sendCurrentDelegate)
{
  MqttManager::mqttClient = mqttClient;
  MqttManager::lampInputBuffer = lampInputBuffer;
  MqttManager::sendCurrentDelegate = sendCurrentDelegate;
  if(mqttIPaddr)
      MqttManager::mqttClient->setServer(MqttServer, MqttPort); // Якщо введено IP адресу
  else
      MqttManager::mqttClient->setServer(MqttHost, MqttPort);  // Ящо введено URL

  char clientIdBuf[sizeof(MqttClientIdPrefix) + 8];
  strcpy_P(clientIdBuf, MqttClientIdPrefix);
  #ifdef ESP32_USED
  uint32_t chipId = get_Chip_ID();
  #else
  uint32_t chipId = ESP.getChipId();
  #endif
  for (uint8_t i = 0; i < 4; ++i)
  {
    byteToHex(&clientIdBuf[i * 2 + sizeof(MqttClientIdPrefix) - 1], chipId >> ((3 - i) * 8));
  }
  allocStr(&clientId, clientIdBuf);
  MqttManager::mqttClient->setClientId(clientId);

  if (MqttUser != NULL) // if (MqttManager::mqttUser != NULL)
  {
    MqttManager::mqttClient->setCredentials(MqttUser, MqttPassword);
  }

  uint8_t topicLength = strlen(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicCmnd) + 1;
  topicInput = (char*)malloc(topicLength);
  sprintf_P(topicInput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicCmnd);      // topicInput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicCmnd

  #ifdef PUBLISH_STATE_IN_OLD_FORMAT
  topicLength = strlen(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicState) + 1;
  topicOutput = (char*)malloc(topicLength);
  sprintf_P(topicOutput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicState);    // topicOutput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicState
  #endif

  topicLength = strlen(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicSnd) + 1;
  topicOutputJSON = (char*)malloc(topicLength);
  sprintf_P(topicOutputJSON, PSTR("%s/%s/%s"), TopicBase, clientId, TopicSnd);    // topicOutputJSON = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicSnd

  #ifdef GENERAL_DEBUG
  LOG.printf_P(PSTR("MQTT топік для вхідних команд: %s\n"), topicInput);
  #ifdef PUBLISH_STATE_IN_OLD_FORMAT
  LOG.printf_P(PSTR("MQTT топік для вихідних відповідей лампи: %s\n"), topicOutput);
  #endif
  LOG.printf_P(PSTR("MQTT топік для вихідних відповідей лампи у форматі JSON: %s\n"), topicOutputJSON);
  #endif

  mqttClient->onConnect(onMqttConnect);
  mqttClient->onDisconnect(onMqttDisconnect);
  mqttClient->onMessage(onMqttMessage);
}

void MqttManager::mqttConnect()
{
  if ((!mqttLastConnectingAttempt) || (millis() - mqttLastConnectingAttempt >= connectionTimeout))
  {
    #ifdef GENERAL_DEBUG
    LOG.print(F("Підключення до MQTT брокеру \""));
    if(mqttIPaddr)
        LOG.print(MqttServer);
    else
        LOG.print(MqttHost);
    LOG.print(':');
    LOG.print(MqttPort);
    LOG.println(F("\"..."));
    #endif
    mqttClient->disconnect();
    mqttClient->connect();
    mqttLastConnectingAttempt = millis();
  }
}

bool MqttManager::publish(const char *topic, const char *value)
{
  if (mqttClient->connected())
  {
    #ifdef GENERAL_DEBUG
    LOG.print(F("Відправлено MQTT: топік \""));
    LOG.print(topic);
    LOG.print(F("\", значення \""));
    LOG.print(value);
    LOG.println('"');
    LOG.println();
    #endif

    return mqttClient->publish(topic, qos, true, value, 0) != 0;
  }

  return false;
}

void MqttManager::onMqttConnect(bool sessionPresent)
{
  #ifdef GENERAL_DEBUG
  LOG.println(F("Підключено до MQTT брокеру"));
  #endif
  mqttLastConnectingAttempt = 0;

  mqttClient->subscribe(topicInput, 1);
  //publishState(0);
  needToPublish = true;
}

void MqttManager::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  #ifdef GENERAL_DEBUG
  LOG.println(F("Відключено від MQTT брокеру"));
  #endif
}

void MqttManager::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  if (payload != NULL)                                      // сохраняем пришедшее MQTT сообщение для дальнейшей обработки
  {
    strncpy(lampInputBuffer, payload, len);
    lampInputBuffer[len] = '\0';
    needToPublish = true;
  }

  #ifdef GENERAL_DEBUG
  LOG.print(F("Отримано MQTT:"));
  LOG.print(F(" топік \""));
  LOG.print(topic);
  LOG.print("\"");
  /*
  LOG.print(F(" qos: "));
  LOG.println(properties.qos);
  LOG.print(F(" dup: "));
  LOG.println(properties.dup);
  LOG.print(F(" retain: "));
  LOG.println(properties.retain);
  LOG.print(F(" len: "));
  LOG.println(len);
  LOG.print(F(" index: "));
  LOG.println(index);
  LOG.print(F(" total: "));
  LOG.println(total);
  */
  LOG.print(F(", значення \""));
  LOG.print(lampInputBuffer);
  LOG.println("\"");
  LOG.println();
  #endif
}

void MqttManager::publishState(uint8_t flag)
{
  if(!flag){                                                // якщо flag == 0 публікуємо буфер у топік <TopicBase>/LedLamp_<ChipId>/state
  #ifdef PUBLISH_STATE_IN_OLD_FORMAT
  if (mqttBuffer == NULL || strlen(mqttBuffer) <= 0)
  {
    sendCurrentDelegate(mqttBuffer);                        // если буфер MQTT ответа не задан, но метод MQTT публикации вызван, закполняем его текущим состоянием лампы
  }

  if (mqttBuffer != NULL && strlen(mqttBuffer) > 0)
  {
    publish(topicOutput, mqttBuffer);                       // публикация буфера MQTT ответа
    mqttBuffer[0] = '\0';                                   // очистка буфера
    needToPublish = false;                                  // сброс флага для предотвращения повторной публикации
  }
  #endif // PUBLISH_STATE_IN_OLD_FORMAT
  ;
  }
  else{                                                     // инакше публікуємо буфер у топік <TopicBase>/LedLamp_<ChipId>/snd
    publish(topicOutputJSON, mqttBuffer);                   // публікація буфера MQTT відповіді (JSON)
    //Serial.print("topicOutputJSON = ");
    //Serial.println(topicOutputJSON);
    mqttBuffer[0] = '\0';                                   // очистка буфера
    needToPublish = false;                                  // сброс флага для предотвращения повторной публикации
  } 
}

char* MqttManager::byteToHex(char *out, uint8_t value)
{
  uint8_t b;

  b = value >> 4;
  if (b < 10)
  {
    out[0] = '0' + b;
  }
  else
  {
    out[0] = 'A' + (b - 10);
  }
  b = value & 0x0F;
  if (b < 10)
  {
    out[1] = '0' + b;
  }
  else
  {
    out[1] = 'A' + (b - 10);
  }
  out[2] = '\0';

  return out;
}

bool MqttManager::allocStr(char **str, const char *src)
{
  if (src && *src)
  {
    if (*str)
    {
      void *ptr = realloc(*str, strlen(src) + 1);

      if (!ptr)
      {
        return false;
      }
      *str = (char*)ptr;
    }
    else
    {
      *str = (char*)malloc(strlen(src) + 1);
      if (!*str)
      {
        return false;
      }
    }
    strcpy(*str, src);
  }
  else
  {
    if (*str)
    {
      free(*str);
      *str = NULL;
    }
  }

  return true;
}
/*
bool MqttManager::allocStr_P(char **str, PGM_P src)
{
  if (src && pgm_read_byte(src))
  {
    if (*str)
    {
      void *ptr = realloc(*str, strlen_P(src) + 1);

      if (!ptr)
      {
        return false;
      }
      *str = (char*)ptr;
    }
    else
    {
      *str = (char*)malloc(strlen_P(src) + 1);
      if (!*str)
      {
        return false;
      }
    }
    strcpy_P(*str, src);
  }
  else
  {
    if (*str)
    {
      free(*str);
      *str = NULL;
    }
  }

  return true;
}
*/
#endif
