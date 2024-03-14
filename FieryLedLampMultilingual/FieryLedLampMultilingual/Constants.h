// Поточна версія / Current version : v3.90_sound_Multilingual_IR 112 ефектов
// УВАГА!!! Більшість установок перенесено у файл data/config і може змінюватися в процесі експлуатації лампи.
// Уважно читайте файл ПРОЧИТИ МЕНЕ!!!.txt і ПРОЧТИ МЕНЕ.doc (тут з картинками)

#pragma once

#define FLL_VERSION           (" Ver.3.90.112")
#define FLL_MARK              (390)


// ==========  ЗОВНІШНЄ УПРАВЛІННЯ  ===============
#define USE_MQTT                    (true)                  // true - використовується mqtt клієнт, false - ні
#if USE_MQTT
#define MQTT_RECONNECT_TIME   (10U)                         // час у секундах перед підключенням до MQTT брокеру у разі втрати підключення
#endif
//#define USE_BLYNK  ("сюди_вставититокен_із_програми")     // розкоментуйте цей рядок, якщо ви використовуєте програму Blynk (для iOS та Android) https://community.alexgyver.ru/goto/post?id=53535 . Токен беруть у додатку в "Параметри -> DEVICES -> MY DEVICES -> AUTH TOKEN"

// =======  МОДУЛІ, ЩО ВИКОРИСТОВУЮТЬСЯ  ==========
#define ESP_USE_BUTTON                                      // Закоментувати, якщо не використовується кнопка (інакше лампа може реєструвати "фантомні" натискання та некоректно встановлювати яскравість)
#define TM1637_USE                                          // Закоментувати, якщо не використовується дисплей TM1637
#define MP3_PLAYER_USE                                      // Закоментувати рядок у разі відсутність плеєра !!!
#define IR_RECEIVER_USE                                     // Закоментувати цей рядок, якщо не використовується ІЧ ДУ

// =============  НАЛАШТУВАННЯ  ====================
#define USE_DEFAULT_SETTINGS_RESET                          // закоментуйте цей рядок, якщо не потрібно, щоб при завантаженні списку ефектів з лампи налаштування ефектів скидалися на значення за замовчуванням

#define USE_MULTIPLE_LAMPS_CONTROL                          // Додати код для керування кількома лампами
#define GEOLOCATION                                         // IP геолокація

//#define MP3_DEBUG                                         // якщо рядок не закоментований, виводитимуться налагоджувальні повідомлення mp3 player
//#define HEAP_SIZE_PRINT                                   // якщо рядок не закоментований, буде виводитись розмір "купи" (вільного ОЗУ)
//#define GENERAL_DEBUG                                     // якщо рядок не закоментований, будуть виводитися загальні налагоджувальні повідомлення
#ifdef GENERAL_DEBUG
 //#define MAIN_CYCLES_PER_SECOND                           // Виводить кількість головних циклів за секунду. Приблизно те саме, що и FPS. Буде виводитись тільки разом з GENERAL_DEBUG
#endif

#define USE_LittleFS                                        // Закоментуйте цей рядок, якщо замість файлової системи LittlFS хочете використати файлову систему SPIFFS (застаріла)

// --- НАЛАШТУВАННЯ МР3 ПЛЕЄРА -------------
#ifdef MP3_PLAYER_USE
  //#define CHECK_MP3_CONNECTION                            // Закоментуйте цей рядок, якщо потрібно, щоб лампа не перевіряла наявність зв'язку з МР3 плеєром.
  //#define DF_PLAYER_IS_ORIGINAL                           // Якщо використовується плеєр з чіпом, відмінним від AS20HGN402, закоментуйте цей рядок
  #ifndef  DF_PLAYER_IS_ORIGINAL
    //#define DF_PLAYER_GD3200x                               // Якщо у вас такій чіп у плеєрі и цей рядок не закоментований, то для озвучування часу використовуються декілька мов
  #endif  //DF_PLAYER_IS_ORIGINAL
#endif  //MP3_PLAYER_USE

// --- НАЛАШТУВАННЯ КНОПКИ -----------------
#define BUTTON_CAN_SET_SLEEP_TIMER                          // с этой строчкой, если на лампе набить кнопкой 4х кратно (один раз), лампа мигнёт дважды синим и если она выключена , то включится.
                                                            // Таймер выключения лампы будет установлен на BUTTON_SET_SLEEP_TIMER1 минут. Если на лампе набить кнопкой 3х кратно + удержание 4го нажатия, лампа мигнёт синим трижды и если она выключена , то включится.
                                                            // Таймер выключения лампы будет установлен на BUTTON_SET_SLEEP_TIMER2 минут. Чтобы убрать эту функцию, удалите или закомментируйте эту строчку
#ifdef BUTTON_CAN_SET_SLEEP_TIMER
  #define BUTTON_SET_SLEEP_TIMER1   (5UL)                   
  #define BUTTON_SET_SLEEP_TIMER2   (10UL)
#endif
#define BUTTON_IS_SENSORY     (1)                           // Якщо у вас не сенсорна, а проста кнопка, поміняйте в цьому рядку 1 на 0
#define BUTTON_LOCK_ON_START                                // з цим рядком, якщо в момент включення лампи в розетку встигнути натиснути кнопку, або якщо ви забудете кнопку підключити, лампа відключить реакцію натискання кнопки до наступного разу
#ifdef ESP_USE_BUTTON
  //#define DISPLAY_IP_AT_START                             // Розкоментуйте цей рядок, якщо хочете, щоб при включенні живлення та підключенні до WiFi, лампа один раз виводила свою IP адресу (для ламп з кнопкою)
#else
   #define DISPLAY_IP_AT_START                              // Закоментуйте цей рядок, якщо не хочете, щоб при включенні живлення та підключенні до WiFi, лампа один раз виводила свою IP адресу (для ламп без кнопки)
#endif  //ESP_USE_BUTTON
#if defined (ESP_USE_BUTTON)
#define BUTTON_STEP_TIMEOUT   (100U)                        // кожні BUTTON_STEP_TIMEOUT мс буде генеруватися подія утримання кнопки (для регулювання яскравості)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальний час між натисканнями кнопки в мс, до досягнення якого вважається серія послідовних натискань
#if (BUTTON_IS_SENSORY == 1)
  #define BUTTON_SET_DEBOUNCE   (10U)                       // Час антибрязкоту mS для сенсорної кнопки
#else
  #define BUTTON_SET_DEBOUNCE   (60U)                       // Час антибрязкоту mS для механічної кнопки
#endif
#endif

// --- ЧАС -----------------------------
#define GET_TIME_FROM_PHONE (5U)                            // с этой строчкой время в лампе само синхронизируется с приложением, когда лампа не имеет или потеряла доступ в интернет на сервер точного времени .
                                                            // для этого нужно использовать приложение FireLamp версии 3.0 или выше, либо другое приложение, которое отправляет время телефона в лампу. 
                                                            // цифра 5U означает, что синхранизация не чаще, чем раз в 5 минут. переход на зимнее время произойдёт только если изменение пришло со смартфона!
#define PHONE_N_MANUAL_TIME_PRIORITY                        // с этой строчкой, если время получено через приложение, то попытки синхронизации с NTP-сервером прекращаются (пригодится тем, у кого возникают проблемы с NTP-сервером)
#define WARNING_IF_NO_TIME      (7U)                        // с этой строчкой лампа будет подмигивать в нижнем ряде светодиодов, когда она не знает, сколько сейчас времени.
                                                            // 7 - это яркость мигающих точек (максимум - 255U), когда лампа выключена
//#define WARNING_IF_NO_TIME_ON_EFFECTS_TOO                 // а если эту строку раскомментировать, то подмигивание будет даже во время работы эффектов. яркость точек будет, как у работающего эффекта
#define USE_NTP                                             // закомментировать или удалить эту строку, если нужно, чтобы лампа не обращалась в интернет на сервер времени (NTP-сервер).
                                                            // Стоит её убрать только в том случае, если в вашей домашней сети нет круглосуточного доступа в интернет.
                                                            // Лампу можно отправить в другой часовой пояс, так как часовой пояс, выставляется на web странице. 
                                                            // Там же чекбоксом выбирается необходимость перехода на летнее время.
#define NTP_INTERVAL          (59 * 60UL * 1000UL)          // интервал синхронизации времени (59 минут)
                                                            // константы DAY_HOURS_BRIGHTNESS и NIGHT_HOURS_BRIGHTNESS используются только, когда матрица выключена, иначе будет использована яркость текущего эффекта
// --- ЯСКРАВИСТЬ СПАЛАХУ ПОПЕРЕДЖЕННЯ showWarning ---
#define WARNING_BRI_DAY    (10U)                            // Яскравість спалаху попередження вдень
#define WARNING_BRI_NIGHT    (5U)                           // Яскравість спалаху попередження вночі


// --- МАТРИЦЯ -------------------------
#define CURRENT_LIMIT         (4000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH                 (16)                          // ширина матрицы
#define HEIGHT                (16)                          // высота матрицы

#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

// --- РІЗНЕ -----------------------------
#define ESP_CONF_TIMEOUT        (60U)                       // час у секундах, який лампа чекатиме від вас введення пароля для ОТА оновлення (пароль збігається з паролем точки доступу)
#define INTERNET_CHECK_PERIOD   (60UL)                      // Період повторної перевірки наявності інтернету за секунди


// ========  РАСПОДІЛЕННЯ  КОНТАКТІВ  МОДУЛЯ  ==================================================

// --- ESP_PIN_OUT ESP32 ---  РАСПОДІЛЕННЯ  КОНТАКТІВ  МОДУЛЯ ESP32 (ESP8266 дивись нижче) -----
#ifdef ESP32_USED
#define LED_PIN               (32U)                         // пін стрічки
#define BTN_PIN               (35U)                         // пін кнопки
#define MOSFET_PIN            (33U)                         // пін MOSFET транзистора - може бути використаний для керування живленням матриці/стрічки (якщо розкоментований рядок)
#define MOSFET_LEVEL          (HIGH)                        // логічний рівень, в який буде встановлений пін MOSFET_PIN, коли матрицю включено - HIGH або LOW (якщо розкоментувати)
//#define ALARM_PIN             (39U)                       // пін стану будильника - може бути використаний для керування будь-яким зовнішнім пристроєм на час роботи будильника (якщо розкоментувати)
//#define ALARM_LEVEL           (HIGH)                      // логічний рівень, в який буде встановлено пін ALARM_PIN, коли "світанок"/будильник увімкнено (якщо розкоментувати)

#ifdef TM1637_USE
#define DIO                   (13U)                         // TM1637 display DIO pin
#define CLK                   (27U)                         // TM1637 display CLK pin
#endif  //TM1637_USE

#ifdef MP3_PLAYER_USE
 #define MP3_TX_PIN            (25U)                        // Визначає  пін TX (RX на плеєрі)  послідовного порту
 #define MP3_RX_PIN            (26U)                        // Визначає  пін RX (TX на плеєрі)  послідовного порту
 #endif //MP3_PLAYER_USE
 
#ifdef IR_RECEIVER_USE
  #define IR_RECEIVER_PIN     (34U)                         // Пін ИЧ сенсора
#endif  //IR_RECEIVER_USE

#else
// --- ESP_PIN_OUT ESP8266 ---  РАСПОДІЛЕННЯ  КОНТАКТІВ  МОДУЛЯ ESP8266 (ESP32 дивись вище) ---
#define LED_PIN               (0U)                          // пін стрічки                (D3) 
#define BTN_PIN               (4U)                          // пін кнопки               (D2)
#define MOSFET_PIN            (5U)                          // пін MOSFET транзистора (D1) - може бути використаний для керування живленням матриці/стрічки (якщо розкоментований рядок)
#define MOSFET_LEVEL          (HIGH)                        // логічний рівень, в який буде встановлений пін MOSFET_PIN, коли матрицю включено - HIGH або LOW (якщо розкоментувати)
//#define ALARM_PIN             (15U)                       // пін стану будильника - може бути використаний для керування будь-яким зовнішнім пристроєм на час роботи будильника (якщо розкоментувати)
//#define ALARM_LEVEL           (HIGH)                      // логічний рівень, в який буде встановлено пін ALARM_PIN, коли "світанок"/будильник увімкнено (якщо розкоментувати)

#ifdef TM1637_USE
#define DIO                   (16U)                         // D0 TM1637 display DIO pin
#define CLK                   (14U)                         // D5 TM1637 display CLK pin
#endif  //TM1637_USE

#ifdef MP3_PLAYER_USE
 #define MP3_TX_PIN            (12U)                        // Визначає  пін (D6) TX (RX на плеєрі)  послідовного порту
 #define MP3_RX_PIN            (13U)                        // Визначає  пін (D7) RX (TX на плеєрі)  послідовного порту
 #endif //MP3_PLAYER_USE

#ifdef IR_RECEIVER_USE
  #define IR_RECEIVER_PIN     (2U)                          //Пін ИЧ сенсора D4
#endif  //IR_RECEIVER_USE
#endif

// ==================================================================================================
//#define CONNECTION_ANGLE      (0U)                        // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
//#define STRIP_DIRECTION       (0U)                        // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// Не використовується.                                     // шпаргалка по настройке матрицы здесь: https://alexgyver.ru/wp-content/uploads/2018/11/scheme3.jpg
// Це налаштовуєтся у веб інтерфейсі.                       // (но в ней, кажется, перепутаны местами квадратики в 4м столбце, а может, и ещё какие-то)

// =========  ЕФЕКТИ  =======================================================================
// == названия и номера эффектов ниже в списке используются на вкладке effectTricker ==
// == если меняете, меняйте и там, и ещё здесь ниже в РЕЕСТРЕ ДОСТУПНЫХ ЭФФЕКТОВ ==

#define EFF_NEXUS               (  0U)    // Nexus
#define EFF_WATERCOLOR          (  1U)    // Акварель
#define EFF_POOL                (  2U)    // Басейн
#define EFF_WHITE_COLOR         (  3U)    // Біле світло
#define EFF_RAINBOW_VER         (  4U)    // Веселка
#define EFF_RAINBOW             (  5U)    // Веселка 3D
#define EFF_WINE                (  6U)    // Вино
#define EFF_WHIRL               (  7U)    // Вихори полум'я
#define EFF_WHIRL_MULTI         (  8U)    // Вихори різнокольорові
#define EFF_FIRE                (  9U)    // Вогонь
#define EFF_FIRE_2012           ( 10U)    // Вогонь 2012
#define EFF_FIRE_2018           ( 11U)    // Вогонь 2018
#define EFF_FIRE_2020           ( 12U)    // Вогонь 2020
#define EFF_FIRE_2021           ( 13U)    // Вогонь 2021
#define EFF_FIREFLY_TOP         ( 14U)    // Вогонь верховий
#define EFF_FIREFLY             ( 15U)    // Вогонь що літає
#define EFF_WATERFALL           ( 16U)    // Водоспад
#define EFF_WATERFALL_4IN1      ( 17U)    // Водоспад 4 в 1
#define EFF_CLOCK               ( 18U)    // Годинник
#define EFF_STORMY_RAIN         ( 19U)    // Гроза в банці
#define EFF_FOUNTAIN            ( 20U)    // Джерело
#define EFF_SMOKE               ( 21U)    // Дим
#define EFF_SMOKE_COLOR         ( 22U)    // Дим різнокольоровий
#define EFF_SMOKEBALLS          ( 23U)    // Димові шашки
#define EFF_DNA                 ( 24U)    // ДНК
#define EFF_SWIRL               ( 25U)    // Завиток
#define EFF_STARFALL            ( 26U)    // Завірюха
#define EFF_FLOCK               ( 27U)    // Зграя
#define EFF_FLOCK_N_PR          ( 28U)    // Зграя та хижак
#define EFF_ZEBRA               ( 29U)    // Зебра
#define EFF_SNAKES              ( 30U)    // Змійка
#define EFF_COLORS              ( 31U)    // Зміна кольору
#define EFF_LOTUS               ( 32U)    // Квітка лотоса
#define EFF_LLAND               ( 33U)    // Кипіння
#define EFF_RINGS               ( 34U)    // Кодовий замок
#define EFF_COLOR               ( 35U)    // Колір
#define EFF_SAND                ( 36U)    // Кольорові драже
#define EFF_COLOR_FRIZZLES      ( 37U)    // Кольорові кучері
#define EFF_COMET               ( 38U)    // Комета
#define EFF_COMET_COLOR         ( 39U)    // Комета однокольорова
#define EFF_COMET_TWO           ( 40U)    // Комета подвійна
#define EFF_COMET_THREE         ( 41U)    // Комета потрійна
#define EFF_SPARKLES            ( 42U)    // Конфетті
#define EFF_DROPS               ( 43U)    // Краплі на склі
#define EFF_CUBE2D              ( 44U)    // Кубик Рубика
#define EFF_SPHERES             ( 45U)    // Кулі
#define EFF_LAVA                ( 46U)    // Лава
#define EFF_LAVALAMP            ( 47U)    // Лавова лампа
#define EFF_BUTTERFLYS_LAMP     ( 48U)    // Лампа з метеликами
#define EFF_FOREST              ( 49U)    // Ліс
#define EFF_LUMENJER            ( 50U)    // Люменьєр
#define EFF_BBALLS              ( 51U)    // М'ячики
#define EFF_BALLS_BOUNCE        ( 52U)    // М'ячики без кордонів
#define EFF_MAGMA               ( 53U)    // Магма
#define EFF_MATRIX              ( 54U)    // Матриця
#define EFF_TWINKLES            ( 55U)    // Мерехтіння
#define EFF_METABALLS           ( 56U)    // Метаболз
#define EFF_BUTTERFLYS          ( 57U)    // Метелики
#define EFF_WEB_TOOLS           ( 58U)    // Мрія дизайнера
#define EFF_CHRISTMAS_TREE      ( 59U)    // Новорічна ялинка
#define EFF_OCEAN               ( 60U)    // Океан
#define EFF_PAINTS              ( 61U)    // Олійні фарби
#define EFF_COLOR_RAIN          ( 62U)    // Опади
#define EFF_OSCILLATING         ( 63U)    // Осцилятор
#define EFF_RAINBOW_STRIPE      ( 64U)    // Павич
#define EFF_PAINTBALL           ( 65U)    // Пейнтбол
#define EFF_AURORA              ( 66U)    // Північне сяйво
#define EFF_PICASSO             ( 67U)    // Пікассо
#define EFF_HOURGLASS           ( 68U)    // Пісочний годинник
#define EFF_PLASMA              ( 69U)    // Плазма
#define EFF_SPIDER              ( 70U)    // Плазмова лампа
#define EFF_BY_EFFECT           ( 71U)    // Побічний ефект
#define EFF_FLAME               ( 72U)    // Полум'я
#define EFF_POPCORN             ( 73U)    // Попкорн
#define EFF_PRISMATA            ( 74U)    // Призмата
#define EFF_ATTRACT             ( 75U)    // Притягнення
#define EFF_PULSE               ( 76U)    // Пульс
#define EFF_PULSE_WHITE         ( 77U)    // Пульс білий
#define EFF_PULSE_RAINBOW       ( 78U)    // Пульс райдужний
#define EFF_SNAKE               ( 79U)    // Райдужний змій
#define EFF_LIQUIDLAMP          ( 80U)    // Рідка лампа
#define EFF_LIQUIDLAMP_AUTO     ( 81U)    // Рідка лампа авто
#define EFF_RAIN                ( 82U)    // Різнобарвний дощ
#define EFF_RIVERS              ( 83U)    // Річки Ботсвани
#define EFF_TORNADO             ( 84U)    // Торнадо
#define EFF_LIGHTERS            ( 85U)    // Світлячки
#define EFF_LIGHTER_TRACES      ( 86U)    // Світлячки зі шлейфом
#define EFF_FEATHER_CANDLE      ( 87U)    // Свічка
#define EFF_SINUSOID3           ( 88U)    // Синусоїд
#define EFF_SNOW                ( 89U)    // Снігопад
#define EFF_SPECTRUM            ( 90U)    // Спектрум
#define EFF_SPIRO               ( 91U)    // Спірали
#define EFF_LEAPERS             ( 92U)    // Стрибуни
#define EFF_STROBE              ( 93U)    // Строб.Хаос.Дифузія
#define EFF_PACIFIC             ( 94U)    // Тихий океан
#define EFF_SHADOWS             ( 95U)    // Тіні
#define EFF_UKRAINE             ( 96U)    // Україна
#define EFF_FIREWORK            ( 97U)    // Феєрверк
#define EFF_FAIRY               ( 98U)    // Фея
#define EFF_WAVES               ( 99U)    // Хвилі
#define EFF_CLOUDS              (100U)    // Хмари
#define EFF_SIMPLE_RAIN         (101U)    // Хмарка в банці
#define EFF_MADNESS             (102U)    // Шаленство
#define EFF_CONTACTS            (103U)    // Контакти
#define EFF_RADIAL_WAVE         (104U)    // Радіальна хвиля
#define EFF_MOSAIC              (105U)    // Мозайка
#define EFF_FIREWORK_2          (106U)    // Феєрверк 2
#define EFF_OCTOPUS             (107U)    // Восьминіг
#define EFF_DROP_IN_WATER       (108U)    // Краплі  на воді
#define EFF_MAGIC_LANTERN       (109U)    // Чарівний ліхтарик
#define EFF_COLORED_PYTHON      (110U)    // Кольоровий Пітон
#define EFF_PLASMA_WAVES        (111U)    // Плазмові хвілі

#define MODE_AMOUNT           (112U)          // количество режимов


// ============= МАССИВ НАСТРОЕК ЭФФЕКТОВ ПО УМОЛЧАНИЮ ===================
// формат записи:
//{ Яркость, Скорость, Масштаб}

static const uint8_t defaultSettings[][3] PROGMEM = {
  {  19,  60,  20}, // Nexus
  {  25, 240,  65}, // Акварель
  {   8, 222,  63}, // Басейн
  {   9, 207,  26}, // Біле світло
  {  10, 196,  56}, // Веселка
  {  11,  13,  60}, // Веселка 3D
  {  50, 230,  63}, // Вино
  {   9, 240,   1}, // Вихори полум'я
  {   9, 240,  86}, // Вихори різнокольорові
  {  22, 230,   1}, // Вогонь
  {  10, 220,  63}, // Вогонь 2012
  {  57, 220,  15}, // Вогонь 2018
  {  40, 225,  11}, // Вогонь 2020
  {  15, 150,  22}, // Вогонь 2021
  {  26, 190,  15}, // Вогонь верховий
  {  26, 206,  15}, // Вогонь що літає
  {   5, 212,  54}, // Водоспад
  {   7, 197,  22}, // Водоспад 4 в 1
  {  20,   5, 100}, // Годинник
  {  20, 236,  25}, // Гроза в банці
  {  15, 233,  77}, // Джерело
  {   9, 157, 100}, // Дим
  {   9, 157,  30}, // Дим різнокольоровий
  {  12,  44,  17}, // Димові шашки
  {  15,  77,  95}, // ДНК
  {  30, 230,  50}, // Завиток
  {  20, 199,  54}, // Завірюха
  {  15, 136,   4}, // Зграя
  {  15, 128,  80}, // Зграя та хижак
  {   7,   8,  21}, // Зебра
  {   9,  96,  31}, // Змійка
  {  10, 252,  32}, // Зміна кольору
  {  20, 210,  33}, // Квітка лотоса
  {   7, 240,  18}, // Кипіння
  {  10, 220,  91}, // Кодовий замок
  {   9, 200,  50}, // Колір
  {   9, 195,  80}, // Кольорові драже
  {  20, 128,  25}, // Кольорові кучері
  {  16, 220,  28}, // Комета
  {  14, 212,  69}, // Комета однокольорова
  {  27, 186,  19}, // Комета подвійна
  {  24, 186,   9}, // Комета потрійна
  {  16, 142,  63}, // Конфетті
  {  23,  71,  59}, // Краплі на склі
  {  10, 222,  92}, // Кубик Рубика
  {   9,  85,  85}, // Кулі
  {   8,   9,  24}, // Лава
  {  23, 203,   1}, // Лавова лампа
  {   7,  61, 100}, // Лампа з метеликами
  {   7,   8,  95}, // Ліс
  {  14, 235,  40}, // Люменьєр
  {  24, 255,  26}, // М'ячики
  {  18,  11,  70}, // М'ячики без кордонів
  {   9, 198,  20}, // Магма
  {  27, 186,  23}, // Матриця
  {  25, 236,   4}, // Мерехтіння
  {   7,  85,   3}, // Метаболз
  {  11,  53,  87}, // Метелики
  {  15, 128,  50}, // Мрія дизайнера
  {  50,  90,  50}, // Новорічна ялинка
  {   7,   6,  12}, // Океан
  {  25, 195,  50}, // Олійні фарби
  {  15, 198,  99}, // Опади
  {   8, 208, 100}, // Осцилятор
  {  11,   5,  12}, // Павич
  {  11, 236,   7}, // Пейнтбол
  {  12,  73,  38}, // Північне сяйво
  {   9, 212,  27}, // Пікассо
  {  30, 250, 100}, // Пісочний годинник
  {  11,  19,  59}, // Плазма
  {   8,  59,  18}, // Плазмова лампа
  {  45, 150,  30}, // Побічний ефект
  {  30,  53,   3}, // Полум'я
  {  19,  32,  16}, // Попкорн
  {  17, 100,   2}, // Призмата
  {  21, 203,  65}, // Притягнення
  {  12, 185,   6}, // Пульс
  {   9, 179,  11}, // Пульс білий
  {  11, 185,  31}, // Пульс райдужний
  {  12, 178, 100}, // Райдужний змій
  {  11,  63,   1}, // Рідка лампа
  {  11, 124,  39}, // Рідка лампа авто
  {  15, 225,   1}, // Різнобарвний дощ
  {  15,  50,  50}, // Річки Ботсвани
  {  15, 127,  50}, // Торнадо
  {  15, 157,  23}, // Світлячки
  {  21, 198,  93}, // Світлячки зі шлейфом
  {  50, 220,   5}, // Свічка
  {   7, 175,  30}, // Синусоїд
  {   9, 180,  90}, // Снігопад
  {  11, 255,   1}, // Спектрум
  {   9,  46,   3}, // Спірали
  {  24, 203,   5}, // Стрибуни
  {  25,  70,  51}, // Строб.Хаос.Дифузія
  {  55, 127, 100}, // Тихий океан
  {  39,  77,   1}, // Тіні
  {  15, 240,  50}, // Україна
  {  80,  50,   0}, // Феєрверк
  {  19, 212,  44}, // Фея
  {   9, 236,  80}, // Хвилі
  {   8,   4,  34}, // Хмари
  {  30, 233,   2}, // Хмарка в банці
  {  11,  33,  58}, // Шаленство
  {  10, 200,  60}, // Контакти
  {  10, 205,  50}, // Радіальна хвиля
  {  10, 220,  50}, // Мозайка
  {  40, 240,  75}, // Феєрверк 2
  {  15, 230,  51}, // Восьминіг
  {  15, 200,  55}, // Краплі  на воді
  {  20, 200,  96}, // Чарівний ліхтарик
  {  15, 127,  92}, // Кольоровий Пітон
  {  15, 150,  50} // Плазмові хвілі
}; // ^-- проверьте, чтобы у предыдущей строки не было запятой после скобки


// ------------   ОШИБАТЬСЯ НЕЛЬЗЯ!!!   ------------------------------------------------

// ============= КОНЕЦ МАССИВА =====

// === ОСТАЛЬНОЕ ДЛЯ РАЗРАБОТЧИКОВ =====

// --- МАТРИЦА -------------------------
// если у вас не обычная матрица, а сборка из ленты или гирлянда, и возникают непонятной природы артефакты в рисунке эффекта, попробуйте менять следующие три строчки
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#ifdef ESP32_USED
 #define FASTLED_ESP32_RAW_PIN_ORDER
#else
 #define FASTLED_ESP8266_RAW_PIN_ORDER                      // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER
#endif

#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты). вряд ли эффекты будут корректно работать, если изменить этот параметр

#define DYNAMIC                (0U)   // динамическая задержка для кадров ( будет использоваться бегунок Скорость )
#define SOFT_DELAY             (1U)   // задержка для смены кадров FPSdelay задается програмно прямо в теле эффекта
#define LOW_DELAY             (15U)   // низкая фиксированная задержка для смены кадров
#define HIGH_DELAY            (50U)   // высокая фиксированная задержка для смены кадров
#define DYNAMIC_DELAY_TICK    if (millis() - effTimer >= (256U - modes[currentMode].Speed))
#define HIGH_DELAY_TICK       if (millis() - effTimer >= 50)
#define LOW_DELAY_TICK        if (millis() - effTimer >= 15)
#define MICRO_DELAY_TICK      if (millis() - effTimer >= 2)
#define SOFT_DELAY_TICK       if (millis() - effTimer >= FPSdelay)

#define BRIGHTNESS            (40U)                         // стандартная маскимальная яркость (0-255). используется только в момент включения питания лампы

#define ESP_HTTP_PORT         (80U)                         // номер порта, который будет использоваться во время первой утановки имени WiFi сети (и пароля), к которой потом будет подключаться лампа в режиме WiFi клиента (лучше не менять)
#define ESP_UDP_PORT          (8888U)                       // номер порта, который будет "слушать" UDP сервер во время работы лампы как в режиме WiFi точки доступа, так и в режиме WiFi клиента (лучше не менять)
#define WIFIMAN_DEBUG         (false)                       // вывод отладочных сообщений при подключении к WiFi сети: true - выводятся, false - не выводятся; настройка не зависит от GENERAL_DEBUG
#define OTA                                                 // если строка не закомментирована, модуль будет ждать два последовательных запроса пользователя на прошивку по воздуху (два четрёхкратных нажатия на кнопку)
#ifdef OTA
#define ESP_OTA_PORT          (8266U)                       // номер порта, который будет "прослушиваться" в ожидании команды прошивки по воздуху
#endif

#define RANDOM_SETTINGS_IN_CYCLE_MODE                       // Строка добавлена для совместимости файлов эффектов с версией  86 в 1 (или уже больше) от Сотнег.                                                             
                                                            // этот режим можно включать/выключать на web странице лампы или "секретной" командой. Не удаляйте и не комментируйте эту строку.
#define USE_RANDOM_SETS_IN_APP                              // Строка добавлена для совместимости файлов эффектов с версией  86 в 1 (или уже больше) от Сотнег.                               
                                                            // Этот режим уже поддерживается приложением от Котейка под Андроид. Не удаляйте и не комментируйте эту строку.
#define SUMMER_HOUR           (3U)                          // час (по зимнему времени!), когда заканчивается зимнее время и начинается летнее; [0..23]
#define SUMMER_WEEK_NUM       (week_t::Last)                // номер недели в месяце, когда происходит переход на летнее время (возможные варианты: First - первая, Second - вторая, Third - третья, Fourth - четвёртая, Last - последняя)
#define SUMMER_WEEKDAY        (dow_t::Sun)                  // день недели, когда происходит переход на летнее время (возможные варианты: Mon - пн, Tue - вт, Wed - ср, Thu - чт, Sat - сб, Sun - вс)
#define SUMMER_MONTH          (month_t::Mar)                // месяц, в котором происходит переход на летнее время (возможные варианты: Jan - январь, Feb - февраль, Mar - март, Apr - апрель, May - май, Jun - июнь, Jul - июль, Aug - август, Sep - сентябрь, Oct - октябрь, Nov - ноябрь, Dec - декабрь)
#define SUMMER_TIMEZONE_NAME  ("EEST")                      // обозначение летнего времени; до 5 символов; может быть использовано, если понадобится его вывести после вывода времени; может быть "ЛЕТ"
#define WINTER_HOUR           (4U)                          // час (по летнему времени!), когда заканчивается летнее время и начинается зимнее; [0..23]
#define WINTER_WEEK_NUM       (week_t::Last)                // номер недели в месяце, когда происходит переход на зимнее время (возможные варианты: First - первая, Second - вторая, Third - третья, Fourth - четвёртая, Last - последняя)
#define WINTER_WEEKDAY        (dow_t::Sun)                  // день недели, когда происходит переход на зимнее время (возможные варианты: Mon - пн, Tue - вт, Wed - ср, Thu - чт, Sat - сб, Sun - вс)
#define WINTER_MONTH          (month_t::Oct)                // месяц, в котором происходит переход на зимнее время (возможные варианты: Jan - январь, Feb - февраль, Mar - март, Apr - апрель, May - май, Jun - июнь, Jul - июль, Aug - август, Sep - сентябрь, Oct - октябрь, Nov - ноябрь, Dec - декабрь)
#define WINTER_TIMEZONE_NAME  ("EET")                       // обозначение зимнего времени; до 5 символов; может быть использовано, если понадобится его вывести после вывода времени; может быть "ЗИМ"


// --- РАССВЕТ -------------------------
uint8_t DAWN_BRIGHT ;                                       // максимальная яркость рассвета (0-255)
uint8_t DAWN_TIMEOUT;                                       // сколько рассвет светит после времени будильника, минут. Может быть изменено в установках будильника


//#define MAX_UDP_BUFFER_SIZE (UDP_TX_PACKET_MAX_SIZE + 1)
// максимальный размер буффера UDP сервера 
// 255 - это максимальное значение, при котором работа с Избранным не будет глючить
// для исходящих сообщений в приложение данное ограничение можно обойти (см. как реализована отправка "LIST"),
// а для входящего списка избранного - хз. пришлось увеличить до максимально возможножного значения. 
// дальше придётся переделывать типы (размеры) переменных в функциях FavoritesManager.h
#define MAX_UDP_BUFFER_SIZE   (MODE_AMOUNT * 2 + 20)        // максимальный размер буффера UDP сервера

#define GENERAL_DEBUG_TELNET  (false)                       // true - отладочные сообщения будут выводиться в telnet вместо Serial порта (для удалённой отладки без подключения usb кабелем)
#define TELNET_PORT           (23U)                         // номер telnet порта

#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
WiFiClient telnet;                                          // обработчик событий telnet клиента
bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"
#define LOG                   telnet
#else
#define LOG                   Serial
#endif

//================ Дальше только для разработчиков. Не меняйте здесь ничего, если не уверены в результате!!! ===================

//#include <EEPROM.h>
//#include "Types.h"
#define EEPROM_PASSWORD_START_ADDRESS           (0U)            // начальный адрес в EEPROM памяти для записи пароля к роутеру
#define EEPROM_MODES_START_ADDRESS              (0x50U)         // начальный адрес в EEPROM памяти для записи настроек эффектов (яркость, скорость, масштаб)
#define EEPROM_FIRST_RUN_ADDRESS                (0x40U)         // (0x40U) адрес в EEPROM памяти для записи признака первого запуска (определяет необходимость первоначальной записи всех хранимых настроек)
#define EEPROM_MODE_STRUCT_SIZE                 (3U)            // 1 байт - яркость; 1 байт - скорость; 1 байт - масштаб
#define EEPROM_TOTAL_BYTES_USED                 (EEPROM_MODES_START_ADDRESS+MODE_AMOUNT*EEPROM_MODE_STRUCT_SIZE+1)       
#define EEPROM_FIRST_RUN_MARK                   (FLL_MARK-MODE_AMOUNT-255) // число-метка, если ещё не записно в EEPROM_FIRST_RUN_ADDRESS, значит нужно проинициализировать EEPROM и записать все первоначальные настройки
#define EEPROM_WRITE_DELAY                      (30000UL)       // отсрочка записи в EEPROM после последнего изменения хранимых настроек, позволяет уменьшить количество операций записи в EEPROM
#define SAVE_FILE_DELAY_TIMEOUT                 (15000UL)       // отсрочка записи в файл

//+++++Функции для работы с json файлами+++++++++++++++++++++++++++

#include <ArduinoJson.h>        //Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/


#ifdef ESP32_USED
 #ifndef CORE_2_0_X
  #define LittleFS LITTLEFS  // Core 1.0.x
 #endif
#include <FS.h>
#ifdef USE_LittleFS
 #ifdef CORE_2_0_X
  #include <LittleFS.h>     // Core 2.0.x
 #else
  #include <LITTLEFS.h>     // Core 1.0.x
 #endif
#else
  #define LittleFS SPIFFS
  #include <SPIFFS.h>
#endif
#else
 #ifdef USE_LittleFS
  #include <LittleFS.h>
  #define LittleFS LittleFS
 #endif  
#endif

// --- AP (WiFi точка доступа) ---
String AP_NAME ="";                                         // Переменная для имени точки доступа. Задаётся на web странице
String AP_PASS = "";                                        // Переменная для пароля точки доступа. Задаётся на web странице
String LAMP_NAME = "";                                      // Переменная для имени Лампы. Задаётся на web странице
const uint8_t AP_STATIC_IP[] = {192, 168, 4, 1};            // статический IP точки доступа (лучше не менять!)

static const uint8_t Default_Settings[] PROGMEM = { //Дополнительные настройки по умолчанию
    0xd0, 0xa1, 0xd0, 0xbb, 0xd0, 0xb8, 0xd1, 0x88,
    0xd0, 0xba, 0xd0, 0xbe, 0xd0, 0xbc, 0x20, 0xd0,
    0xbc, 0xd0, 0xbd, 0xd0, 0xbe, 0xd0, 0xb3, 0xd0,
    0xbe, 0x20, 0xd1, 0x8d, 0xd1, 0x84, 0xd1, 0x84,
    0xd0, 0xb5, 0xd0, 0xba, 0xd1, 0x82, 0xd0, 0xbe,
    0xd0, 0xb2, 0x20, 0x2a, 0x20, 0x54, 0x6f, 0x6f,
    0x20, 0x4d, 0x61, 0x6e, 0x79, 0x20, 0x45, 0x66,
    0x66, 0x65, 0x63, 0x74, 0x73, 0x20, 0x00, 0x00
};

uint8_t ORIENTATION;                                        // Оріентація матриці
uint8_t MATRIX_TYPE = 0;                                    // тип матрицы: 0 - зигзаг, 1 - параллельная

uint8_t DONT_TURN_ON_AFTER_SHUTDOWN;                        // Не вмикати після обезструмлювання
uint32_t AUTOMATIC_OFF_TIME = (0UL);                        // Автовимкнення

char NTP_ADDRESS [32];                                      // Массив для адреси сервера NTP

String configSetup = "{}";                                  // Конфіг лампи

// --- ВЫВОД ВРЕМЕНИ БЕГУЩЕЙ СТРОКОЙ ---
unsigned int NIGHT_HOURS_START;                             // Початок нічного часу
unsigned int NIGHT_HOURS_STOP;                              // Завершення нічного часу
unsigned int DAY_HOURS_BRIGHTNESS;                          // Початок денного часу
unsigned int NIGHT_HOURS_BRIGHTNESS;                        // Завершення денного часу

// Разкоментуйте ці чотири функції та закоментуйте наступні чотири функції, якщо використовуєте бібліотеку ArduinoJSON Version 5 (по умолчанию используется Version5)

// ------------- Чтение значения json String
String jsonRead(String &json, String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name].as<String>();
}

// ------------- Чтение значения json int
int jsonReadtoInt(String &json, String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name];
}

// ------------- Запись значения json String
String jsonWrite(String &json, String name, String volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}

// ------------- Запись значения json int
String jsonWrite(String &json, String name, int volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}


// Разкоментуйте ці чотири функції та закоментуйте попередні чотири функції, якщо використовуєте бібліотеку ArduinoJSON Version 6
/*
// StaticJsonDocument<2048> doc;  // DynamicJsonDocument doc(2048);
// ------------- Чтение значения json String
String jsonRead(String &json, String name) {
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, json);
  #ifdef GENERAL_DEBUG
  if (error) {
    LOG.print(F("deserializeJson() failed: "));
    LOG.println(error.f_str());
  }
  #endif
//  deserializeJson(doc, json);
  JsonObject obj = doc.as<JsonObject>();  
  return obj[name].as<String>();
}

// ------------- Чтение значения json int
int jsonReadtoInt(String &json, String name) {
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, json);
  #ifdef GENERAL_DEBUG
  if (error) {
    LOG.print(F("deserializeJson() failed: "));
    LOG.println(error.f_str());
  }
  #endif
//  deserializeJson(doc, json);
  return doc[name];
}

// ------------- Запись значения json String
void jsonWrite(String &json, String name, String volume) {
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, json);
  #ifdef GENERAL_DEBUG
  if (error) {
    LOG.print(F("deserializeJson() failed: "));
    LOG.println(error.f_str());
  }
  #endif
//  deserializeJson(doc, json);
  doc[name] = volume;
  json = "";
  serializeJson(doc, json);
  //return json;
}

// ------------- Запись значения json int
void jsonWrite(String &json, String name, int volume) {
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, json);
  #ifdef GENERAL_DEBUG
  if (error) {
    LOG.print(F("deserializeJson() failed: "));
    LOG.println(error.f_str());
  }
  #endif
//  deserializeJson(doc, json);
  doc[name] = volume;
  json = "";
  serializeJson(doc, json);
  //return json;
}
*/

// ------------- Запись строки в файл
String writeFile(const String& fileName, String& strings ) {
  File configFile = LittleFS.open("/" + fileName, "w");
  if (!configFile) {
    return "Failed to open config file";
  }
  configFile.print(strings);
  //strings.printTo(configFile);
  configFile.close();
  #ifdef GENERAL_DEBUG
  LOG.print (F("\nСохранён файл "));
  LOG.println (fileName);
  #endif  //GENERAL_DEBUG
  return "Write sucsses";
}
void saveConfig (){
  writeFile(F("config.json"), configSetup );
}
// ------------- Чтение файла в строку
String readFile(const String& fileName, size_t len ) {
  File configFile = LittleFS.open("/" + fileName, "r");
  if (!configFile) {
    return "Failed";
  }
  size_t size = configFile.size();
  if (size > len) {
    configFile.close();
    return "Large";
  }
  String temp = configFile.readString();
  configFile.close();
  return temp;
}


#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
void setModeSettings(uint8_t Scale = 0U, uint8_t Speed = 0U);
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)

#ifdef USE_MULTIPLE_LAMPS_CONTROL
void multiple_lamp_control ();
bool repeat_multiple_lamp_control = false;
#endif  //USE_MULTIPLE_LAMPS_CONTROL

uint8_t eff_num_correct [MODE_AMOUNT]; // Коригування номерів ефектів для різних мов
void Display_Timer (uint8_t argument = 0);
void timeTick();
bool FileCopy (const String& SourceFile , const String& TargetFile); // Копіювання файлів
uint8_t T_flag = 0;                   // Службове
void showWarning(CRGB color, uint32_t duration, uint16_t blinkHalfPeriod);

#ifdef ESP32_USED
 uint32_t get_Chip_ID(void){
 uint32_t chipId = 0;
   for(int i=0; i<17; i=i+8) {
     chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
   }
   return chipId;
 }
#endif