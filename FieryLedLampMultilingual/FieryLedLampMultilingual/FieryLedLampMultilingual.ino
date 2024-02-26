// 
// ========================================= УВАГА !!! ==============================================
// Усі налаштування робляться на вкладці Constants.h
// Почитайте там те, що написано україньскою або російською мовою.
// Або нічого не чіпайте, якщо збирали, за схемами з цього архіву.
// У будь-якому випадку УВАЖНО прочитайте файл ПРОЧИТАЙ МЕНЕ!!!.txt з цього архіву.
// У нагоді також буде перечітати файл Version.txt починаючи з версії 2.14
//
// -- ВСІ БІБЛІОТЕКИ БЕРЕМО З АРХИВА ЦІЄЇ ПРОШИВЦІ  --
//
// ==================================================================================================
//
// Посилання для менеджера плат Arduino:
// ESP8266 :
// https://arduino.esp8266.com/stable/package_esp8266com_index.json
// При установці вибираємо версію ядра 2.7.4
// РАСПОДІЛЕННЯ  КОНТАКТІВ  МОДУЛЯ ESP8266 ДИВИСЬ У ФАЙЛИ Constants.h або на схемі
//
// ESP32 :
// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// Перевірялось на ядрі 1.0.6 та 2.0.14. На ядрі 1.0.6 код займає на 4% меньше місця.
// Для використання ядра 1.0.6 закоментуйте відповідний рядок нижче.
// Вибираємо плату ESP32 Dev Module. На платах ESP32S2, ESP32S3, ESP32C2 робота не перевірялась
// РАСПОДІЛЕННЯ  КОНТАКТІВ  МОДУЛЯ ESP32 ДИВИСЬ У ФАЙЛИ Constants.h
//
// ==================================================================================================

// ******************************* - ВИБІР ПЛАТИ ТА ЯДРА- *******************************************
#ifdef ESP32           // Визначено в IDE якщо використовуется ESP32 (не треба змінювати для ESP8266)
 #define ESP32_USED    // Використовується контолер ESP32 (не треба змінювати для ESP8266)
 #define CORE_2_0_X    // Якщо використовується ядро ESP32 версії 1.0.Х закоментуйте цей рядок
#endif
// **************************************************************************************************

//===================================================================================================
//
// Далі йде код проекту. Не змінюйте тут нічого, якщо Ви не розумієте, до чого це призведе!!!
//
//===================================================================================================


#include <pgmspace.h>
#ifdef ESP32_USED
 #include "esp_wifi.h"                // Борьба с рестартом esp32 "assertion "Invalid mbox""
 #include "nvs_flash.h"               // Борьба с рестартом esp32 "assertion "Invalid mbox""
 #include <WiFi.h>
 #include <WiFiClient.h>
 #include <WiFiAP.h>
 #include <WebServer.h>
 #include <ESP32SSDP.h>               // https://github.com/luc-github/ESP32SSDP
 #include <HTTPUpdateServer.h>        // Обновление с web страницы
 #include <time.h>
 #include <HardwareSerial.h>          // Вbкористовуєтся апаратний UART
 #include "esp_system.h"
 #include "esp_int_wdt.h"
 #include "esp_task_wdt.h"

#else
 #include <ESP8266SSDP.h>
 #include <ESP8266HTTPUpdateServer.h> // Обновление с web страницы
 #include <ESP8266WiFi.h>
 #include <ESP8266WebServer.h> 
 #define FASTLED_USE_PROGMEM 1        // просим библиотеку FASTLED экономить память контроллера на свои палитры
#endif

#include <FastLED.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include "Constants.h"
#ifdef ESP_USE_BUTTON
#include <GyverButton.h>
#endif
#ifdef USE_NTP
#include <NTPClient.h>
#include <Timezone.h>
#endif
#include "Types.h"
#include "timerMinim.h"
#include "fonts.h"
#ifdef OTA
#include "OtaManager.h"
#endif
#if USE_MQTT
#include "MqttManager.h"
#endif
#include "EepromManager.h"
#include "FavoritesManager.h"
#include "TimerManager.h"
#ifdef USE_BLYNK
 #ifdef ESP32_USED
  #include <BlynkSimpleEsp32.h>
 #else
  #include <BlynkSimpleEsp8266.h>
 #endif
#endif
//#ifdef ESP32_USED
// #include "esp_system.h"
// #include "esp_int_wdt.h"
// #include "esp_task_wdt.h"
//#endif
#ifdef TM1637_USE
#include "TM1637Display.h"
#endif
#ifdef MP3_PLAYER_USE
 #ifndef ESP32_USED
  #include <SoftwareSerial.h>     // Подключаем библиотеку для работы с последовательным интерфейсом
 #endif
 #ifdef MP3_DEBUG
  #define FEEDBACK  1
 #else
  #define FEEDBACK  0
 #endif  //MP3_DEBUG
#endif  // MP3_PLAYER_USE
#ifdef IR_RECEIVER_USE
 #include <IRremoteESP8266.h>  // https://github.com/crankyoldgit/IRremoteESP8266
 #include <IRrecv.h>
 #include "IR_Receiver.h"
#endif  //IR_RECEIVER_USE


// --- ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ ----------
CRGB leds[NUM_LEDS];
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
//char* MqttManager::mqttServer = NULL;
//char* MqttManager::mqttUser = NULL;
//char* MqttManager::mqttPassword = NULL;
char* MqttManager::clientId = NULL;
char* MqttManager::lampInputBuffer = NULL;
char* MqttManager::topicInput = NULL;
#ifdef PUBLISH_STATE_IN_OLD_FORMAT
char* MqttManager::topicOutput = NULL;
#endif
char* MqttManager::topicOutputJSON = NULL;
bool MqttManager::needToPublish = false;
char MqttManager::mqttBuffer[] = {};
uint32_t MqttManager::mqttLastConnectingAttempt = 0;
SendCurrentDelegate MqttManager::sendCurrentDelegate = NULL;
#endif

#ifdef ESP32_USED
 HTTPUpdateServer httpUpdater;           // Объект для обнавления с web страницы
 WebServer HTTP (ESP_HTTP_PORT);         // Объект для обнавления с web страницы
#else
 ESP8266HTTPUpdateServer httpUpdater;    // Объект для обнавления с web страницы
 ESP8266WebServer HTTP (ESP_HTTP_PORT);  // Web интерфейс для устройства
#endif
File fsUploadFile;                       // Для файловой системы



// --- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ -------
uint16_t localPort = ESP_UDP_PORT;
char packetBuffer[MAX_UDP_BUFFER_SIZE];  // buffer to hold incoming packet
char inputBuffer[MAX_UDP_BUFFER_SIZE];
static const uint8_t maxDim = max(WIDTH, HEIGHT);


AlarmType alarms[7];

static const uint8_t dawnOffsets[] PROGMEM = {5, 10, 15, 20, 25, 30, 40, 50, 60};   // опции для выпадающего списка параметра "время перед 'рассветом'" (будильник); синхронизировано с android приложением
uint8_t dawnMode;
bool dawnFlag = false;
uint32_t thisTime;
bool manualOff = false;

int16_t offset = WIDTH;
uint32_t scrollTimer = 0LL;

uint8_t currentMode;
bool loadingFlag = true;
bool ONflag = false;
//uint32_t eepromTimeout;
//bool settChanged = false;
#ifdef ESP_USE_BUTTON
 bool buttonEnabled = true; // Вкл \ откл кнопки
 #if defined(BUTTON_LOCK_ON_START)
  bool buttonBlocing = false;
 #endif
#endif
unsigned char matrixValue[8][16]; //это массив для эффекта Огонь

bool TimerManager::TimerRunning = false;
bool TimerManager::TimerHasFired = false;
uint8_t TimerManager::TimerOption = 1U;
uint32_t TimerManager::TimeToFire = 0ULL;

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

#ifdef USE_MULTIPLE_LAMPS_CONTROL
char Host1[16], Host2[16], Host3[16];
uint8_t ml1, ml2, ml3;
#endif //USE_MULTIPLE_LAMPS_CONTROL

#ifdef MP3_PLAYER_USE
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
uint8_t mp3_folder_last=255;         // Предыдущая папка для воспроизведения
//uint8_t mp3_folder_change =0;        // Указуе, була-ли змінена папка
bool set_mp3_play_now=false;         // Указывает, надо ли играть сейчас мелодии
uint32_t alarm_timer;                // Периодичность проверки и плавного изменения громкости будильника
uint32_t mp3_timer = 0;
bool mp3_stop = true;                        // Озвучка эффектов остановлена
bool pause_on = true;                        // Озвучка эффектов на паузе. false - не на паузе
uint8_t eff_volume = 9;                      // громкость воспроизведения
uint8_t eff_sound_on = 0;                    // звук включен - !0 (true), выключен - 0
uint8_t CurrentFolder;                       // Папка, на которую переключились (будет проигрываться)
uint8_t CurrentFolder_last = 0;                  // Попередня текуща папка
#ifdef ESP32_USED
 HardwareSerial mp3(1);  // Використовуємо UART1
#else
 SoftwareSerial mp3(MP3_RX_PIN, MP3_TX_PIN);  // создаём объект mySoftwareSerial и указываем выводы, к которым подлючен плеер (RX, TX)
#endif
//#ifndef TM1637_USE
// uint8_t minute_tmp;
//#endif
uint8_t mp3_receive_buf[10];
uint8_t effects_folders[MODE_AMOUNT];    // Номера папок для озвучивания
uint16_t ADVERT_TIMER_H, ADVERT_TIMER_M; // тривалість озвучування годин та хвилин
uint8_t mp3_delay;                       // Затрімка між командами плеєру
uint8_t send_sound = 1;                  // Передавати або ні вагалі параметри звуку (папка,озвучування_on/off,гучнисть)
uint8_t send_eff_volume = 1;             // Передавати або ні озвучування_on/off, гучність
#endif  // MP3_PLAYER_USE
#ifdef TM1637_USE
uint8_t DispBrightness = 1;          // +++ Яркость дисплея от 0 до 255(5 уровней яркости с шагом 51). 0 - дисплей погашен 
bool dotFlag = false;                // +++ флаг: в часах рисуется двоеточие или нет
uint32_t tmr_clock = 0;              // +++ таймер мигания разделителя часов на дисплее
uint32_t tmr_blink = 0;              // +++ таймер плавного изменения яркости дисплея
TM1637Display display(CLK, DIO);     // +++ подключаем дисплей
bool aDirection = false;             // +++ Направление изменения яркрсти
uint32_t DisplayTimer;               // Время отображения номера эффекта
uint8_t LastEffect = 255;            // последний Проигрываемый эффект
uint8_t DisplayFlag=0;               // Флаг, показывающий, что отображается номер эффекта и папки
 #ifdef MP3_PLAYER_USE
 uint8_t LastCurrentFolder = 255;    // Проигрываемая папка
 #endif  // MP3_PLAYER_USE
#endif  //TM1637_USE

#ifdef HEAP_SIZE_PRINT
uint32_t mem_timer;
#endif //HEAP_SIZE_PRINT 

#ifdef IR_RECEIVER_USE
 uint32_t IR_Code = 0x00000000;
 uint32_t IR_Repeat_Timer;
 uint32_t IR_Tick_Timer;
 uint32_t IR_Dgit_Enter_Timer;
 uint8_t Repeat; 
 uint8_t IR_Data_Ready;
 uint8_t Enter_Digit_1;
 uint8_t Enter_Number;

 IRrecv irrecv(IR_RECEIVER_PIN);
 decode_results results;
#endif  //IR_RECEIVER_USE

uint8_t RuninTextOverEffects = 0;
uint32_t Last_Time_RuninText = 0;
bool Fill_String = false;
uint8_t SpeedRunningText;
uint8_t ColorRunningText;
uint8_t ColorTextFon;

uint8_t use_static_ip = 0;
IPAddress Static_IP;//(192,168,0,17);  // статичний IP
IPAddress Gateway;//(192,168,0,1);     // шлюз
IPAddress Subnet;//(255,255,255,0);    // маска підмережи
IPAddress DNS1;//(208,67,222,222);     //DNS сервери. Можна також DNS1(1,1,1,1) або DNS1(8,8,4,4);
IPAddress DNS2(8,8,8,8);               //Резервний DNS

uint8_t C_flag = 0;                    // Службове
uint16_t current_limit;                // ліміт струму, що настроюється
uint8_t last_minute;                   // хвилини
uint8_t hours;                         // години
//uint8_t last_hours; 
uint8_t m_date,d_date;                 // дата
uint8_t AutoBrightness;                // Автояскравість on/off
uint8_t last_day_night = 0;

void setup()  //==================================================================  void setup()  =========================================================================
{

  Serial.begin(115200);
  delay(300);
  #ifdef ESP32_USED
  esp_task_wdt_init(8, true);   // Initialize the task watchdog timer
  #else
  ESP.wdtEnable(WDTO_8S);
  #endif

  LOG.print(F("\n\n\nSYSTEM START"));
  #ifdef ESP32
  LOG.print (F("  ESP32\n"));
  #endif
  #ifdef ESP8266
  LOG.print (F("  ESP8266\n"));
  #endif


  #if defined(ESP_USE_BUTTON) && defined(BUTTON_LOCK_ON_START)
    #if (BUTTON_IS_SENSORY == 1)
        if (digitalRead(BTN_PIN)) {
            buttonBlocing = true;
        }
    #endif
    #if (BUTTON_IS_SENSORY == 0)
        if (!digitalRead(BTN_PIN)) {
            buttonBlocing = true;
        }
    #endif
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
  
  // часы
#ifdef TM1637_USE
  LOG.print(F("\nСтарт дисплея TM1637\n"));
  tmr_clock = millis();                                     // +++ устанавливаем начальное значение счетчика
  display.setBrightness(DispBrightness);                    // +++ яркость дисплея максимальная = 255
  display.displayByte(_empty, _empty, _empty, _empty);      // +++ очистка дисплея
  display.displayByte(_dash, _dash, _dash, _dash);          // +++ отображаем прочерки
#endif

   //File Fystem
  #ifdef GENERAL_DEBUG  
  LOG.print(F("\nСтарт файлової системи\n"));
  #endif
  FS_init();  //Запускаем файловую систему
  #ifdef GENERAL_DEBUG
  LOG.print(F("Читання файла конфигурації\n"));
  #endif
  configSetup = readFile(F("config.json"), 2048);
  #ifdef GENERAL_DEBUG  
  LOG.println(configSetup);
  #endif
  //Настраиваем и запускаем SSDP интерфейс
  #ifdef GENERAL_DEBUG
  LOG.print(F("Старт SSDP\n"));
  #endif
  SSDP_init();

  
//-----------Инициализируем переменные, хранящиеся в файле config.json--------------
  LAMP_NAME = jsonRead(configSetup, "SSDP");
  AP_NAME = jsonRead(configSetup, "ssidAP");
  AP_PASS = jsonRead(configSetup, "passwordAP");
  Favorit_only = jsonReadtoInt(configSetup, "favorit");
  random_on = jsonReadtoInt(configSetup, "random_on");
  espMode = jsonReadtoInt(configSetup, "ESP_mode");
  PRINT_TIME = jsonReadtoInt(configSetup, "print_time");
  #ifdef ESP_USE_BUTTON
   buttonEnabled = jsonReadtoInt(configSetup, "button_on");
  #endif
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
  RuninTextOverEffects = jsonReadtoInt(configSetup, "toe");  // Виводити рядок, що бежить поверх єфектів
  SpeedRunningText = jsonReadtoInt(configSetup, "spt");      // Швидкість рядка, що бежить
  ColorRunningText = jsonReadtoInt(configSetup, "sct");      // Колір рядка, що бежить
  ColorTextFon = jsonReadtoInt(configSetup, "ctf");          //виводити рядок, що бежить, на кольоровом фоні
  jsonWrite(configSetup, "ver", FLL_VERSION);                // Версія ПЗ
  AutoBrightness = jsonReadtoInt(configSetup, "auto_bri");   // Автоматичне зниження яскравості on/off
  #ifdef USE_NTP
  (jsonRead(configSetup, "ntp")).toCharArray (NTP_ADDRESS, (jsonRead(configSetup, "ntp")).length()+1);
  #endif
  #ifdef USE_NTP
  winterTime.offset = jsonReadtoInt(configSetup, "timezone") * 60;
  summerTime.offset = winterTime.offset + jsonReadtoInt(configSetup, "Summer_Time") *60;
  localTimeZone.setRules (summerTime, winterTime);
  #endif
  #ifdef MP3_PLAYER_USE
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
  send_sound = jsonReadtoInt(configSetup, "s_s");
  send_eff_volume = jsonReadtoInt(configSetup, "s_e_v");
  #endif // MP3_PLAYER_USE
  {
  String configHardware = readFile(F("hardware_config.json"), 1024);    
  current_limit = jsonReadtoInt(configHardware, "cur_lim");
  MATRIX_TYPE = jsonReadtoInt(configHardware, "m_t");
  ORIENTATION = jsonReadtoInt(configHardware, "m_o");
  #ifdef MP3_PLAYER_USE
  ADVERT_TIMER_H = 100 * jsonReadtoInt(configHardware, "tim_h");
  ADVERT_TIMER_M = 100 * jsonReadtoInt(configHardware, "tim_m");
  mp3_delay = 10 * jsonReadtoInt(configHardware, "delay");
  #ifdef GENERAL_DEBUG
     LOG.print (F("\nADVERT_TIMER_H = "));
     LOG.println (ADVERT_TIMER_H);
     LOG.print (F("ADVERT_TIMER_M = "));
     LOG.println (ADVERT_TIMER_M);
     LOG.print (F("mp3_delay = "));
     LOG.println (mp3_delay);
  #endif

  #endif
  }
  {
    String Name = F("correct.");
    Name.reserve(17);
    Name += jsonRead (configSetup, "lang");
    Name += F(".json");
    String Correct = readFile(Name, 2048);
    for ( uint8_t n=0; n< MODE_AMOUNT; n++)
    {
        eff_num_correct[n] = jsonReadtoInt (Correct, String(n));
    }
  }

  {
  String configIP = readFile(F("ip_config.json"), 512);
  use_static_ip = jsonReadtoInt(configSetup, "s_IP");
  Static_IP.fromString(jsonRead(configIP, "ip"));
  Gateway.fromString(jsonRead(configIP, "gateway"));
  Subnet.fromString(jsonRead(configIP, "subnet"));
  DNS1.fromString(jsonRead(configIP, "dns"));
  #ifdef GENERAL_DEBUG
     LOG.print (F("\nUse Static IP = "));
     LOG.println (use_static_ip);
     LOG.print (F("Static IP = "));
     LOG.println (Static_IP);
     LOG.print (F("Gateway = "));
     LOG.println (Gateway);
     LOG.print (F("Subnet = "));
     LOG.println (Subnet);
     LOG.print (F("DNS1 = "));
     LOG.println (DNS1);
     LOG.print (F("DNS2 = "));
     LOG.println (DNS2);
  #endif
  }

  // TELNET
  #if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  telnetServer.begin();
  for (uint8_t i = 0; i < 100; i++)                         // пауза 10 секунд в отладочном режиме, чтобы успеть подключиться по протоколу telnet до вывода первых сообщений
  {
    handleTelnetClient();
    delay(100);
    #ifdef ESP32_USED
     esp_task_wdt_reset();
    #else
     ESP.wdtFeed();
    #endif
  }
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


  // КНОПКА
  #if defined(ESP_USE_BUTTON)
  touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  touch.setDebounce(BUTTON_SET_DEBOUNCE);
    #ifdef BUTTON_LOCK_ON_START
    if (buttonBlocing) {
       buttonEnabled = false;
       //jsonWrite(configSetup, "button_on", buttonEnabled);
       //saveConfig();
    }
    #ifdef ESP32_USED
     esp_task_wdt_reset();
    #else
     ESP.wdtFeed();
    #endif
    #endif
  #endif

#ifdef USE_SHUFFLE_FAVORITES // первоначальная очередь избранного до перемешивания
    for (uint8_t i = 0; i < MODE_AMOUNT; i++)
      shuffleFavoriteModes[i] = i;
#endif


  // EEPROM
  EepromManager::InitEepromSettings(modes, &(restoreSettings)); // инициализация EEPROM; запись начального состояния настроек, если их там ещё нет; инициализация настроек лампы значениями из EEPROM
 // не придумал ничего лучше, чем делать восстановление настроек по умолчанию в обработчике инициализации EepromManager
    

  if(DONT_TURN_ON_AFTER_SHUTDOWN){
      ONflag = false;
  jsonWrite(configSetup, "Power", ONflag);
  }
  else
      ONflag = jsonReadtoInt (configSetup, "Power");  // Чтение состояния лампы вкл/выкл,текущий эффект,яркость,скорость,масштаб

  currentMode = eff_num_correct[jsonReadtoInt (configSetup, "eff_sel")];
  modes[currentMode].Brightness = jsonReadtoInt (configSetup, "br");
  modes[currentMode].Speed = jsonReadtoInt (configSetup, "sp");
  modes[currentMode].Scale = jsonReadtoInt (configSetup, "sc");
  first_entry = 1;
  handle_alarm ();
  first_entry = 0;
  FavoritesManager::FavoritesRunning = jsonReadtoInt(configSetup, "cycle_on");  // чтение состояния настроек режима Цикл 
  FavoritesManager::Interval = jsonReadtoInt(configSetup, "time_eff");          // вкл/выкл,время переключения,дисперсия,вкл цикла после перезагрузки
  FavoritesManager::Dispersion = jsonReadtoInt(configSetup, "disp");
  FavoritesManager::UseSavedFavoritesRunning = jsonReadtoInt(configSetup, "cycle_allwase");
  jsonWrite(configSetup, "tmr", 0);
  #ifdef ESP_USE_BUTTON
  jsonWrite(configSetup, "button_on", buttonEnabled);
  #endif
  first_entry = 1;
  handle_cycle_set();  // чтение выбранных эффектов
  first_entry = 0;
#ifdef MP3_PLAYER_USE
  first_entry = 1;
  handle_sound_set();  //чтение выбранных папок
  first_entry = 0;
#endif  // MP3_PLAYER_USE
#ifdef USE_MULTIPLE_LAMPS_CONTROL  
  multilamp_get ();   // Чтение из файла адресов синхронно управляемых ламп 
#endif //USE_MULTIPLE_LAMPS_CONTROL
  
  // MP3 Player
   
  #ifdef MP3_PLAYER_USE
   #ifdef ESP32_USED
    mp3.begin(9600, SERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN);
   #else
    mp3.begin(9600);
   #endif
   LOG.println (F("\nСтарт MP3 Player"));
   mp3_timer = millis();
   mp3_player_connect = 1;
  #endif 

  // WI-FI
  
  LOG.printf_P(PSTR("\nРобочий режим лампи: ESP_MODE = %d\n"), espMode);

  //Запускаем WIFI  
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
    #ifdef ESP32_USED
     WiFi.softAP(AP_NAME.c_str(), AP_PASS.c_str());
    #else
     WiFi.softAP(AP_NAME, AP_PASS);
    #endif
    LOG.print(F("Старт WiFi в режиме точки доступа\n"));
    LOG.print(F("IP адрес: "));
    LOG.println(WiFi.softAPIP());
   #ifdef GENERAL_DEBUG
    LOG.println (F("*******************************************"));
    LOG.print (F("Heap Size after connection AP mode = "));
    LOG.println(ESP.getFreeHeap());
    LOG.println (F("*******************************************"));
    #endif    
    connect = true;
    delay (100);    
  }
  else                                                      // режим WiFi клиента. Подключаемся к роутеру
  {
    LOG.print(F("\nПідключення до мережі "));
       
   WiFi.persistent(false);

  // Попытка подключения к Роутеру
  {
  WiFi.mode(WIFI_STA);
  String _ssid = jsonRead(configSetup, "ssid");
  LOG.println(_ssid);
  char* Pass_STA = new char[64];
  char* SSID_STA = new char[32];
  _ssid.toCharArray(SSID_STA, _ssid.length()+1);
  #ifdef GENERAL_DEBUG
  LOG.print("\nPass_STA = ");
  #endif
  for (uint8_t address = 0; address < 64; address ++){
      Pass_STA[address] = EEPROM.read(EEPROM_PASSWORD_START_ADDRESS + address);
      #ifdef GENERAL_DEBUG
      //LOG.print(Pass_STA[address]);
      #endif
      if (Pass_STA[address] == 0) break;
  }
  #ifdef GENERAL_DEBUG
  LOG.println(Pass_STA );
  #endif
  if (_ssid == "") {
     espMode = 0;
     jsonWrite(configSetup, "ESP_mode", (int)espMode);
     saveConfig(); 
     ESP.restart();
  }
  else
  {

    if(use_static_ip)
    {  
        WiFi.config(Static_IP, Gateway, Subnet, DNS1, DNS2); // Конфігурація під статичний IP Address
    }
    delay(10);  
    WiFi.begin("Pestel" , "BarikMobarik"); //WiFi.begin(SSID_STA, Pass_STA); //WiFi.begin(_ssid.c_str(), _password.c_str()); //
    delete [] Pass_STA;
    delete [] SSID_STA;
  }
  }
    delay (10);   
    #ifdef USE_BLYNK
    Blynk.config(USE_BLYNK);
    #endif
  }     //if (espMode == 0U) {...} else {...
  
    #ifdef ESP32_USED
     esp_task_wdt_reset();
    #else
     ESP.wdtFeed();
    #endif

   
  // UDP 
  LOG.printf_P(PSTR("\nСтарт UDP сервера. Порт: %u\n"), localPort);
  Udp.begin(localPort);

  //Настраиваем и запускаем HTTP интерфейс
  User_setings ();
  #ifdef GENERAL_DEBUG
  LOG.print (F("Старт WebServer\n"));
  #endif
  HTTP_init();
  WiFiClient client;  //Declare an object of class HTTPClient
 
 // NTP
  #ifdef USE_NTP
  timeClient.begin();
    #ifdef ESP32_USED
     esp_task_wdt_reset();
    #else
     ESP.wdtFeed();
    #endif
  #endif


  // MQTT
  #if (USE_MQTT)
  String configMQTT = readFile(F("mqtt_config.json"), 512);
  String str;
  if(!MqttServer.fromString(jsonRead(configMQTT, "mq_ip"))){
        jsonRead(configMQTT, "mq_ip").toCharArray(MqttHost, jsonRead(configMQTT, "mq_ip").length()+1);
        mqttIPaddr = false;
/*        #ifdef ESP32_USED
         WiFi.hostByName(MqttHost, MqttServer);
        #else
         WiFi.hostByName(MqttHost, MqttServer, RESOLVE_TIMEOUT);
        #endif
*/
  }
  else
      mqttIPaddr = true;
  str = jsonRead(configMQTT, "mq_user");
  str.toCharArray(MqttUser, str.length()+1);
  str = jsonRead(configMQTT, "mq_pass");
  str.toCharArray(MqttPassword, str.length()+1);
  str = jsonRead(configMQTT, "topic");
  str.toCharArray(TopicBase, str.length()+1);
  MqttPort = jsonReadtoInt(configMQTT, "mq_port");
  MqttOn = jsonReadtoInt(configMQTT, "mq_on");
  MqttPeriod = jsonReadtoInt(configMQTT, "mq_prd");
  #ifdef GENERAL_DEBUG
   LOG. println("Start MQTT");
   LOG.print("MQTT server ");
   if(mqttIPaddr)
       LOG.print(MqttServer);
   else
       LOG.print(MqttHost);
   LOG.print(": ");
   LOG.println(MqttPort);
   LOG.print("MQTT User - ");
   LOG.println(MqttUser);
   LOG.print("MQTT Password - ");
   LOG.println(MqttPassword);
  #endif //GENERAL_DEBUG

  if (espMode == 1U)
  {
    mqttClient = new AsyncMqttClient();
    MqttManager::setupMqtt(mqttClient, inputBuffer, &sendCurrent);    // создание экземпляров объектов для работы с MQTT, их инициализация и подключение к MQTT брокеру
  }
    #ifdef ESP32_USED
     esp_task_wdt_reset();
    #else
     ESP.wdtFeed();
    #endif
  #endif


  // ОСТАЛЬНОЕ
  memset(matrixValue, 0, sizeof(matrixValue)); //это массив для эффекта Огонь. странно, что его нужно залить нулями
  randomSeed(micros());
  changePower();
  loadingFlag = true;
  
  //IR receiver
  #ifdef IR_RECEIVER_USE
    irrecv.enableIRIn();  // Start the IR receiver
    IR_Tick_Timer = millis();
    IR_Repeat_Timer = millis();
  #endif  //IR_RECEIVER_USE

  //TM1637
  #ifdef TM1637_USE
    DisplayTimer = millis();
    #ifdef MP3_PLAYER_USE
      CurrentFolder = effects_folders[currentMode];
      mp3_folder = CurrentFolder;
      jsonWrite(configSetup, "fold_sel", CurrentFolder);
    #endif  // MP3_PLAYER_USE
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
        #ifdef ESP32_USED
         esp_task_wdt_reset();
        #else
         ESP.wdtFeed();
        #endif
      }
      else {
        // Если не удалось подключиться запускаем в режиме AP
        espMode = 0;
        jsonWrite(configSetup, "ESP_mode", (int)espMode);
        saveConfig(); 
        ESP.restart();
      }
    }
  }
    else {
        // Иначе удалось подключиться отправляем сообщение
        // о подключении и выводим адрес IP
        LOG.print(F("\nПідключення до роутера встановлено\n"));
        LOG.print(F("IP адреса: "));
        LOG.println(WiFi.localIP());
        long rssi = WiFi.RSSI();
        LOG.print(F("Рівень сигналу мережі RSSI = "));
        LOG.print(rssi);
        LOG.println(F(" dbm"));
        connect = true;
        lastResolveTryMoment = 0;
      #ifdef GENERAL_DEBUG
        LOG.println (F("***********************************************"));
        LOG.print (F("Heap Size after connection Station mode = "));
        LOG.println(ESP.getFreeHeap());
        LOG.println (F("***********************************************"));
      #endif
      #ifdef DISPLAY_IP_AT_START
        loadingFlag = true;
      #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, матрица должна быть включена на время вывода текста
        digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
      #endif
        while(!fillString(WiFi.localIP().toString().c_str(), CRGB::White, false)) {
           delay(1);
           #ifdef ESP32_USED
            esp_task_wdt_reset();
          #else
           ESP.wdtFeed();
          #endif
           }
        if (ColorTextFon  & (!ONflag || (currentMode == EFF_COLOR && modes[currentMode].Scale < 3))){
          FastLED.clear();
          delay(1);
          FastLED.show();
        }
      #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы или будильника
        digitalWrite(MOSFET_PIN, ONflag || (dawnFlag && !manualOff) ? MOSFET_LEVEL : !MOSFET_LEVEL);
      #endif
        loadingFlag = true;
      #endif  // DISPLAY_IP_AT_START
        delay (0);
    }
 }
 
 if (connect || !espMode)  { my_timer = millis(); }
 #ifdef MAIN_CYCLES_PER_SECOND
   int32_t my_timer2 = millis();
   uint16_t mcps_counter = 0;
 #endif 
do {    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++========= Главный цикл ==========+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Если не устойчивое подключение к WiFi, или не создаётся точка доступа, или лампа не хочет подключаться к вашей сети или вы не можете подключиться к точке доступа, то может быть у вас не качественная плата.
  delay (0);   //Для некоторых плат ( особенно без металлического экрана над ESP и Flash памятью ) эта задержка должна быть увеличена. Подбирается индивидуально в пределах 1-12 мс до устойчивой работы WiFi. Чем меньше, тем лучше. Качественные платы работают с задержкой 0.
  yield();
  
    //if ((connect || !espMode)&&((millis() - my_timer) >= 10UL)) 
    {
    HTTP.handleClient(); // Обработка запросов web страницы. 
    //my_timer = millis();
    }
 
  parseUDP();
  yield();
  #ifdef TM1637_USE
    if (millis() - tmr_clock > 500UL) {         // каждую секунду изменяем
      tmr_clock = millis();                     // обновляем значение счетчика
      dotFlag = !dotFlag;                       // инверсия флага
      if (!DisplayFlag) display.point(dotFlag); // выкл/выкл двоеточия
      Display_Timer ();
    }
    if (dawnFlag) {
    clockTicker_blink();
    }
  #endif  //TM1637_USE
  #ifdef MP3_PLAYER_USE
  switch (mp3_player_connect){
      case 0: break;
      case 1: read_command(1);
              if (millis() > 3000UL || mp3_receive_buf[3] == 0x3F){
                 first_entry = 5;
                 //mp3_timer = millis();
                 mp3_setup ();
                }
              break;
      case 2: read_command(1);
              if ( millis() - mp3_timer > 3000UL || mp3_receive_buf[3] == 0x3F) mp3_player_connect = 3;
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
       LOG.println(ESP.getFreeHeap());
   }
  #endif //HEAP_SIZE_PRINT
  
  #ifdef IR_RECEIVER_USE
       IR_Receive_Handle();
    if (millis() - IR_Tick_Timer > 100)
    {
        IR_Tick_Timer = millis();
        if (IR_Data_Ready) 
        {
            IR_Receive_Button_Handle();
            IR_Data_Ready =0;
        }       
    }
  #endif  //IR_RECEIVER_USE

  //EepromManager::HandleEepromTick(&settChanged, &eepromTimeout, modes);
    yield();

  //#ifdef USE_NTP
  #if defined(USE_NTP) || defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
    timeTick();
  #endif

  #ifdef ESP_USE_BUTTON
    buttonTick();
  #endif

  #ifdef OTA
  otaManager.HandleOtaUpdate();                             // ожидание и обработка команды на обновление прошивки по воздуху
  #endif
                                                            
  TimerManager::HandleTimer(&ONflag, //&settChanged, //&eepromTimeout, // обработка событий таймера отключения лампы
                            &timeout_save_file_changes,
                            &save_file_changes, &changePower);    
  
  if (FavoritesManager::HandleFavorites(                    // обработка режима избранных эффектов
      &ONflag,
      &currentMode,
      &loadingFlag
      #if defined(USE_NTP) || defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
      , &dawnFlag
      #endif
      , &random_on
      , &selectedSettings
      ,espMode
      ))
  {
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
    SetBrightness(modes[currentMode].Brightness);
  }

  #if USE_MQTT
  if (espMode == 1U && mqttClient && WiFi.isConnected() && !mqttClient->connected() && MqttOn)
  {
    MqttManager::mqttConnect();                             // библиотека не умеет восстанавливать соединение в случае потери подключения к MQTT брокеру, нужно управлять этим явно
    MqttManager::needToPublish = true;
  }

  if ((MqttManager::needToPublish || (MqttPeriod && (millis() - mqtt_timer) >= (MqttPeriod * 1000))) && MqttOn)
  //if ((MqttManager::needToPublish ) && MqttOn)
  {
    mqtt_timer = millis();
    if (strlen(inputBuffer) > 0)                            // проверка входящего MQTT сообщения; если оно не пустое - выполнение команды из него и формирование MQTT ответа
    {
      processInputBuffer(inputBuffer, MqttManager::mqttBuffer, true);
    }
  #ifdef PUBLISH_STATE_IN_OLD_FORMAT  
    MqttManager::publishState(0);  //публікація буфера MQTT відповіді в топік <TopicBase>/LedLamp_<ChipId>/state
  #endif // PUBLISH_STATE_IN_OLD_FORMAT
    String MqttSnd = "{\"power\":\"P_ON\"}"; //Рядок для відповіді "{"power":"P_ON","cycle":"FAV_OFF","effect":"111","bri":"15","spd":"33","sca":"58","runt":"10","runc":"123","runf":"1","runc":"220","rnde":"0","rndc":"1","rndf":"0","tmr":59900","volume":"10","sound":"SO_ON"}"
    jsonWrite(MqttSnd, "power", ONflag ? "P_ON" : "P_OFF");   // Створення рядку для MQTT відповіді у форматі JSON  у вигляді, як вище. 
    jsonWrite(MqttSnd, "cycle", FavoritesManager::FavoritesRunning ? "FAV_ON" : "FAV_OFF"); // Увімкнути / вимкнути режим "цикл" ("ізбранне")
     for ( uint8_t n=0; n< MODE_AMOUNT; n++)
     {
         if (eff_num_correct[n] == currentMode){
            jsonWrite(MqttSnd, "effect", (String)n);                    // Визначення назви ефекту за значенням currentMode в залежністі від вибранної мови
            break;
         } 
     } 
    jsonWrite(MqttSnd, "bri", (String)modes[currentMode].Brightness);   // Яскравість ефектів
    jsonWrite(MqttSnd, "spd", (String)modes[currentMode].Speed);        // Швидкість ефектів
    jsonWrite(MqttSnd, "sca", (String)modes[currentMode].Scale);        // Масштаб ефектів
    jsonWrite(MqttSnd, "runt", (String)RuninTextOverEffects);           // Періодичність віводу рядку,що біжить
    jsonWrite(MqttSnd, "runc", (String)ColorRunningText);               // Колір рядку,що біжить
    jsonWrite(MqttSnd, "runf", (String)ColorTextFon);                   // Фон рядку, що біжить. 0-чорний фон; 1-кольоровий фон.
    jsonWrite(MqttSnd, "runs", (String)SpeedRunningText);               // Швидкисть рядку, що біжить.
    jsonWrite(MqttSnd, "rnde", (String)FavoritesManager::rndCycle);     // Випадковий вибір ефектов в циклі
    jsonWrite(MqttSnd, "rndс", (String)random_on);                      // Випадковий вибір  налаштувань  ефектов в циклі
    jsonWrite(MqttSnd, "rndf", (String)selectedSettings);               // Випадковий вибір  налаштувань  поточного ефекту
    uint32_t temp = TimerManager::TimeToFire - millis();
    if (temp && TimerManager::TimerRunning)
        jsonWrite(MqttSnd, "tmr", (String)(temp / 1000));               // Кількисть секунд до спрацьовування таймера
    else
        jsonWrite(MqttSnd, "tmr", (String)0);                           // Кількисть секунд до спрацьовування таймера == 0
    #ifdef MP3_PLAYER_USE
     jsonWrite(MqttSnd, "volume", (String)eff_volume);                  // Гучність
     jsonWrite(MqttSnd, "sound", eff_sound_on ? "SO_ON" : "SO_OFF");    // Увімкнути / вимкнути озвучування ефектів
    #endif //MP3_PLAYER_USE
    MqttSnd.toCharArray(MqttManager::mqttBuffer, MqttSnd.length() +1);  // можливо додати ще якісь змінні (данні) для виводу у відповіді, але довжина радку відповіді повина бути меньша ниж 255 байт
    MqttManager::publishState(1);  //публікація буфера MQTT відповіді (JSON): "{"power":"P_ON","cycle":"FAV_OFF","effect":"111","bri":"15","spd":"33","sca":"58","runt":"10","runc":"123","runf":"1","runc":"220","rnde":"0","rndc":"1","rndf":"0","tmr":59900","volume":"10","sound":"SO_ON"}" в топик <TopicBase>/LedLamp_<ChipId>/snd
  }
  #endif

  #ifdef USE_BLYNK
  if (espMode == 1U && WiFi.isConnected())
    Blynk.run();
  #endif

  #if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  handleTelnetClient();
  #endif
 } //if (Painting == 0)
  yield();
    #ifdef ESP32_USED
     esp_task_wdt_reset();
    #else
     ESP.wdtFeed();
    #endif
  #ifdef MAIN_CYCLES_PER_SECOND
    mcps_counter ++;
    if ((millis() - my_timer2) > 1000)
    {
        my_timer2 = millis();
        LOG.print("MAIN CYCLES PER SECOND = ");
        LOG.println(mcps_counter);
        mcps_counter = 0;
    }
  #endif
} while (connect);
}
