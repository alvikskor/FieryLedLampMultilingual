// Поточна версія / Current version : v3.3_sound_Multilingual_IR 107 ефектов
// УВАГА!!! Більшість установок перенесено у файл data/config і може змінюватися в процесі експлуатації лампи.
// Уважно читайте файл ПРОЧИТИ МЕНЕ!!!.txt і ПРОЧТИ МЕНЕ.doc (тут з картинками)

#pragma once

#define FLL_VERSION           (" Ver.3.3.107")

//#include <ESP8266WebServer.h>

// ============= НАСТРОЙКИ =============
//#define USE_SECRET_COMMANDS                               // удалите эту строку, если вам не нужна возможность смены режимов работы ESP_MODE и обнуления настроек из приложения
                                                            // список секретных команд тут: https://community.alexgyver.ru/goto/post?id=55780
//#define USE_BLYNK  ("сюда_вставить_токен_из_приложения")  // раскомментируйте эту строку, если вы используете приложение Blynk (для iOS и Android) https://community.alexgyver.ru/goto/post?id=53535
                                                            // токен берут в приложении в "настройки -> DEVICES -> MY DEVICES -> AUTH TOKEN"
//#define USE_SHUFFLE_FAVORITES                             // раскомментируйте эту строку, если вам нужно, чтобы режим Цикл показал каждый эффект по 1 разу перед перемешиванием (иначе просто случайный эффект),
                                                            // а также если у вас выбрано меньше десятка эффектов. кстати, если выбрано менее 2 эффектов, то демонстрироваться будут все эффекты по порядку без перемешивания
uint8_t DONT_TURN_ON_AFTER_SHUTDOWN;                        // Не удаляйте и не комментируйте эту строку
uint32_t AUTOMATIC_OFF_TIME = (0UL);                        // Не удаляйте и не комментируйте эту строку
#define USE_DEFAULT_SETTINGS_RESET                          // закомментируйте или удалите эту строку, если не нужно, чтобы при загрузке списка эффектов из лампы в приложение настройки эффектов сбрасывались на значения по умолчанию

// --- КНОПКА --------------------------
#define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе лампа может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
#define BUTTON_IS_SENSORY     (1)                           // если у вас не сенсорная, а обычная кнопка, поменяйте в этой строчке 1 на нолик 
#define BUTTON_LOCK_ON_START                                // с этой строкой, если в момент включения лампы в розетку успеть нажать кнопку, или если вы забудете кнопку подключить, лампа отключит реакцию на нажатие кнопки до следующего раза
#define BUTTON_CAN_SET_SLEEP_TIMER                          // с этой строчкой, если на лампе набить кнопкой 4х кратно (один раз), лампа мигнёт дважды синим и если она выключена , то включится.
                                                            // Таймер выключения лампы будет установлен на BUTTON_SET_SLEEP_TIMER1 минут. Если на лампе набить кнопкой 3х кратно + удержание 4го нажатия, лампа мигнёт синим трижды и если она выключена , то включится.
                                                            // Таймер выключения лампы будет установлен на BUTTON_SET_SLEEP_TIMER2 минут. Чтобы убрать эту функцию, удалите или закомментируйте эту строчку
#ifdef BUTTON_CAN_SET_SLEEP_TIMER
  #define BUTTON_SET_SLEEP_TIMER1   (5UL)                   
  #define BUTTON_SET_SLEEP_TIMER2   (10UL)
#endif
//#define BUTTON_CHANGE_FAVORITES_MODES_ONLY                // Выберите чекбокс на web странице лампы, если хотите, чтобы кнопка переключала режимы только между теми, которые выбраны для режима Цикл (настраивается в приложении)
//Перенесено в файл data/config.json. Имя поля favorit      // иначе переключаться будут все существующие в лампе режимы по порядку (двойным кликом вперёд, тройным назад)
//Чекбокс "Кнопкой - только эффекты, выбранные в Цикле"

// --- ESP -----------------------------
//#define esp_Mode                                          // Если чекбокс на web странице не отмечен - лампа работает как WiFi точка доступа всегда (работа без роутера),
// Перенесено в файл data/config.json. Имя поля ESP_mode    // Если чекбокс на web странице  отмечен - лампа - клиент WiFi (работа через роутер). Подключение к роутеру настраивается на web странице 
//Чекбокс "Использовать подключение к роутеру"              // данный выбор режима влияет только на первую загрузку прошивки. потом он сохраняется в настройках и может изменяться в процессе эксплуатации лампы
                                                            // подробнее можно почитать по ссылке: https://community.alexgyver.ru/goto/post?id=33674
#define ESP_CONF_TIMEOUT      (60U)                         // время в секундах, которое лампа будет ждать от вас введения пароля для ОТА обновления (пароль совпадает с паролем точки доступа)

#define USE_MULTIPLE_LAMPS_CONTROL                          // Включать код для управления несколькими лампами


//#define MP3_DEBUG                                         // якщо рядок не закоментований, виводитимуться налагоджувальні повідомлення mp3 player
//#define HEAP_SIZE_PRINT                                   // якщо рядок не закоментований, буде виводитись розмір "купи" (вільного ОЗУ)
//#define GENERAL_DEBUG                                     // якщо рядок не закоментований, будуть виводитися загальні налагоджувальні повідомлення


#define LED_PIN               (0U)                          // пин ленты                (D3) 
#define BTN_PIN               (4U)                          // пин кнопки               (D2)
#define MOSFET_PIN            (5U)                          // пин MOSFET транзистора   (D1) - может быть использован для управления питанием матрицы/ленты (если раскомментировать строку)
#define MOSFET_LEVEL          (HIGH)                        // логический уровень, в который будет установлен пин MOSFET_PIN, когда матрица включена - HIGH или LOW (если раскомментировать)
//#define ALARM_PIN             (15U)                       // пин состояния будильника (D8) - может быть использован для управления каким-либо внешним устройством на время работы будильника (если раскомментировать)
//#define ALARM_LEVEL           (HIGH)                      // логический уровень, в который будет установлен пин ALARM_PIN, когда "рассвет"/будильник включен (если раскомментировать)

#define USE_LittleFS                                        // Закомментируйте эту строку, если вместо файловой системы LittlFS  хотите использовать файловую систему SPIFFS

#ifdef ESP_USE_BUTTON
  //#define DISPLAY_IP_AT_START                             // Раскоментируйте эту строчку, если хотите, чтобы при включении пмтания и подключению к WiFi, лампа один раз выводила свой IP адрес (для ламп с кнопкой)
#else
   #define DISPLAY_IP_AT_START                              // Закоментируйте эту строчку, если не хотите, чтобы при включении пмтания и подключению к WiFi, лампа один раз выводила свой IP адрес (для ламп без кнопки)
#endif  //ESP_USE_BUTTON

#define TM1637_USE                                          // закоментировать, если не используется дисплей TM1637
#ifdef TM1637_USE
#define DIO                   (16U)                         // D0 TM1637 display DIO pin
#define CLK                   (14U)                         // D5 TM1637 display CLK pin
#endif  //TM1637_USE

#define MP3_TX_PIN            (12U)                         // В СЛУЧАЕ ОТСУТСТВИЯ ПЛЕЕРА ЗАКОМЕНТМРОВАТЬ СТРОКУ!!! Определяет вывод TX (D6)(RX на плеере)  
#define MP3_RX_PIN            (13U)                         // Определяет вывод RX (D7)(TX на плеере) программного последовательного порта
#ifdef MP3_TX_PIN
  #define CHECK_MP3_CONNECTION                              // Закоментируйте эту строку если нужно, чтобы лампа не проверяла наличие связи с МР3 плеером
  //#define DF_PLAYER_IS_ORIGINAL                           // Если используеися плеер с чипом, отличным от AS20HGN402 ,закоментируйте эту строку
#endif  //MP3_TX_PIN

#define IR_RECEIVER_USE                                     // Если не используется ИК ДУ - Закомментировать эту строку
#ifdef IR_RECEIVER_USE
  #define IR_RECEIVER_PIN  2                                // Пин ИК сенсора D4
#endif  //IR_RECEIVER_USE

// --- ESP (WiFi клиент) ---------------
                                                            // SSID и пароль Вашей WiFi-сети задаются на web странице лампы в режиме WiFi точки доступа по IP 192.168.4.1            
                                                            // Там же задаётся время в секундах (таймаут), которое ESP будет пытаться подключиться к WiFi сети, после его истечения автоматически развернёт WiFi точку доступа
#define	INTERNET_CHECK_PERIOD	(60UL)                      // Период повторной проверки наличия интернета в секундах


// --- AP (WiFi точка доступа) ---
String AP_NAME ="";                                         // Переменная для имени точки доступа. Задаётся на web странице
String AP_PASS = "";                                        // Переменная для пароля точки доступа. Задаётся на web странице
String LAMP_NAME = "";                                      // Переменная для имени Лампы. Задаётся на web странице
const uint8_t AP_STATIC_IP[] = {192, 168, 4, 1};            // статический IP точки доступа (лучше не менять!)

// --- ВРЕМЯ ---------------------------
#define GET_TIME_FROM_PHONE (5U)                            // с этой строчкой время в лампе само синхронизируется с приложением, когда лампа не имеет или потеряла доступ в интернет на сервер точного времени .
                                                            // для этого нужно использовать приложение FireLamp версии 3.0 или выше, либо другое приложение, которое отправляет время телефона в лампу. 
                                                            // цифра 5U означает, что синхранизация не чаще, чем раз в 5 минут. переход на зимнее время произойдёт только если изменение пришло со смартфона!
//#define USE_MANUAL_TIME_SETTING                           // с этой строчкой у вас будет возможность устанавливать время на лампе из приложения вручную (например, когда лампа не имеет доступа в интернет)
                                                            // для этого в приложении в поле для текста бегущей строки нужно вписать "time=ЧЧ:ММ Д" в 24-часовом формате
                                                            // например, time=07:25 4  - означает, что время будет установлено на 7 часов 25 минут, четверг
                                                            // время установится в момент нажатия кнопки "НАЗАД" или "ОТПРАВИТЬ", секунды будут по нулям. лампа мигнёт голубым цветом при удачной установке
#define PHONE_N_MANUAL_TIME_PRIORITY                        // с этой строчкой, если время получено через приложение, то попытки синхронизации с NTP-сервером прекращаются (пригодится тем, у кого возникают проблемы с NTP-сервером)
#define WARNING_IF_NO_TIME      (7U)                        // с этой строчкой лампа будет подмигивать в нижнем ряде светодиодов, когда она не знает, сколько сейчас времени.
                                                            // 7 - это яркость мигающих точек (максимум - 255U), когда лампа выключена
//#define WARNING_IF_NO_TIME_ON_EFFECTS_TOO                 // а если эту строку раскомментировать, то подмигивание будет даже во время работы эффектов. яркость точек будет, как у работающего эффекта


#define USE_NTP                                             // закомментировать или удалить эту строку, если нужно, чтобы лампа не обращалась в интернет на сервер времени (NTP-сервер).
                                                            // Стоит её убрать только в том случае, если в вашей домашней сети нет круглосуточного доступа в интернет.
                                                            // Лампу можно отправить в другой часовой пояс, так как часовой пояс, выставляется на web странице. 
                                                            // Там же чекбоксом выбирается необходимость перехода на летнее время.
char NTP_ADDRESS [32];                                      // Не удаляйте и не комментируйте эту строку
															
#define NTP_INTERVAL          (59 * 60UL * 1000UL)          // интервал синхронизации времени (59 минут)
//#define SUMMER_WINTER_TIME                                // Переход на зимнее/летнее время и смещение летнего времени относительно универсального координированного времени UTC  управляется на web странице лампы
// Перенесено в файл data/config.json.

// --- ВЫВОД ВРЕМЕНИ БЕГУЩЕЙ СТРОКОЙ ---
unsigned int NIGHT_HOURS_START;                             // Не удаляйте и не комментируйте эту строку
unsigned int NIGHT_HOURS_STOP;                              // Не удаляйте и не комментируйте эту строку
unsigned int DAY_HOURS_BRIGHTNESS;                          // Не удаляйте и не комментируйте эту строку
unsigned int NIGHT_HOURS_BRIGHTNESS;                        // Не удаляйте и не комментируйте эту строку                        
                                                            // константы DAY_HOURS_BRIGHTNESS и NIGHT_HOURS_BRIGHTNESS используются только, когда матрица выключена, иначе будет использована яркость текущего эффекта
// --- ЯСКРАВИСТЬ СПАЛАХУ ПОПЕРЕДЖЕННЯ showWarning ---
#define WARNING_BRI_DAY    (10U)                            // Яскравість спалаху попередження вдень
#define WARNING_BRI_NIGHT    (5U)                           // Яскравість спалаху попередження вночі


// --- МАТРИЦА -------------------------
#define CURRENT_LIMIT         (4000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH                 (16)                          // ширина матрицы
#define HEIGHT                (16)                          // высота матрицы

#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

uint8_t ORIENTATION;
uint8_t MATRIX_TYPE = 0;                                    // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (0U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (0U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
                                                            // шпаргалка по настройке матрицы здесь: https://alexgyver.ru/wp-content/uploads/2018/11/scheme3.jpg
                                                            // (но в ней, кажется, перепутаны местами квадратики в 4м столбце, а может, и ещё какие-то)

// --- ЭФФЕКТЫ -------------------------
#define RUNNING_TEXT_DEFAULT  ("Привет!")                   // текст, который будет выводиться в эффекте Бегущая строка по умолчанию. его можно менять в приложении
//#define RANDOM_SETTINGS_IN_CYCLE_MODE     (1U)            // с этой строчкой в режиме Цикл эффекты будут включаться на случайных (но удачных) настройках Скорости и Масштаба
//Управляется на web странице лампы                         // настройки подбирались для лампы с матрицей 16х16 со стеклянным плафоном и калькой под ним. на других - не гарантируется
// Перенесено в файл data/config.json.                      // этот режим можно включать/выключать на web странице и секретной командой. чтобы после первой загрузки прошивки в плату он был выключен, поменяйте параметр random_on c 1 на 0 в файле cnfig.json.

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
#define EFF_TEXT                ( 84U)    // Рядок що біжить
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
#define EFF_RADIAL_WAWE         (104U)    // Радіальна хвиля
#define EFF_MOSAIC              (105U)    // Мозайка
#define EFF_FIREWORK_2          (106U)    // Феєрверк 2
#define EFF_OCTOPUS             (107U)    // Восьминіг
#define EFF_DROP_IN_WATER       (108U)    // Краплі  на воді
#define EFF_MAGIC_LANTERN       (109U)    // Чарівний ліхтарик

#define MODE_AMOUNT           (110U)          // количество режимов

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
  {  55, 150,  70}, // Квітка лотоса
  {   7, 240,  18}, // Кипіння
  {  10, 220,  91}, // Кодовий замок
  {   9, 180,  99}, // Колір
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
  {  55, 150,   1}, // Пісочний годинник
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
  {  15, 190,  38}, // Рядок що біжить
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
  {  15, 200,  51}, // Восьминіг
  {  15, 200,  55}, // Краплі  на воді
  {  20, 200,  96} // Чарівний ліхтарик
}; // ^-- проверьте, чтобы у предыдущей строки не было запятой после скобки

// ------------   ОШИБАТЬСЯ НЕЛЬЗЯ!!!   ------------------------------------------------

// ============= КОНЕЦ МАССИВА =====

// === ОСТАЛЬНОЕ ДЛЯ РАЗРАБОТЧИКОВ =====
//#define USE_OLD_IOS_APP                                   // раскомментируйте эту строку, если вы используете приложение для iOS "Arduino Lamp" (удалено из Апстора, исходники на форуме)

// --- МАТРИЦА -------------------------
// если у вас не обычная матрица, а сборка из ленты или гирлянда, и возникают непонятной природы артефакты в рисунке эффекта, попробуйте менять следующие три строчки
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER


#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты). вряд ли эффекты будут корректно работать, если изменить этот параметр

#define DYNAMIC                (0U)   // динамическая задержка для кадров ( будет использоваться бегунок Скорость )
#define SOFT_DELAY             (1U)   // задержка для смены кадров FPSdelay задается програмно прямо в теле эффекта
#define LOW_DELAY             (15U)   // низкая фиксированная задержка для смены кадров
#define HIGH_DELAY            (50U)   // высокая фиксированная задержка для смены кадров
#define DYNAMIC_DELAY_TICK    if (millis() - effTimer >= (256U - modes[currentMode].Speed))
#define HIGH_DELAY_TICK       if (millis() - effTimer >= 50)
#define LOW_DELAY_TICK        if (millis() - effTimer >= 15)
#define SOFT_DELAY_TICK       if (millis() - effTimer >= FPSdelay)

#define BRIGHTNESS            (40U)                         // стандартная маскимальная яркость (0-255). используется только в момент включения питания лампы

#if defined (ESP_USE_BUTTON)
#define BUTTON_STEP_TIMEOUT   (100U)                        // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#if (BUTTON_IS_SENSORY == 1)
  #define BUTTON_SET_DEBOUNCE   (10U)                       // Время антидребезга mS для сенсорной кнопки
#else
  #define BUTTON_SET_DEBOUNCE   (60U)                       // Время антидребезга mS для механической кнопки
#endif
#endif
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

// --- ВНЕШНЕЕ УПРАВЛЕНИЕ --------------
#define USE_MQTT              (false)                       // true - используется mqtt клиент, false - нет
#if USE_MQTT
#define MQTT_RECONNECT_TIME   (10U)                         // время в секундах перед подключением к MQTT брокеру в случае потери подключения
#endif

// --- РАССВЕТ -------------------------
uint8_t DAWN_BRIGHT ;                                       // максимальная яркость рассвета (0-255)
uint8_t DAWN_TIMEOUT;                                       // сколько рассвет светит после времени будильника, минут. Может быть изменено в установках будильника


//#define MAX_UDP_BUFFER_SIZE (UDP_TX_PACKET_MAX_SIZE + 1)
// максимальный размер буффера UDP сервера 
// 255 - это максимальное значение, при котором работа с Избранным не будет глючить
// для исходящих сообщений в приложение данное ограничение можно обойти (см. как реализована отправка "LIST"),
// а для входящего списка избранного - хз. пришлось увеличить до максимально возможножного значения. 
// дальше придётся переделывать типы (размеры) переменных в функциях FavoritesManager.h
#define MAX_UDP_BUFFER_SIZE   (255U)                        // максимальный размер буффера UDP сервера

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
#define EEPROM_FIRST_RUN_MARK                  (128+MODE_AMOUNT)// число-метка, если ещё не записно в EEPROM_FIRST_RUN_ADDRESS, значит нужно проинициализировать EEPROM и записать все первоначальные настройки
#define EEPROM_WRITE_DELAY                      (30000UL)       // отсрочка записи в EEPROM после последнего изменения хранимых настроек, позволяет уменьшить количество операций записи в EEPROM
#define SAVE_FILE_DELAY_TIMEOUT                 (15000UL)       // отсрочка записи в файл

//+++++Функции для работы с json файлами+++++++++++++++++++++++++++

#include <ArduinoJson.h>        //Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/
#ifdef USE_LittleFS
#include <LittleFS.h>    
#define SPIFFS LittleFS  
#endif

static const uint8_t Default_Settings[] PROGMEM = { //Дополнительные настройки по умолчанию
    0x42, 0xD0, 0xB7, 0xD0, 0xBB, 0x6F, 0xD0, 0xBC,
    0x20, 0xD0, 0x9F, 0x4F, 0x20, 0x2A, 0x20, 0x53,
    0xD0, 0xBE, 0x66, 0x74, 0x77, 0xD0, 0xB0, 0x72,
    0xD0, 0xB5, 0x20, 0xD0, 0x9D, 0xD0, 0xB0, 0xD1,
    0x81, 0x6B, 0xD1, 0x96, 0x6E, 0x67, 0x00, 0x00
};

String configSetup = "{}";

// Раскоментируйте эти четыре функции и закоментируйте следующие четыре функции если используете библиотеку ArduinoJSON Version 5 (по умолчанию используется Version5)

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

/*
// Раскоментируйте эти четыре функции и закоментируйте предыдущие четыре функции если используете библиотеку ArduinoJSON Version 6
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
  File configFile = SPIFFS.open("/" + fileName, "w");
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
  File configFile = SPIFFS.open("/" + fileName, "r");
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

uint8_t eff_num_correct [MODE_AMOUNT]; //Корректировка номеров эффектов для разных языков
void Display_Timer (uint8_t argument = 0);
