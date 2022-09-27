# FieryLedLamp_Multilingual

Led lamp with web interface and multi language

# WARNING!!! Look carefully at the connection diagram.

Extract the contents of the archive to the root folder on the disk (not to the desktop, please).
The archive contains the file "Read me!!.doc. You need to read it carefully. To upload files from the data folder to the file system of the controller, you need to install Uploader. Video https://esp8266-arduinoide.ru/esp8266fs/
Select the version of the board in the "Board Manager" 2.7.4. At the first startup, the lamp will create its own WiFi network with the name LedLamp, the password of this network at the first startup will be 31415926. After connecting to the LedLamp network, type 192.168.4.1 in the browser and go to the web page of the lamp. There you can change the name of the lamp (if there are several of them in the network), configure the connection to your home WiFi network, the time zone and switch to summer time. You can also change the access point password (recommended) and access point name. Reboot the lamp.
All firmware settings are located on the Constants.h tab (there in Russian, you will understand without problems) and in the data/config.json file (you can not change anything there, everything is changed later from the web page of the lamp). Other settings can be made from the lamp page.

For those who have already assembled the lamp and it is not possible (or difficult) to get to the connector for flashing, you can flash the OTA. For this, it is necessary to use WEB_Updater. The folder contains the Readme.txt file. Read it carefully

In this firmware, the operating mode is ESP_MODE 1 (with a router) or ESP_MODE 0 (without (access point)).
At any moment it can be changed, or it will change itself.

# Увага!!! Уважно дивіться схему підлючення.

Розпакуйте вміст архіву в кореневу папку на диску (не на робочий стіл, будь ласка).
В архіві є файл "Прочитай мене!!.doc. Його потрібно уважно прочитати. Для завантаження файлів з папки data в файлову систему контролера потрібно встановити Uploader. Відео https://esp8266-arduinoide.ru/esp8266fs/
Версію плати у "Менеджері плат" вибирайте 2.7.4. При першому запуску лампа створить свою WiFi мережу з ім'ям LedLamp пароль цієї мережі при першому запуску буде 31415926. Після підключення до мережі LedLamp наберіть у браузері 192.168.4.1 і зайдіть на web сторінку лампи. Там можна змінити ім'я лампи (якщо їх кілька у мережі), налаштувати підключення до Вашої домашньої WiFi мережі часовий пояс та перехід на літній час. Також можна змінити пароль точки доступу (рекомендовано) та ім'я точки доступу. Перезавантажити лампу.
Всі налаштування прошивки знаходяться на вкладці Constants.h (там російською або українською, без проблем розберетеся) і у файлі data/config.json (там можна нічого не міняти, все змінюється потім з веб-сторінки лампи). Інші налаштування можна зробити зі сторінки лампи.

Тим, у кого лампа вже зібрана і немає можливості (або складно) дістатись роз'єму для перепрошивки, можна прошитися по ОТА. Для цього необхідно використовувати WEB_Updater. Папка містить файл Readme.txt. Уважно його прочитайте

У цій прошивці режим роботи ESP_MODE 1 (з роутером) або ESP_MODE 0 (без (точка доступу)).
Будь-якої миті його можна буде поміняти, або він сам зміниться.

Дякую @Stepko за допомогу у перекладі текстових файлів українською мовою.
