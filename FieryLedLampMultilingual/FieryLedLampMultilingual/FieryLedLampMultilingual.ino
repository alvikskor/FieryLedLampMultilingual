// 
// Дякую @Stepko за допомогу у перекладі текстових файлів українською мовою.
//
// ======================= УВАГА !!! =============================
// Усі налаштування робляться на вкладці Constants.h
// Почитайте там те, що російською мовою написано.
// Або нічого не чіпайте, якщо збирали, за схемами з цього архіву.
// У будь-якому випадку УВАЖНО прочитайте файл ПРОЧИТАЙ МЕНЕ!!!.txt з цього архіву.
//
// ================================================ ==================
//
// Посилання для менеджера плат:
// https://arduino.esp8266.com/stable/package_esp8266com_index.json
// При установці вибираємо версію 2.7.4
//
// ==================================================================
//
// Далі йде код проекту. Не змінюйте тут нічого, якщо Ви не розумієте, до чого це призведе!
//
//+++++++++++++++++==========================================+++++++++++++++++++++++++++++++
//
#define FASTLED_USE_PROGMEM 1 // просим библиотеку FASTLED экономить память контроллера на свои палитры
#include "pgmspace.h"
#include "Constants.h"
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "Types.h"
#include "timerMinim.h"
#ifdef ESP_USE_BUTTON
#include <GyverButton.h>
#endif
#include "fonts.h"
#ifdef USE_NTP
#include <NTPClient.h>
#include <Timezone.h>
#endif
#include <TimeLib.h>
#ifdef OTA
#include "OtaManager.h"
#endif
#if USE_MQTT
#include "MqttManager.h"
#endif
#include "TimerManager.h"
#include "EepromManager.h"
#include "FavoritesManager.h"
#ifdef USE_BLYNK
#include <BlynkSimpleEsp8266.h>
#endif
#include <ESP8266SSDP.h>        
#include <ArduinoJson.h>        //Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/
#include <ESP8266HTTPUpdateServer.h>    // Обновление с web страницы
#ifdef USE_LittleFS
#include <LittleFS.h>    
#define SPIFFS LittleFS  
#endif
#ifdef TM1637_USE
#include "TM1637Display.h"
#endif
#ifdef MP3_TX_PIN
#include <SoftwareSerial.h>                  // Подключаем библиотеку для работы с последовательным интерфейсом
//#include <DFRobotDFPlayerMini.h>             // Подключаем библиотеку для работы с плеером
 #ifdef MP3_DEBUG
  #define FEEDBACK  1
 #else
  #define FEEDBACK  0
 #endif  //MP3_DEBUG
#endif  //MP3_TX_PIN

// --- ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ ----------
CRGB leds[NUM_LEDS];
//WiFiManager wifiManager;
//WiFiServer wifiServer(ESP_HTTP_PORT);
WiFiUDP Udp;

#ifdef USE_NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, 0, NTP_INTERVAL); // объект, запрашивающий время с ntp сервера; в нём смещение часового пояса не используется (перенесено в объект localTimeZone); здесь всегда должно быть время UTC
   TimeChangeRule summerTime  = { SUMMER_TIMEZONE_NAME, SUMMER_WEEK_NUM, SUMMER_WEEKDAY, SUMMER_MONTH, SUMMER_HOUR, 0 };
   TimeChangeRule winterTime  = { WINTER_TIMEZONE_NAME, WINTER_WEEK_NUM, WINTER_WEEKDAY, WINTER_MONTH, WINTER_HOUR, 0 };
   Timezone localTimeZone(summerTime, winterTime);
  #ifdef PHONE_N_MANUAL_TIME_PRIORITY
    bool stillUseNTP = true;
  #endif    
#endif

timerMinim timeTimer(3000);
bool ntpServerAddressResolved = false;
bool timeSynched = false;
uint32_t lastTimePrinted = 0U;

#if defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
time_t manualTimeShift;
#endif

#ifdef GET_TIME_FROM_PHONE
time_t phoneTimeLastSync;
#endif

uint8_t selectedSettings = 0U;

#ifdef ESP_USE_BUTTON
#if (BUTTON_IS_SENSORY == 1)
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);  // для сенсорной кнопки LOW_PULL
#else
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN); // для физической (не сенсорной) кнопки HIGH_PULL. ну и кнопку нужно ставить без резистора в разрыв между пинами D2 и GND
#endif
#endif

#ifdef OTA
OtaManager otaManager(&showWarning);
OtaPhase OtaManager::OtaFlag = OtaPhase::None;
#endif

#if USE_MQTT
AsyncMqttClient* mqttClient = NULL;
AsyncMqttClient* MqttManager::mqttClient = NULL;
char* MqttManager::mqttServer = NULL;
char* MqttManager::mqttUser = NULL;
char* MqttManager::mqttPassword = NULL;
char* MqttManager::clientId = NULL;
char* MqttManager::lampInputBuffer = NULL;
char* MqttManager::topicInput = NULL;
char* MqttManager::topicOutput = NULL;
bool MqttManager::needToPublish = false;
char MqttManager::mqttBuffer[] = {};
uint32_t MqttManager::mqttLastConnectingAttempt = 0;
SendCurrentDelegate MqttManager::sendCurrentDelegate = NULL;
#endif

ESP8266HTTPUpdateServer httpUpdater;  // Объект для обнавления с web страницы
ESP8266WebServer HTTP (ESP_HTTP_PORT);//ESP8266WebServer HTTP;  // Web интерфейс для устройства
File fsUploadFile;  // Для файловой системы



// --- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ -------
uint16_t localPort = ESP_UDP_PORT;
char packetBuffer[MAX_UDP_BUFFER_SIZE];                     // buffer to hold incoming packet
char inputBuffer[MAX_UDP_BUFFER_SIZE];
static const uint8_t maxDim = max(WIDTH, HEIGHT);


AlarmType alarms[7];

static const uint8_t dawnOffsets[] PROGMEM = {5, 10, 15, 20, 25, 30, 40, 50, 60};   // опции для выпадающего списка параметра "время перед 'рассветом'" (будильник); синхронизировано с android приложением
uint8_t dawnMode;
bool dawnFlag = false;
uint32_t thisTime;
bool manualOff = false;

uint8_t currentMode = 3;
bool loadingFlag = true;
bool ONflag = false;
uint32_t eepromTimeout;
bool settChanged = false;
bool buttonEnabled = true; // это важное первоначальное значение. нельзя делать false по умолчанию

unsigned char matrixValue[8][16]; //это массив для эффекта Огонь

bool TimerManager::TimerRunning = false;
bool TimerManager::TimerHasFired = false;
uint8_t TimerManager::TimerOption = 1U;
uint64_t TimerManager::TimeToFire = 0ULL;

uint8_t FavoritesManager::FavoritesRunning = 0;
uint16_t FavoritesManager::Interval = DEFAULT_FAVORITES_INTERVAL;
uint16_t FavoritesManager::Dispersion = DEFAULT_FAVORITES_DISPERSION;
uint8_t FavoritesManager::UseSavedFavoritesRunning = 0;
uint8_t FavoritesManager::FavoriteModes[MODE_AMOUNT] = {0};
uint32_t FavoritesManager::nextModeAt = 0UL;
bool FavoritesManager::rndCycle = false;

char TextTicker [86];
int Painting = 0; CRGB DriwingColor = CRGB(255, 255, 255);

//..................... Переменные, добавленные с внедрением web интерфейса .............................................................................................

uint8_t FPSdelay = DYNAMIC;
uint8_t espMode ;
uint8_t random_on;
uint8_t Favorit_only;
//uint32_t my_time;
uint32_t my_timer;
uint8_t time_always;
bool connect = false;
uint32_t lastResolveTryMoment = 0xFFFFFFFFUL;
uint8_t ESP_CONN_TIMEOUT;
uint8_t PRINT_TIME ;
uint8_t day_night = false;     // если день - true, ночь - false
uint8_t save_file_changes =0;
uint32_t timeout_save_file_changes;
uint8_t first_entry = 0;
uint16_t dawnPosition;
#define SAVE_FILE_DELAY_TIMEOUT  15000UL

#ifdef USE_MULTIPLE_LAMPS_CONTROL
char Host1[16], Host2[16], Host3[16];
uint8_t ml1, ml2, ml3;
//bool repeat_multiple_lamp_control = false;
#endif //USE_MULTIPLE_LAMPS_CONTROL

#ifdef MP3_TX_PIN
uint8_t mp3_folder=1;                // Текущая папка для воспроизведения.
uint8_t alarm_sound_on =false;       // Включить/выключить звук будильника
uint8_t alarm_volume;                // Громкость будильника
uint8_t AlarmFolder;                 // Папка будильника
uint8_t Equalizer;                   // Еквалайзер
bool alarm_sound_flag =false;        // проигрывается ли сейчас будильник
uint8_t dawnflag_sound = false;      // Звук не начал обслуживание рассвета. Если не true - звук обслуживает рассвет
//uint8_t tmp_fold;
bool advert_flag = false;            // Озвучивается время
bool advert_hour;                    // Озвучиваются часы времени
uint8_t day_advert_volume;           // Дневная Громкость озвучивания времени
uint8_t night_advert_volume;         // Ночная Громкость озвучивания времени
bool day_advert_sound_on;            // Вкл.Выкл озвучивания времени днём
bool night_advert_sound_on;          // Вкл.Выкл озвучивания времени ночью
bool alarm_advert_sound_on;          // Вкл.Выкл озвучивания времени будильником
uint8_t mp3_player_connect = 0;      // Плеер не подключен. true - подключен.
uint8_t mp3_folder_last=255;           // Предыдущая папка для воспроизведения
bool set_mp3_play_now=false;         // Указывает, надо ли играть сейчас мелодии
//bool mp3_play_now=false;           // Указывает, играет ли сейчас мелодия
//uint8_t eff_volume_tmp = 0;
//uint8_t day_volum;
//uint8_t night_volum;
uint32_t alarm_timer;                // Периодичность проверки и плавного изменения громкости будильника
uint32_t mp3_timer = 0;
bool mp3_stop = true;                       // Озвучка эффектов остановлена
bool pause_on = true;                        // Озвучка эффектов на паузе. false - не на паузе
uint8_t eff_volume = 9;                      // громкость воспроизведения
uint8_t eff_sound_on = 0;                    // звук включен - !0 (true), выключен - 0
uint8_t CurrentFolder;              // Папка, на которую переключились (будет проигрываться)
SoftwareSerial mp3(MP3_RX_PIN, MP3_TX_PIN);  // создаём объект mySoftwareSerial и указываем выводы, к которым подлючен плеер (RX, TX)
#ifndef TM1637_USE
 uint8_t minute_tmp;
#endif
uint8_t mp3_receive_buf[10];
uint8_t effects_folders[MODE_AMOUNT]; // Номера папок для озвучивания
#endif  //MP3_TX_PIN
uint16_t current_limit;              // настраиваемый  Лимит тока
#ifdef TM1637_USE
uint8_t DispBrightness = 1;          // +++ Яркость дисплея от 0 до 255(5 уровней яркости с шагом 51). 0 - дисплей погашен 
bool dotFlag = false;                // +++ флаг: в часах рисуется двоеточие или нет
uint32_t tmr_clock = 0;              // +++ таймер мигания разделителя часов на дисплее
uint32_t tmr_blink = 0;              // +++ таймер плавного изменения яркости дисплея
//bool blink_clock = false;            // +++ флаг: false-запрещает плавное изменение яркости дисплея, true-разрешает плавное изменение яркости дисплея
TM1637Display display(CLK, DIO);     // +++ подключаем дисплей
bool aDirection = false;             // +++ Направление изменения яркрсти
uint8_t last_minute;
uint32_t DisplayTimer;               // Время отображения номера эффекта
uint8_t LastEffect = 255;            // последний Проигрываемый эффект
uint8_t DisplayFlag=0;               // Флаг, показывающий, что отображается номер эффекта и папки
 #ifdef MP3_TX_PIN
 uint8_t LastCurrentFolder = 255;    // Проигрываемая папка
 #endif  //MP3_TX_PIN
#endif  //TM1637_USE

#ifdef HEAP_SIZE_PRINT
uint32_t mem_timer;
#endif //HEAP_SIZE_PRINT 


void setup()  //==================================================================  void setup()  =========================================================================
{
	
  Serial.begin(115200);
  LOG.println();
  ESP.wdtEnable(WDTO_8S);

  // часы
#ifdef TM1637_USE
  tmr_clock = millis();                                     // +++ устанавливаем начальное значение счетчика
  display.setBrightness(DispBrightness);                    // +++ яркость дисплея максимальная = 255
  display.displayByte(_empty, _empty, _empty, _empty);      // +++ очистка дисплея
  display.displayByte(_dash, _dash, _dash, _dash);          // +++ отображаем прочерки
#endif

  // ПИНЫ
  #ifdef MOSFET_PIN                                         // инициализация пина, управляющего MOSFET транзистором в состояние "выключен"
  pinMode(MOSFET_PIN, OUTPUT);
  #ifdef MOSFET_LEVEL
  digitalWrite(MOSFET_PIN, !MOSFET_LEVEL);
  #endif
  #endif

  #ifdef ALARM_PIN                                          // инициализация пина, управляющего будильником в состояние "выключен"
  pinMode(ALARM_PIN, OUTPUT);
  #ifdef ALARM_LEVEL
  digitalWrite(ALARM_PIN, !ALARM_LEVEL);
  #endif
  #endif
  
   //HTTP
  User_setings (); 
  LOG.print(F("\nСтарт файловой системы\n"));
  FS_init();  //Запускаем файловую систему
  LOG.print(F("Чтение файла конфигурации\n"));
  configSetup = readFile("config.json", 1024);   
  LOG.println(configSetup);
  //Настраиваем и запускаем SSDP интерфейс
  LOG.print(F("Старт SSDP\n"));
  SSDP_init();
  //Настраиваем и запускаем HTTP интерфейс
  LOG.print (F("Старт WebServer\n"));
  HTTP_init();

  
//-----------Инициализируем переменные, хранящиеся в файле config.json--------------
  LAMP_NAME = jsonRead(configSetup, "SSDP");
  AP_NAME = jsonRead(configSetup, "ssidAP");
  AP_PASS = jsonRead(configSetup, "passwordAP");
  Favorit_only = jsonReadtoInt(configSetup, "favorit");
  random_on = jsonReadtoInt(configSetup, "random_on");
  espMode = jsonReadtoInt(configSetup, "ESP_mode");
  PRINT_TIME = jsonReadtoInt(configSetup, "print_time");
  buttonEnabled = jsonReadtoInt(configSetup, "button_on");
  ESP_CONN_TIMEOUT = jsonReadtoInt(configSetup, "TimeOut");
  time_always = jsonReadtoInt(configSetup, "time_always");
  (jsonRead(configSetup, "run_text")).toCharArray (TextTicker, (jsonRead(configSetup, "run_text")).length()+1);
  NIGHT_HOURS_START = 60U * jsonReadtoInt(configSetup, "night_time");
  NIGHT_HOURS_BRIGHTNESS = jsonReadtoInt(configSetup, "night_bright");
  NIGHT_HOURS_STOP = 60U * jsonReadtoInt(configSetup, "day_time");
  DAY_HOURS_BRIGHTNESS = jsonReadtoInt(configSetup, "day_bright");
  DONT_TURN_ON_AFTER_SHUTDOWN = jsonReadtoInt(configSetup, "effect_always"); 
  FavoritesManager::rndCycle = jsonReadtoInt(configSetup, "rnd_cycle");  // Перемешать Цикл
  AUTOMATIC_OFF_TIME = (30UL * 60UL * 1000UL) * ( uint32_t )(jsonReadtoInt(configSetup, "timer5h"));  
  #ifdef USE_NTP
  (jsonRead(configSetup, "ntp")).toCharArray (NTP_ADDRESS, (jsonRead(configSetup, "ntp")).length()+1);
  #endif
  //saveConfig(); 
  //Serial.print ("TextTicker = ");
  //Serial.println (TextTicker);
  #ifdef USE_NTP
  winterTime.offset = jsonReadtoInt(configSetup, "timezone") * 60;
  summerTime.offset = winterTime.offset + jsonReadtoInt(configSetup, "Summer_Time") *60;
  localTimeZone.setRules (summerTime, winterTime);
  #endif
  #ifdef MP3_TX_PIN
  eff_volume = jsonReadtoInt(configSetup, "vol");
  eff_sound_on = (jsonReadtoInt(configSetup, "on_sound")==0)? 0 : eff_volume;
  alarm_volume = jsonReadtoInt(configSetup, "alm_vol");
  AlarmFolder = jsonReadtoInt(configSetup, "alm_fold");
  alarm_sound_on = jsonReadtoInt(configSetup, "on_alm_snd");
  day_advert_sound_on = jsonReadtoInt(configSetup,"on_day_adv");
  night_advert_sound_on = jsonReadtoInt(configSetup,"on_night_adv");
  day_advert_volume = jsonReadtoInt(configSetup,"day_vol");
  alarm_advert_sound_on = jsonReadtoInt(configSetup,"on_alm_adv");
  night_advert_volume = jsonReadtoInt(configSetup,"night_vol");
  Equalizer = jsonReadtoInt(configSetup, "eq");
  #endif //MP3_TX_PIN
  current_limit = jsonReadtoInt(configSetup, "cur_lim");
  MATRIX_TYPE = jsonReadtoInt(configSetup, "m_t");
  ORIENTATION = jsonReadtoInt(configSetup, "m_o");
  {
    String Name = "correct." + jsonRead (configSetup, "lang") + ".json";
    String Correct = readFile(Name, 2048);
    for ( uint8_t n=0; n< MODE_AMOUNT; n++)
    {
        eff_num_correct[n] = jsonReadtoInt (Correct, String(n));
    }
  }


  // TELNET
  #if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  telnetServer.begin();
  for (uint8_t i = 0; i < 100; i++)                         // пауза 10 секунд в отладочном режиме, чтобы успеть подключиться по протоколу telnet до вывода первых сообщений
  {
    handleTelnetClient();
    delay(100);
    ESP.wdtFeed();
  }
  #endif


  // КНОПКА
  #if defined(ESP_USE_BUTTON)
  touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  touch.setDebounce(BUTTON_SET_DEBOUNCE);
   #if (BUTTON_IS_SENSORY == 1)
    #if ESP_RESET_ON_START
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (digitalRead(BTN_PIN))
    {
     // wifiManager.resetSettings();                          
      LOG.println(F("Настройки WiFiManager сброшены"));
      //buttonEnabled = true;                                   // при сбросе параметров WiFi сразу после старта с зажатой кнопкой, также разблокируется кнопка, если была заблокирована раньше
	jsonWrite(configSetup, "ssid", "");                          // сброс сохранённых SSID и пароля при старте с зажатой кнопкой, если разрешено
	jsonWrite(configSetup, "password", "");
	saveConfig();                                       // Функция сохранения данных во Flash
    }
    ESP.wdtFeed();
    #elif defined(BUTTON_LOCK_ON_START)
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (digitalRead(BTN_PIN))
      buttonEnabled = false;
    ESP.wdtFeed();
    #endif
   #endif
   #if (BUTTON_IS_SENSORY == 0)
    #if ESP_RESET_ON_START
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (!(digitalRead(BTN_PIN)))
    {
     // wifiManager.resetSettings();                          
      LOG.println(F("Настройки WiFiManager сброшены"));
      //buttonEnabled = true;                                   // при сбросе параметров WiFi сразу после старта с зажатой кнопкой, также разблокируется кнопка, если была заблокирована раньше
	jsonWrite(configSetup, "ssid", "");                          // сброс сохранённых SSID и пароля при старте с зажатой кнопкой, если разрешено
	jsonWrite(configSetup, "password", "");
	saveConfig();                                       // Функция сохранения данных во Flash
    }
    ESP.wdtFeed();
    #elif defined(BUTTON_LOCK_ON_START)
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (!(digitalRead(BTN_PIN)))
      buttonEnabled = false;
    ESP.wdtFeed();
    #endif
   #endif    
  #endif


  // ЛЕНТА/МАТРИЦА
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection(TypicalLEDStrip)*/;
  //FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xB0FFE0); // Калибровка баланса белого цвета. Последовательность байт RGB (B0-R FF-G E0-B)
  FastLED.setBrightness(BRIGHTNESS);
  if (current_limit > 0)
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(5, current_limit);
  }
  FastLED.clear();
  FastLED.show();

#ifdef USE_SHUFFLE_FAVORITES // первоначальная очередь избранного до перемешивания
    for (uint8_t i = 0; i < MODE_AMOUNT; i++)
      shuffleFavoriteModes[i] = i;
#endif

  // EEPROM
  EepromManager::InitEepromSettings(                        // инициализация EEPROM; запись начального состояния настроек, если их там ещё нет; инициализация настроек лампы значениями из EEPROM
    modes, alarms, &ONflag, &dawnMode, &currentMode,
    &(FavoritesManager::ReadFavoritesFromEeprom),
    &(FavoritesManager::SaveFavoritesToEeprom),
    &(restoreSettings)); // не придумал ничего лучше, чем делать восстановление настроек по умолчанию в обработчике инициализации EepromManager

  jsonWrite(configSetup, "Power", ONflag);  // Чтение состояния лампы вкл/выкл,текущий эффект,яркость,скорость,масштаб
  {
    String Name = "correct." + jsonRead (configSetup, "lang") + ".json";
    String Correct = readFile(Name, 2048);
    for ( uint8_t n=0; n< MODE_AMOUNT; n++) 
    {
        eff_num_correct[n] = jsonReadtoInt (Correct, String(n)); 
        if (eff_num_correct[n] == currentMode) jsonWrite(configSetup, "eff_sel", n);
    }
  }
  {
    File file = SPIFFS.open("/index.json.gz","r");
    File Status = SPIFFS.open("/effect2.ini", "r");
    if (Status && file.size() > 700)
    {
    String Name = Status.readString();
    Name.toCharArray (TextTicker, Name.length()+1);
    SPIFFS.format();
    buttonEnabled = 0;
    currentMode = EFF_TEXT;
    ONflag = 1;
    changePower();
    }
    file.close();
    Status.close();
  }
  //jsonWrite(configSetup, "eff_sel", currentMode);
  jsonWrite(configSetup, "br", modes[currentMode].Brightness);
  jsonWrite(configSetup, "sp", modes[currentMode].Speed);
  jsonWrite(configSetup, "sc", modes[currentMode].Scale); 
  //sendAlarms(inputBuffer);                                                 // Чтение настроек будильника при старте лампы
  first_entry = 1;
  handle_alarm ();
  first_entry = 0;
  jsonWrite(configSetup, "cycle_on", FavoritesManager::FavoritesRunning);  // чтение состояния настроек режима Цикл 
  jsonWrite(configSetup, "time_eff", FavoritesManager::Interval);          // вкл/выкл,время переключения,дисперсия,вкл цикла после перезагрузки
  jsonWrite(configSetup, "disp", FavoritesManager::Dispersion);
  jsonWrite(configSetup, "cycle_allwase", FavoritesManager::UseSavedFavoritesRunning);
  jsonWrite(configSetup, "tmr", 0);
  jsonWrite(configSetup, "button_on", buttonEnabled);
  //cycle_get ();
  first_entry = 1;
  handle_cycle_set();  // чтение выбранных эффектов
  first_entry = 0;
#ifdef MP3_TX_PIN
  first_entry = 1;
  handle_sound_set();
  first_entry = 0;
#endif  //MP3_TX_PIN
#ifdef USE_MULTIPLE_LAMPS_CONTROL  
  multilamp_get ();   // Чтение из файла адресов синхронно управляемых ламп 
#endif //USE_MULTIPLE_LAMPS_CONTROL
//#ifdef MP3_TX_PIN
//jsonWrite(configSetup, "vol", eff_volume);
//jsonWrite(configSetup, "on_sound", constrain (eff_sound_on,0,1));
//#endif //MP3_TX_PIN
  
  
  // WI-FI
  
  LOG.printf_P(PSTR("\nРабочий режим лампы: ESP_MODE = %d\n"), espMode);
  //Запускаем WIFI
  LOG.println(F("Старуем WIFI"));
  
  WiFi.persistent(false);   // Побережём EEPROM
 
  if (espMode == 0U)                                        // режим WiFi точки доступа
  {
	// Отключаем WIFI
	WiFi.disconnect();
	// Меняем режим на режим точки доступа
	WiFi.mode(WIFI_AP);
	// Задаем настройки сети
    if (sizeof(AP_STATIC_IP))
    {
      WiFi.softAPConfig(                      
        IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], AP_STATIC_IP[3]),      // IP адрес WiFi точки доступа
        IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], 1),                    // первый доступный IP адрес сети
        IPAddress(255, 255, 255, 0));                                                       // маска подсети
    }
	// Включаем WIFI в режиме точки доступа с именем и паролем
	// хронящихся в переменных _ssidAP _passwordAP в фвйле config.json
	WiFi.softAP(AP_NAME, AP_PASS);
    LOG.print(F("Старт WiFi в режиме точки доступа\n"));
    LOG.print(F("IP адрес: "));
    LOG.println(WiFi.softAPIP());
   #ifdef GENERAL_DEBUG
    LOG.println (F("*******************************************"));
    LOG.print (F("Heap Size after connection AP mode = "));
    LOG.println(system_get_free_heap_size());
    LOG.println (F("*******************************************"));
    #endif    
	connect = true;
    delay (100);    
  }
  else                                                      // режим WiFi клиента. Подключаемся к роутеру
  {
    LOG.println(F("Старт WiFi в режиме клиента (подключение к роутеру)"));
//	WIFI_start_station_mode (); 
	
   
   WiFi.persistent(false);

  // Попытка подключения к Роутеру
  WiFi.mode(WIFI_STA);
  String _ssid = jsonRead(configSetup, "ssid");
  String _password = jsonRead(configSetup, "password");
  if (_ssid == "" && _password == "") {
   espMode = 0;
   jsonWrite(configSetup, "ESP_mode", (int)espMode);
   saveConfig(); 
   ESP.restart();
  }
  else {
    WiFi.begin(_ssid.c_str(), _password.c_str());
  }
		
	delay (100);	  
    #ifdef USE_BLYNK
    Blynk.config(USE_BLYNK);
    #endif
  }     //if (espMode == 0U) {...} else {...
  
  ESP.wdtFeed();

  LOG.printf_P(PSTR("Порт UDP сервера: %u\n"), localPort);
  Udp.begin(localPort);


  // NTP
  #ifdef USE_NTP
  timeClient.begin();
  ESP.wdtFeed();
  #endif

  // MQTT
  #if (USE_MQTT)
  if (espMode == 1U)
  {
    mqttClient = new AsyncMqttClient();
    MqttManager::setupMqtt(mqttClient, inputBuffer, &sendCurrent);    // создание экземпляров объектов для работы с MQTT, их инициализация и подключение к MQTT брокеру
  }
  ESP.wdtFeed();
  #endif

  // ОСТАЛЬНОЕ
  memset(matrixValue, 0, sizeof(matrixValue)); //это массив для эффекта Огонь. странно, что его нужно залить нулями
  randomSeed(micros());
  changePower();
  loadingFlag = true;
  
  #ifdef MP3_TX_PIN
   mp3.begin(9600);
   LOG.println (F("Старт mp3 player"));
   //mp3_setup();
   mp3_timer = millis();
   mp3_player_connect = 1;
  #endif 

  //TextTicker = RUNNING_TEXT_DEFAULT;
  delay (100);
  
#ifdef TM1637_USE
  DisplayTimer = millis();
 #ifdef MP3_TX_PIN
    CurrentFolder = effects_folders[currentMode];
    jsonWrite(configSetup, "fold_sel", CurrentFolder);
 #endif  //MP3_TX_PIN
#endif  //TM1637_USE
  
  my_timer=millis();
  
  #ifdef HEAP_SIZE_PRINT
   mem_timer = millis();
  #endif //HEAP_SIZE_PRINT 
}


void loop()  //====================================================================  void loop()  ===========================================================================
{
  
 if (espMode) {
  if (WiFi.status() != WL_CONNECTED) {
	if ((millis()-my_timer) >= 1000UL) {	
	  my_timer=millis();
	  if (ESP_CONN_TIMEOUT--) {
		LOG.print(F("."));
		ESP.wdtFeed();
	  }
	  else {
		// Если не удалось подключиться запускаем в режиме AP
		espMode = 0;
		jsonWrite(configSetup, "ESP_mode", (int)espMode);
		saveConfig(); 
		ESP.restart();
		//StartAPMode();
	  }
	}
  }
	else {
		// Иначе удалось подключиться отправляем сообщение
		// о подключении и выводим адрес IP
		LOG.print(F("\nПодключение к роутеру установлено\n"));
		LOG.print(F("IP адрес: "));
		LOG.println(WiFi.localIP());
		long rssi = WiFi.RSSI();
		LOG.print(F("Уровень сигнала сети RSSI = "));
		LOG.print(rssi);
		LOG.println(F(" dbm"));
		connect = true;
		//ESP_CONN_TIMEOUT = 0;
		lastResolveTryMoment = 0;
      #ifdef GENERAL_DEBUG
        LOG.println (F("***********************************************"));
        LOG.print (F("Heap Size after connection Station mode = "));
        LOG.println(system_get_free_heap_size());
        LOG.println (F("***********************************************"));
      #endif
      #ifdef DISPLAY_IP_AT_START
        loadingFlag = true;
      #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, матрица должна быть включена на время вывода текста
        digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
      #endif
        while(!fillString(WiFi.localIP().toString().c_str(), CRGB::White, false)) { delay(1); ESP.wdtFeed(); }
      #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы или будильника
        digitalWrite(MOSFET_PIN, ONflag || (dawnFlag && !manualOff) ? MOSFET_LEVEL : !MOSFET_LEVEL);
      #endif
        loadingFlag = true;
      #endif  // DISPLAY_IP_AT_START
		delay (100);	  
	}
 }
 
 if (connect || !espMode)  { my_timer = millis(); }
  
do {	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++========= Главный цикл ==========+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Если не устойчивое подключение к WiFi, или не создаётся точка доступа, или лампа не хочет подключаться к вашей сети или вы не можете подключиться к точке доступа, то может быть у вас не качественная плата.
  delay (0);   //Для некоторых плат ( особенно без металлического экрана над ESP и Flash памятью ) эта задержка должна быть увеличена. Подбирается индивидуально в пределах 1-12 мс до устойчивой работы WiFi. Чем меньше, тем лучше. Качественные платы работают с задержкой 0.
  yield();
  
	if ((connect || !espMode)&&((millis() - my_timer) >= 10UL)) 
	{
	HTTP.handleClient(); // Обработка запросов web страницы. 
	my_timer = millis();
	}
 
  //HTTP.handleClient(); // Обработка запросов web страницы. 
  parseUDP();
  yield();
  #ifdef TM1637_USE
    if (millis() - tmr_clock > 500UL) {         // каждую секунду изменяем
      tmr_clock = millis();                  // обновляем значение счетчика
      dotFlag = !dotFlag;                    // инверсия флага
      if (!DisplayFlag) display.point(dotFlag); // выкл/выкл двоеточия
      Display_Timer ();
    }
    if (dawnFlag) {
    clockTicker_blink();
    }
  #endif  //TM1637_USE
  #ifdef MP3_TX_PIN
  switch (mp3_player_connect){
      case 0: break;
      case 1: if (millis() - mp3_timer > 3000UL || (read_command(10) == 0x3F)){  //if (millis() - mp3_timer > 5000) {
                 first_entry = 5;
                 mp3_timer = millis();
                 mp3_setup ();
                }
              break;
      case 2: if ( millis() - mp3_timer > 3500UL ) mp3_player_connect = 3;//if ((millis() - mp3_timer > 5000UL) || (read_command(10) == 0x3F)) mp3_player_connect = 3;
              break;
      case 3: mp3_setup(); break;
      case 4: mp3_loop(); break;
      }
                    
  #endif

 if (Painting == 0) {

  effectsTick();
  
  #ifdef HEAP_SIZE_PRINT
   if (millis() - mem_timer > 10000UL) {
       mem_timer = millis();
       LOG.print (F("Heap Size = "));
       LOG.println(system_get_free_heap_size());
   }
  #endif //HEAP_SIZE_PRINT

  EepromManager::HandleEepromTick(&settChanged, &eepromTimeout, &ONflag, 
    &currentMode, modes, &(FavoritesManager::SaveFavoritesToEeprom));
    yield();

  //#ifdef USE_NTP
  #if defined(USE_NTP) || defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
  //if (millis() > 30 * 1000U) можно попытаться оттянуть срок первой попытки синхронизации времени на 30 секунд, чтобы роутер успел не только загрузиться, но и соединиться с интернетом
    timeTick();
  #endif

  #ifdef ESP_USE_BUTTON
  //if (buttonEnabled) в процедуре ведь есть эта проверка
    buttonTick();
  #endif

  #ifdef OTA
  otaManager.HandleOtaUpdate();                             // ожидание и обработка команды на обновление прошивки по воздуху
  #endif

  TimerManager::HandleTimer(&ONflag, &settChanged,          // обработка событий таймера отключения лампы
    &eepromTimeout, &changePower);
  
  if (FavoritesManager::HandleFavorites(                    // обработка режима избранных эффектов
      &ONflag,
      &currentMode,
      &loadingFlag
      //#ifdef USE_NTP
      #if defined(USE_NTP) || defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
      , &dawnFlag
      #endif
      , &random_on
      , &selectedSettings
      ))
  {
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
    FastLED.setBrightness(modes[currentMode].Brightness);
  }

  #if USE_MQTT
  if (espMode == 1U && mqttClient && WiFi.isConnected() && !mqttClient->connected())
  {
    MqttManager::mqttConnect();                             // библиотека не умеет восстанавливать соединение в случае потери подключения к MQTT брокеру, нужно управлять этим явно
    MqttManager::needToPublish = true;
  }

  if (MqttManager::needToPublish)
  {
    if (strlen(inputBuffer) > 0)                            // проверка входящего MQTT сообщения; если оно не пустое - выполнение команды из него и формирование MQTT ответа
    {
      processInputBuffer(inputBuffer, MqttManager::mqttBuffer, true);
    }
    
    MqttManager::publishState();
  }
  #endif

  #ifdef USE_BLYNK
  if (espMode == 1U && WiFi.isConnected())
    Blynk.run();
  #endif

  #if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  handleTelnetClient();
  #endif
 }//if (Painting == 0)
  yield();
  ESP.wdtFeed();
  //delay (7);
} while (connect);
}
