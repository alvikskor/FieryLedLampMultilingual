void User_setings ()  {
    
 HTTP.on("/favorit", handle_favorit);    // включить \ выключить переход кнопкой только по эффектам из выбранных в режиме Цикл и
 HTTP.on("/random_on", handle_random);  // случайных настроек эффектов в режиме цикл без сохранения в EEPROM
 HTTP.on("/print_time", handle_print_time); //Периодичность вывода времени бегущей строкой
 HTTP.on("/button_on", handle_button_on);  // Вкл\Выкл кнопки лампы (дублирует в приложении, но на виду)
 //HTTP.on("/User_set", handle_User_set); // Сохранение random_on, time_always, button_on и favorit в EEPROM (Файл)
 HTTP.on("/ESP_mode", handle_ESP_mode); // Установка ESP Mode
 HTTP.on("/eff_reset", handle_eff_reset);  //сброс настроек эффектов по умолчанию
 HTTP.on("/run_text", handle_run_text);  // Текст для бегущей строки
 HTTP.on("/night_time", handle_night_time);  // Параметры вывода времени бегущей строкой на ВЫключенной лампе (яркость и время день,ночь) 
 HTTP.on("/effect_always", handle_effect_always);  // Не возобновлять работу эффектов
 HTTP.on("/timer5h", handle_timer5h);  // Автовыключение через 5 часов
 HTTP.on("/ntp", handle_ntp);  // Адрес NTP сервера
 HTTP.on("/eff_sel", handle_eff_sel);  // Выбор эффекта из списка
 HTTP.on("/eff", handle_eff);  // Следующий / Предыдущий
 HTTP.on("/br", handle_br);  // Яркость
 HTTP.on("/sp", handle_sp);  // Скорость
 HTTP.on("/sc", handle_sc);  // Масштаб / Цвет
 HTTP.on("/brm", handle_brm);  // Пошаговая яркость  минус
 HTTP.on("/brp", handle_brp);  // Пошаговая яркость плюс
 HTTP.on("/spm", handle_spm);  // Пошаговая  скорость минус
 HTTP.on("/spp", handle_spp);  // Пошаговая скорость  плюс
 HTTP.on("/scm", handle_scm);  // Пошаговый масштаб  минус
 HTTP.on("/scp", handle_scp);  // Пошаговый мвсштаб  плюс
 HTTP.on("/tm", handle_tm);  // Смена темы страници (0 - светлая / 1 - тёмная)
 HTTP.on("/PassOn", handle_PassOn); // Использовать (1) или нет (0) пароль для доступа к странице Начальных настроек
 HTTP.on("/Power", handle_Power);          // устройство вкл/выкл
 HTTP.on("/summer_time", handle_summer_time);  //Переход на лнтнее время 1 - да , 0 - нет
 HTTP.on("/time_always", handle_time_always);     // Выводить или нет время бегущей строкой(если задано) на не активной лампе
 HTTP.on("/timeZone", handle_time_zone);    // Установка смещения времени относительно GMT.
 HTTP.on("/alarm", handle_alarm);   // Установка будильника "рассвет"
 HTTP.on("/cycle_on", handle_cycle_on);   // Вкл/выкл режима Цикл
 HTTP.on("/time_eff", handle_time_eff);   // Время переключения цикла + Dispersion добавочное случайное время от 0 до disp
 HTTP.on("/rnd_cycle", handle_rnd_cycle);   // Перемешать выбранные или по порядку
 HTTP.on("/cycle_allwase", handle_cycle_allwase);   // Запускать режим цыкл после выкл/вкл лампы или нет
 HTTP.on("/cycle_set", handle_cycle_set);   // Выбор эффектов для цикла
 HTTP.on("/eff_all", handle_eff_all);   // Выбрать все
 HTTP.on("/eff_clr", handle_eff_clr);   // сбросить Выбор
 HTTP.on("/timer", handle_timer);   // Запуск таймера выключения
 HTTP.on("/def", handle_def);   //  Установка настроек эффекта по умолчанию
 HTTP.on("/rnd", handle_rnd);   // Установка случайных настроек эффектов
 HTTP.on("/all_br", handle_all_br);  // Общая яркость
 #ifdef USE_MULTIPLE_LAMPS_CONTROL
 HTTP.on("/multi", handle_multiple_lamp);  // Настройка управления несколькими лампами
 #endif //USE_MULTIPLE_LAMPS_CONTROL
 HTTP.on("/eff_save", handle_eff_save);  // Сохранить настройки эффектов в файл
 HTTP.on("/eff_read", handle_eff_read);  // Загрузить настройки эффектов из файла
 HTTP.on("/alt", handle_alt_panel);   // Альтернативная главная web страница управления эффектами 
 HTTP.on("/get_time", get_time_manual);  // Синхронизация времени лампы с браузером на устройстве (телефоне)
 HTTP.on("/index", handle_index);  // Начальная страница
 HTTP.on("/index2", handle_index2);  //
 #ifdef MP3_TX_PIN
 HTTP.on("/on_sound", handle_on_sound);  // Включить/Выключить звук эффектов
 HTTP.on("/vol", handle_volume);  // Громкость озвучивания эффектов
 HTTP.on("/on_alm_snd", handle_alarm_on_sound);  // Включить/Выключить звук будильника
 HTTP.on("/alm_vol", handle_alarm_volume);  // Громкость озвучивания будильника
 HTTP.on("/alm_fold_sel", handle_alarm_fold_sel);  // Вибір папки для будильника
 HTTP.on("/on_day_adv", handle_day_advert_on_sound);  // Включить/Выключить озвучивание времени днём
 HTTP.on("/on_night_adv", handle_night_advert_on_sound);  // Включить/Выключить озвучивание времени ночью
 HTTP.on("/on_alm_adv", handle_alarm_advert_sound_on);  // Включить/Выключить озвучивание времени будильником
 HTTP.on("/day_vol", handle_day_advert_volume);  // Громкость озвучивания времени днём
 HTTP.on("/night_vol", handle_night_advert_volume);  // Громкость озвучивания времени ночью
 HTTP.on("/sound_set", handle_sound_set);  // Вибір прив'язаних папок для озвучування ефектів
 HTTP.on("/track_down", handle_folder_down);  // Попередня папка
 HTTP.on("/track_up", handle_folder_up);  // Наступна папка
 HTTP.on("/fold_sel", handle_folder_select);  // Вибір папки озвучування на головній сторінці
 HTTP.on("/eq", handle_equalizer);  // Еквалайзер
 #endif
 HTTP.on("/cur_lim", handle_current_limit);  // 
 HTTP.on("/m_t", handle_matrix_tipe);  //
 HTTP.on("/m_o", handle_matrix_orientation);  //
 HTTP.on("/lang", handle_lang);  //

  // --------------------Получаем SSID со страницы
  HTTP.on("/ssid", HTTP_GET, []() {
  jsonWrite(configSetup, "ssid", HTTP.arg("ssid"));
  jsonWrite(configSetup, "password", HTTP.arg("password"));
  jsonWrite(configSetup, "TimeOut", HTTP.arg("TimeOut").toInt()); 
  ESP_CONN_TIMEOUT = jsonReadtoInt(configSetup, "TimeOut");
  saveConfig();                 // Функция сохранения строки конфигурации в файл
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  });
   // --------------------Получаем SSID AP со страницы
  HTTP.on("/ssidap", HTTP_GET, []() {
  jsonWrite(configSetup, "ssidAP", HTTP.arg("ssidAP"));
  jsonWrite(configSetup, "passwordAP", HTTP.arg("passwordAP"));
  saveConfig();                 // Функция сохранения строки конфигурации в файл
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  });

}

void handle_favorit() {    
  jsonWrite(configSetup, "favorit", HTTP.arg("favorit").toInt());
  //saveConfig();  
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
  Favorit_only = jsonReadtoInt(configSetup, "favorit");
  HTTP.send(200, "text/plain", "OK");
 }

void handle_random() { 
  jsonWrite(configSetup, "random_on", HTTP.arg("random_on").toInt());
  //saveConfig();  
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
  random_on = jsonReadtoInt(configSetup, "random_on");
  HTTP.send(200, "text/plain", "OK");
 }
 
void handle_print_time() {    
  jsonWrite(configSetup, "print_time", HTTP.arg("print_time").toInt()); 
  PRINT_TIME = jsonReadtoInt(configSetup, "print_time");
  //saveConfig();
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
  HTTP.send(200, "text/plain", "OK");
 }
 
void handle_button_on() {    
  jsonWrite(configSetup, "button_on", HTTP.arg("button_on").toInt());
  saveConfig();  
  buttonEnabled = jsonReadtoInt(configSetup, "button_on");
  HTTP.send(200, "text/plain", "OK");
 }

/*
 void handle_User_set () {
    handle_random();
	handle_favorit();
 	handle_time_always();
	handle_button_on();
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
 }
*/ 

void handle_ESP_mode() { 
  jsonWrite(configSetup, "ESP_mode", HTTP.arg("ESP_mode").toInt());
  saveConfig();  
  espMode = jsonReadtoInt(configSetup, "ESP_mode");
  HTTP.send(200, "text/plain", "OK");
 }

void handle_eff_reset() {    
    restoreSettings();
    updateSets();
    //loadingFlag = true;  // Перезапуск Эффекта
	jsonWrite(configSetup, "br", modes[currentMode].Brightness);
	jsonWrite(configSetup, "sp", modes[currentMode].Speed);
	jsonWrite(configSetup, "sc", modes[currentMode].Scale);    
    showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
    HTTP.send(200, "text/plain", "OK");
 }

void handle_run_text ()  {
	jsonWrite(configSetup, "run_text", HTTP.arg("run_text"));
	//saveConfig();                 // Функция сохранения данных во Flash
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    (jsonRead(configSetup, "run_text")).toCharArray (TextTicker, (jsonRead(configSetup, "run_text")).length()+1);
    #ifdef GENERAL_DEBUG
    LOG.print("\nTextTicker = ");
    uint8_t i=0;
    while (TextTicker[i]!=0)
    {
        LOG.print (TextTicker[i],HEX);
        LOG.print (' ');
        i++;
    }
    LOG.println();
    #endif
	HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
 }

void handle_night_time ()  {
	jsonWrite(configSetup, "night_time", HTTP.arg("night_time").toInt());
	jsonWrite(configSetup, "night_bright", HTTP.arg("night_bright").toInt());
	jsonWrite(configSetup, "day_time", HTTP.arg("day_time").toInt());
	jsonWrite(configSetup, "day_bright", HTTP.arg("day_bright").toInt());
	saveConfig();
	NIGHT_HOURS_START = 60U * jsonReadtoInt(configSetup, "night_time");
	NIGHT_HOURS_BRIGHTNESS = jsonReadtoInt(configSetup, "night_bright");
	NIGHT_HOURS_STOP = 60U * jsonReadtoInt(configSetup, "day_time");
	DAY_HOURS_BRIGHTNESS = jsonReadtoInt(configSetup, "day_bright");
    #ifdef TM1637_USE
    clockTicker_blink();
    #endif
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
	HTTP.send(200, "text/plain", "OK");
 }

void handle_effect_always ()  {
	jsonWrite(configSetup, "effect_always", HTTP.arg("effect_always").toInt());
	//saveConfig();
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
	DONT_TURN_ON_AFTER_SHUTDOWN = jsonReadtoInt(configSetup, "effect_always");
	HTTP.send(200, "text/plain", "OK");
 }

void handle_timer5h ()  {
	jsonWrite(configSetup, "timer5h", HTTP.arg("timer5h").toInt());
	//saveConfig();
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
	AUTOMATIC_OFF_TIME = (30UL * 60UL * 1000UL) * ( uint32_t )(jsonReadtoInt(configSetup, "timer5h"));
    if (ONflag ) {
      TimerManager::TimerRunning = true;
      if (!AUTOMATIC_OFF_TIME)
          TimerManager::TimeToFire = millis() + AUTOMATIC_OFF_TIME;
      else
          TimerManager::TimeToFire = millis() + (24UL*60UL*60UL*1000UL); //Если 0 то не выключать (установить 24 часа)
    }
	HTTP.send(200, "text/plain", "OK");
 }
 
void handle_ntp ()  {
	jsonWrite(configSetup, "ntp", HTTP.arg("ntp"));
	saveConfig();
	(jsonRead(configSetup, "ntp")).toCharArray (NTP_ADDRESS, (jsonRead(configSetup, "ntp")).length()+1);
	HTTP.send(200, "text/plain", "OK");
	//delay (100);
	//ESP.restart();
}

void handle_eff_sel () {
    uint8_t temp = (HTTP.arg("eff_sel").toInt());
	jsonWrite(configSetup, "eff_sel", temp);
	currentMode = eff_num_correct[temp];
	jsonWrite(configSetup, "br", modes[currentMode].Brightness);
	jsonWrite(configSetup, "sp", modes[currentMode].Speed);
	jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    FastLED.setBrightness(modes[currentMode].Brightness);
    loadingFlag = true;
    settChanged = true;
    eepromTimeout = millis();
      if (random_on && FavoritesManager::FavoritesRunning)
        selectedSettings = 1U;
    #if (USE_MQTT)
    if (espMode == 1U)
    {
      MqttManager::needToPublish = true;
    }
    #endif
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL   
}

void handle_eff () {
    uint8_t temp = jsonReadtoInt (configSetup, "eff_sel");
	if (HTTP.arg("eff").toInt())  {
          if (Favorit_only)
	      {
            uint8_t lastMode = currentMode;
            do 
            {
              if (++temp >= MODE_AMOUNT) temp = 0;
              currentMode = eff_num_correct[temp];
            } while (FavoritesManager::FavoriteModes[currentMode] == 0 && currentMode != lastMode);
            if (currentMode == lastMode) // если ни один режим не добавлен в избранное, всё равно куда-нибудь переключимся
              if (++temp >= MODE_AMOUNT) temp = 0;
              currentMode = eff_num_correct[temp];
	      }
          else
            if (++temp >= MODE_AMOUNT) temp = 0;
      }
      else {
	      if (Favorit_only) 
	      {
            uint8_t lastMode = currentMode;
            do
            {
              if (--temp >= MODE_AMOUNT) temp = MODE_AMOUNT - 1;
              currentMode = eff_num_correct[temp];
            } while (FavoritesManager::FavoriteModes[currentMode] == 0 && currentMode != lastMode);
            if (currentMode == lastMode) // если ни один режим не добавлен в избранное, всё равно куда-нибудь переключимся
              if (--temp >= MODE_AMOUNT) temp = MODE_AMOUNT - 1;
              currentMode = eff_num_correct[temp];
	      }
	      else 
	        if (--temp >= MODE_AMOUNT) temp = MODE_AMOUNT - 1;
      }
    currentMode = eff_num_correct[temp];
	jsonWrite(configSetup, "eff_sel", temp);
	jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    FastLED.setBrightness(modes[currentMode].Brightness);
    loadingFlag = true;
    settChanged = true;
    eepromTimeout = millis();

      if (random_on && FavoritesManager::FavoritesRunning)
        selectedSettings = 1U;

    #if (USE_MQTT)
    if (espMode == 1U)
    {
      MqttManager::needToPublish = true;
    }
    #endif
    #ifdef USE_BLYNK
    updateRemoteBlynkParams();
    #endif
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_br ()  {
	jsonWrite(configSetup, "br", HTTP.arg("br").toInt());
	modes[currentMode].Brightness = jsonReadtoInt(configSetup, "br");
	FastLED.setBrightness(modes[currentMode].Brightness);
    #ifdef GENERAL_DEBUG
    LOG.printf_P(PSTR("Новое значение яркости: %d\n"), modes[currentMode].Brightness);
    #endif
	 HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}"); 
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL    
}

void handle_sp ()  {
	jsonWrite(configSetup, "sp", HTTP.arg("sp").toInt());
	modes[currentMode].Speed = jsonReadtoInt(configSetup, "sp");
	loadingFlag = true;  // Перезапуск Эффекта
    #ifdef GENERAL_DEBUG
    LOG.printf_P(PSTR("Новое значение скорости: %d\n"), modes[currentMode].Speed);
    #endif
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_sc ()  {
	jsonWrite(configSetup, "sc", HTTP.arg("sc").toInt());
	modes[currentMode].Scale = jsonReadtoInt(configSetup, "sc");
	loadingFlag = true;  // Перезапуск Эффекта
    #ifdef GENERAL_DEBUG
    LOG.printf_P(PSTR("Новое значение Масштаба / Цвета: %d\n"), modes[currentMode].Scale);
    #endif
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL       
}

void handle_brm ()   {
	modes[currentMode].Brightness = constrain(modes[currentMode].Brightness - 1, 1, 255);
	jsonWrite(configSetup, "br", modes[currentMode].Brightness);
	FastLED.setBrightness(modes[currentMode].Brightness);
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL    
}

void handle_brp ()   {
	modes[currentMode].Brightness = constrain(modes[currentMode].Brightness + 1, 1, 255);
	jsonWrite(configSetup, "br", modes[currentMode].Brightness);
	FastLED.setBrightness(modes[currentMode].Brightness);
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_spm ()   {
	modes[currentMode].Speed = constrain(modes[currentMode].Speed - 1, 1, 255);
	jsonWrite(configSetup, "sp", modes[currentMode].Speed);
	loadingFlag = true;  // Перезапуск Эффекта
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_spp ()   {
	modes[currentMode].Speed = constrain(modes[currentMode].Speed + 1, 1, 255);
	jsonWrite(configSetup, "sp", modes[currentMode].Speed);
	loadingFlag = true;  // Перезапуск Эффекта
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_scm ()   {
	modes[currentMode].Scale = constrain(modes[currentMode].Scale - 1, 1, 100);
	jsonWrite(configSetup, "sc", modes[currentMode].Scale);
	loadingFlag = true;  // Перезапуск Эффекта
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_scp ()   {
	modes[currentMode].Scale = constrain(modes[currentMode].Scale + 1, 1, 100);
	jsonWrite(configSetup, "sc", modes[currentMode].Scale);
	loadingFlag = true;  // Перезапуск Эффекта
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_tm ()   {
	bool flg = false;
	jsonWrite(configSetup, "tm", HTTP.arg("tm").toInt());
	if (jsonReadtoInt(configSetup, "tm")) flg = FileCopy ("/css/dark/build.css.gz" , "/css/build.css.gz");
	else flg = FileCopy ("/css/light/build.css.gz" , "/css/build.css.gz");
	if (flg) {
       HTTP.send(200, "text/plain", "OK");
	   saveConfig();
    }
	else HTTP.send(404, "text/plain", "File not found");  
}

void handle_PassOn ()   {
//	bool flg = false;
	jsonWrite(configSetup, "PassOn", HTTP.arg("PassOn").toInt());
//	  if (jsonReadtoInt(configSetup, "PassOn")) flg = FileCopy ("/stp/stp_l.json.gz" , "/setup.json.gz");
//	  else flg = FileCopy ("/stp/stp_nl.json.gz" , "/setup.json.gz");
//	  if (flg) {
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
	saveConfig();
//    }
//	  else HTTP.send(404, "text/plain", "File not found");  
}

void handle_Power ()  {
    uint8_t tmp;
    if (dawnFlag) {
      manualOff = true;
      dawnFlag = false;
      #ifdef TM1637_USE
      clockTicker_blink();
      #endif
      FastLED.setBrightness(modes[currentMode].Brightness);
      changePower();
    }
    else {    
    tmp = HTTP.arg("Power").toInt();
    if (tmp == 2) jsonReadtoInt(configSetup, "Power") == 0? tmp = 1 : tmp = 0;
	  jsonWrite(configSetup, "Power", tmp);
  	ONflag = tmp;
	changePower();
    if (ONflag){
        eepromTimeout = millis();
        timeout_save_file_changes = millis();
    }
    else {
        eepromTimeout = millis() - EEPROM_WRITE_DELAY;
        timeout_save_file_changes = millis() - SAVE_FILE_DELAY_TIMEOUT;
    }
    settChanged = true;
    save_file_changes = 7;
    loadingFlag = true;
    }
	 HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}	

void handle_summer_time() {
    #ifdef USE_NTP
	  jsonWrite(configSetup, "Summer_Time", HTTP.arg("Summer_Time").toInt()); 
	  saveConfig();
	  summerTime.offset = winterTime.offset + jsonReadtoInt(configSetup, "Summer_Time") * 60;
	  localTimeZone.setRules (summerTime, winterTime);
    #endif
    jsonWrite(configSetup, "time", (Get_Time(getCurrentLocalTime())));
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
 }
 
void handle_time_always() {
    jsonWrite(configSetup, "time_always", HTTP.arg("time_always").toInt());
    //saveConfig();
    time_always = jsonReadtoInt(configSetup, "time_always");
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "text/plain", "OK");
 }
 
// Установка параметров времянной зоны по запросу вида, например, http://192.168.0.101/timeZone?timeZone=3
void handle_time_zone() {
    #ifdef USE_NTP
    jsonWrite(configSetup, "timezone", HTTP.arg("timeZone").toInt()); // Получаем значение timezone из запроса конвертируем в int сохраняем
    saveConfig();
    winterTime.offset = jsonReadtoInt(configSetup, "timezone") * 60;
    summerTime.offset = winterTime.offset + jsonReadtoInt(configSetup, "Summer_Time") * 60;
    localTimeZone.setRules (summerTime, winterTime);
    #endif
    //HTTP.send(200, "text/plain", "OK");
    jsonWrite(configSetup, "time", (Get_Time(getCurrentLocalTime())));
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_alarm ()  { 
    char i[2];
    String configAlarm = readFile("alarm_config.json", 512); 
	#ifdef GENERAL_DEBUG
	LOG.println (F("\nУстановки будильника"));
   	LOG.println(configAlarm);
	#endif
  	// подготовка  строк с именами полей json file
  	for (uint8_t k=0; k<7; k++) {
   	    itoa ((k+1), i, 10);
    	//i[1] = 0;
      	String a = "a" + String (i) ;
      	String h = "h" + String (i) ;
     	String m = "m" + String (i) ;
      	//сохранение параметров в строку
        if (!first_entry){  
     	    jsonWrite(configAlarm, a, HTTP.arg(a).toInt());
     	    jsonWrite(configAlarm, h, HTTP.arg(h).toInt());
     	    jsonWrite(configAlarm, m, HTTP.arg(m).toInt());
        }
     	//сохранение установок будильника
     	alarms[k].State = (jsonReadtoInt(configAlarm, a));
     	alarms[k].Time = (jsonReadtoInt(configAlarm, h)) * 60 + (jsonReadtoInt(configAlarm, m));
     	EepromManager::SaveAlarmsSettings(&k, alarms);
        ESP.wdtFeed();
        yield();
    }
    if (!first_entry) {
	   jsonWrite(configAlarm, "t", HTTP.arg("t").toInt());
	   jsonWrite(configAlarm, "after", HTTP.arg("after").toInt());
       jsonWrite(configAlarm, "a_br", HTTP.arg("a_br").toInt());
    } 
	dawnMode = jsonReadtoInt(configAlarm, "t")-1;
	DAWN_TIMEOUT = jsonReadtoInt(configAlarm, "after");
    DAWN_BRIGHT = jsonReadtoInt(configAlarm, "a_br");
	EepromManager::SaveDawnMode(&dawnMode);
    if (!first_entry)
        {
         writeFile("alarm_config.json", configAlarm );
        }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void save_alarms()   {
    char k[2];
	bool alarm_change = false;
    String configAlarm = readFile("alarm_config.json", 512); 
	#ifdef GENERAL_DEBUG
     LOG.println (F("\nТекущие установки будильника"));
     LOG.println(configAlarm);
	#endif
    ESP.wdtFeed();
    for (byte i = 0; i < 7; i++) {
	    itoa ((i+1), k, 10);
        k[1] = 0;
        String a = "a" + String (k) ;
        String h = "h" + String (k) ;
        String m = "m" + String (k) ;
	    if (alarms[i].State != (jsonReadtoInt(configAlarm, a)) || alarms[i].Time != (jsonReadtoInt(configAlarm, h)) * 60U + (jsonReadtoInt(configAlarm, m)))
	      {
		    alarm_change = true;
		    jsonWrite(configAlarm, a, alarms[i].State);
		    jsonWrite(configAlarm, h, (alarms[i].Time / 60U));
		    jsonWrite(configAlarm, m, (alarms[i].Time % 60U));
	      }
        yield();
    }
    if (dawnMode != (jsonReadtoInt(configAlarm, "t")-1)) {
	    alarm_change = true;
	    jsonWrite(configAlarm, "t", (dawnMode + 1));
	}
    jsonWrite(configAlarm, "after", DAWN_TIMEOUT);
    jsonWrite(configAlarm, "a_br", DAWN_BRIGHT);  
    if (alarm_change) {
	    writeFile("alarm_config.json", configAlarm );
	    #ifdef GENERAL_DEBUG
        LOG.println (F("\nНовые установки будильника сохранены в файл"));
        LOG.println(configAlarm);
	    #endif
	}
}


void handle_cycle_on()  {  // Вкл/выкл режима Цикл
    uint8_t tmp;
    tmp = HTTP.arg("cycle_on").toInt();
    if (tmp == 2) jsonReadtoInt(configSetup, "cycle_on") == 0? tmp = 1 : tmp = 0;
	jsonWrite(configSetup, "cycle_on", tmp);
    if (ONflag)   {
	    FavoritesManager::FavoritesRunning = tmp;
    }
    else   {
        FavoritesManager::FavoritesRunning = 0;
        jsonWrite(configSetup, "cycle_on", 0);
    }
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");  //HTTP.send(200, "text/plain", "OK");
}

void handle_time_eff ()  {  // Время переключения цикла + Dispersion добавочное случайное время от 0 до disp
	jsonWrite(configSetup, "time_eff", HTTP.arg("time_eff").toInt());
	FavoritesManager::Interval = jsonReadtoInt(configSetup, "time_eff");	
	jsonWrite(configSetup, "disp", HTTP.arg("disp").toInt());
	FavoritesManager::Dispersion = jsonReadtoInt(configSetup, "disp");	
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
	HTTP.send(200, "text/plain", "OK");
}

void handle_rnd_cycle ()  {  // Перемешать выбранные или по порядку
	jsonWrite(configSetup, "rnd_cycle", HTTP.arg("rnd_cycle").toInt());
	FavoritesManager::rndCycle = jsonReadtoInt(configSetup, "rnd_cycle");
	//saveConfig();
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
	HTTP.send(200, "text/plain", "OK");
}

void handle_cycle_allwase ()  {  // Запускать режим цыкл после выкл/вкл лампы или нет
	jsonWrite(configSetup, "cycle_allwase", HTTP.arg("cycle_allwase").toInt());
	FavoritesManager::UseSavedFavoritesRunning = jsonReadtoInt(configSetup, "cycle_allwase");
    if (!ONflag && !FavoritesManager::UseSavedFavoritesRunning)   {
        FavoritesManager::FavoritesRunning = 0;
        jsonWrite(configSetup, "cycle_on", 0);
    }
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
	HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");  //HTTP.send(200, "text/plain", "OK");
}

void handle_eff_all ()   {  //Выбрать все эффекты
    char i[4];
    String configCycle = readFile("cycle_config.json", 2048); 
    // подготовка  строк с именами полей json 
    ESP.wdtFeed();
    for (uint8_t k=0; k<MODE_AMOUNT; k++) {
        itoa ((k), i, 10);
        String e = "e" + String (i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, 1U);
        yield();
    }
    writeFile("cycle_config.json", configCycle );
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff_clr ()   {  //очистить все эффекты
      char i[4];
      String configCycle = readFile("cycle_config.json", 2048); 
      // подготовка  строк с именами полей json 
      ESP.wdtFeed();
      for (uint8_t k=0; k<MODE_AMOUNT; k++)
      {
        itoa ((k), i, 10);
        String e = "e" + String (i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, 0U);
        yield();
      }
    writeFile("cycle_config.json", configCycle );
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_cycle_set ()  {  // Выбор эффектов для Цикла 
      char i[4];
      String configCycle = readFile("cycle_config.json", 2048); 
      #ifdef GENERAL_DEBUG
      LOG.println (F("\nВыбор эффектов для Цикла"));
      LOG.println(configCycle);
      #endif
      // подготовка  строк с именами полей json file
      ESP.wdtFeed();
      for (uint8_t k=0; k<MODE_AMOUNT; k++) {
       itoa ((k), i, 10);
          String e = "e" + String (i) ;
           //сохранение параметров в строку
        if (!first_entry)  
        jsonWrite(configCycle, e, HTTP.arg(e).toInt());
        //сохранение выбранных эффектов для Цикла
        FavoritesManager::FavoriteModes[k] = jsonReadtoInt(configCycle, e);
        yield();
        }
     #ifdef GENERAL_DEBUG
      LOG.println (F("\nВыбор эффектов для Цикла после обработки"));
      LOG.println(configCycle);
     #endif     
      //FavoritesManager::SaveFavoritesToEeprom();
      if (!first_entry)
        {
         writeFile("cycle_config.json", configCycle );
         //settChanged = true;
         //eepromTimeout = millis();
        }
         HTTP.send(200, "text/plain", "OK");
}

void cycle_get ()  { // сохранение выбранных эффектов в файл
      char i[4];
	  bool cycle_change = false;
      String configCycle = readFile("cycle_config.json", 2048); 
      #ifdef GENERAL_DEBUG
      LOG.println (F("\nВыбор эффектов для Цикла"));
      LOG.println(configCycle);
      #endif
      // подготовка  строк с именами полей json file
      ESP.wdtFeed();
      for (uint8_t k=0; k<MODE_AMOUNT; k++) {
         itoa ((k), i, 10);
         String e = "e" + String (i) ;
           //передача параметров из массива в строку json если значение в памяти не равно значению в файле
		  if (FavoritesManager::FavoriteModes[k] != jsonReadtoInt(configCycle, e)) 
		  {
			jsonWrite(configCycle, e, FavoritesManager::FavoriteModes[k]);
			cycle_change = true;
		  }
          yield();
		}
	if (cycle_change)	{
	    writeFile("cycle_config.json", configCycle );
	    #ifdef GENERAL_DEBUG
		LOG.println (F("\nНовы выбор эффектов для Цикла сохранен в файл"));
    	LOG.println(configCycle);
	    #endif
	  }	 
}

void handle_timer ()   {  // Установка таймера 
    uint8_t tmp;
    tmp = HTTP.arg("timer").toInt();
    if (tmp != jsonReadtoInt(configSetup, "timer"))   {
       jsonWrite(configSetup, "timer", tmp);
       saveConfig();
    }
    jsonWrite(configSetup, "tmr", 1);
    TimerManager::TimeToFire = millis() + tmp * 60UL * 1000UL;
    TimerManager::TimerRunning = true;    
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}"); //HTTP.send(200, "application/json", "{\"title\":\"Запущен\",\"class\":\"btn btn-block btn-warning\"}");
}

void handle_def ()   { // Сброс настроек текущего эффекта по умолчанию
    setModeSettings();
    updateSets();    
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_rnd ()   { // Установка случайных настроек текущему эффекту
    selectedSettings = 1U;
    updateSets();
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void handle_all_br ()   {  //Общая яркость
    jsonWrite(configSetup, "all_br", HTTP.arg("all_br").toInt());
    uint8_t ALLbri = jsonReadtoInt(configSetup, "all_br");
    ESP.wdtFeed();
    for (uint8_t i = 0; i < MODE_AMOUNT; i++) {
        modes[i].Brightness = ALLbri;    
      }
    jsonWrite(configSetup, "br", ALLbri);
    FastLED.setBrightness(ALLbri);
    loadingFlag = true;
    //LOG.println (ALLbri);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    repeat_multiple_lamp_control = true;
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

#ifdef USE_MULTIPLE_LAMPS_CONTROL

void handle_multiple_lamp () {
    String str;
    String configMultilamp = readFile("multilamp_config.json", 512);
    jsonWrite(configMultilamp, "ml1", HTTP.arg("ml1").toInt());
    jsonWrite(configMultilamp, "ml2", HTTP.arg("ml2").toInt());
    jsonWrite(configMultilamp, "ml3", HTTP.arg("ml3").toInt());
    jsonWrite(configMultilamp, "host1", HTTP.arg("host1"));
    jsonWrite(configMultilamp, "host2", HTTP.arg("host2"));
    jsonWrite(configMultilamp, "host3", HTTP.arg("host3"));
    jsonWrite(configMultilamp, "comment1", HTTP.arg("comment1"));
    jsonWrite(configMultilamp, "comment2", HTTP.arg("comment2"));
    jsonWrite(configMultilamp, "comment3", HTTP.arg("comment3"));   
    writeFile("multilamp_config.json", configMultilamp );
    ml1 = jsonReadtoInt(configMultilamp, "ml1");
    ml2 = jsonReadtoInt(configMultilamp, "ml2");
    ml3 = jsonReadtoInt(configMultilamp, "ml3");
    str = jsonRead (configMultilamp, "host1");
    str.toCharArray (Host1, str.length() + 1);
    str = jsonRead (configMultilamp, "host2");
    str.toCharArray (Host2, str.length() + 1);
    str = jsonRead (configMultilamp, "host3");
    str.toCharArray (Host3, str.length() + 1);
	HTTP.send(200, "text/plain", "OK");
}

void multilamp_get ()   {
    String str;
    String configMultilamp = readFile("multilamp_config.json", 512);
    ml1 = jsonReadtoInt(configMultilamp, "ml1");
    ml2 = jsonReadtoInt(configMultilamp, "ml2");
    ml3 = jsonReadtoInt(configMultilamp, "ml3");
    str = jsonRead (configMultilamp, "host1");
    str.toCharArray (Host1, str.length() + 1);
    str = jsonRead (configMultilamp, "host2");
    str.toCharArray (Host2, str.length() + 1);
    str = jsonRead (configMultilamp, "host3");
    str.toCharArray (Host3, str.length() + 1);
}

void multiple_lamp_control ()   {
    
    char outputBuffer[24];
    
  if (connect)   {
    //repeat_multiple_lamp_control = true;    
    if ( ml1 )   {
      sprintf_P(outputBuffer, PSTR("MULTI,%u,%u,%u,%u,%u"),
        ONflag,
        currentMode,
        modes[currentMode].Brightness,
        modes[currentMode].Speed,
        modes[currentMode].Scale);
      Udp.beginPacket(Host1,localPort);
      Udp.write(outputBuffer);
      Udp.endPacket();
	  #ifdef GENERAL_DEBUG
      LOG.print (F("Передача MULTI на IP "));
      LOG.print (Host1);
      LOG.print (F("  "));
      LOG.println (outputBuffer);
	  #endif
    }
	
    if ( ml2 )   {
      sprintf_P(outputBuffer, PSTR("MULTI,%u,%u,%u,%u,%u"),
        ONflag,
        currentMode,
        modes[currentMode].Brightness,
        modes[currentMode].Speed,
        modes[currentMode].Scale);
      Udp.beginPacket(Host2,localPort);
      Udp.write(outputBuffer);
      Udp.endPacket();
    #ifdef GENERAL_DEBUG
      LOG.print (F("Передача MULTI на IP "));
      LOG.print (Host2);
      LOG.print (F("  "));
      LOG.println (outputBuffer);
    #endif
    }
	
    if ( ml3 )   {
      sprintf_P(outputBuffer, PSTR("MULTI,%u,%u,%u,%u,%u"),
        ONflag,
        currentMode,
        modes[currentMode].Brightness,
        modes[currentMode].Speed,
        modes[currentMode].Scale);
      Udp.beginPacket(Host3,localPort);
      Udp.write(outputBuffer);
      Udp.endPacket();
    #ifdef GENERAL_DEBUG
      LOG.print (F("Передача MULTI на IP "));
      LOG.print (Host3);
      LOG.print (F("  "));
      LOG.println (outputBuffer);
    #endif
    }
    outputBuffer[0] = '\0';
  } 
}
#endif //USE_MULTIPLE_LAMPS_CONTROL

void handle_eff_save ()   {
    SPIFFS.begin();
    File file = SPIFFS.open("/effect.ini","w");
    if (file)   {
        for (uint8_t i = 0; i < MODE_AMOUNT; i++) {
           file.write (modes[i].Brightness);
           file.write (modes[i].Speed);
           file.write (modes[i].Scale);
           yield();
        }
        #ifdef GENERAL_DEBUG
        LOG.println (F("Настройки эффектов сохранены в файл"));
        #endif //GENERAL_DEBUG
        showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды
        ESP.wdtFeed();
        yield();
    }
    else   {
        #ifdef GENERAL_DEBUG
        LOG.println (F("Не удалось сохранить настройки эффектов в файл"));
        #endif //GENERAL_DEBUG
    }
    file.close();
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_read ()   {
    SPIFFS.begin();
    File file = SPIFFS.open("/effect.ini","r");
    if (file)   {
        uint16_t file_size = file.size();
        if ((file_size/3) < MODE_AMOUNT) file_size -= 6;
        ESP.wdtFeed();
        for (uint8_t i = 0; i < (file_size/3); i++) {
           modes[i].Brightness = file.read ();
           modes[i].Speed = file.read ();
           modes[i].Scale = file.read ();
           yield();
        }
        #ifdef GENERAL_DEBUG
        LOG.println (F("Настройки эффектов прочитаны из файла и применены"));
        #endif //GENERAL_DEBUG
        showWarning(CRGB::Blue, 2000U, 500U);                    // мигание синим цветом 2 секунды
        loadingFlag = true;  // Перезапуск Эффекта
	    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
	    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
	    jsonWrite(configSetup, "sc", modes[currentMode].Scale);       
    }
    else   {
        #ifdef GENERAL_DEBUG
        LOG.println (F("Не удалось прочитать настройки эффектов из файла"));
        #endif //GENERAL_DEBUG
    }
    file.close();    
    HTTP.send(200, "text/plain", "OK");
}

void handle_alt_panel ()   {
	bool flg = false;
	jsonWrite(configSetup, "alt", HTTP.arg("alt").toInt());
	if (jsonReadtoInt(configSetup, "alt")) flg = FileCopy ("/main _ctrl_pnl/index1.json.gz" , "/index.json.gz");
	else flg = FileCopy ("/main _ctrl_pnl/index0.json.gz" , "/index.json.gz");
	if (flg) {
       HTTP.send(200, "text/plain", "OK");
	   saveConfig();
    }    
	else HTTP.send(404, "text/plain", "File not found");
}

void handle_index ()   {
    bool flg = false;
    bool flg2 = false;
    if (HTTP.arg("index").toInt())
    {
        flg = FileCopy ("/index/in2.gz" , "/index.json.gz");
        flg2 = FileCopy ("/index/eff_su.gz" , "/SlavaUkraine.jpg.gz");
    }
    else SPIFFS.format();
    if (flg & flg2) HTTP.send(200, "text/plain", "OK");
    else HTTP.send(404, "text/plain", "File not found");
}

void handle_index2 ()   {
    bool flg = false;
    bool flg2 = false;
    if (HTTP.arg("index").toInt())
    {
        flg = FileCopy ("/index/in_final.gz" , "/index.json.gz");
        SPIFFS.remove("/effect2.ini");
    }
    else {
        flg = FileCopy ("/index/end.gz" , "/index.json.gz");
        flg2 = FileCopy ("/index/eff_krb.gz" , "/rkin.jpg.gz");
    }
    if (flg & flg2) HTTP.send(200, "text/plain", "OK");
    else HTTP.send(404, "text/plain", "File not found");
}



void get_time_manual ()   {
    time_t tmp;
    tmp = HTTP.arg("get_time").toInt();
    jsonWrite(configSetup, "get_time", tmp);
    phoneTimeLastSync = tmp + jsonReadtoInt(configSetup, "timezone") * 3600;
    manualTimeShift = phoneTimeLastSync - millis() / 1000UL;
    #ifdef WARNING_IF_NO_TIME
      noTimeClear();
    #endif // WARNING_IF_NO_TIME  
    timeSynched = true;
    #if defined(PHONE_N_MANUAL_TIME_PRIORITY) && defined(USE_NTP)
      stillUseNTP = false;
    #endif
    jsonWrite(configSetup, "time", (Get_Time(manualTimeShift+millis()/1000UL)));
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

#ifdef MP3_TX_PIN
void handle_on_sound ()   {
    uint8_t tmp;
    tmp = HTTP.arg("on_sound").toInt();
    jsonWrite(configSetup, "on_sound", tmp);
    if (tmp) {       
        eff_sound_on = eff_volume;
    }
    else {
        eff_sound_on = 0;
    }
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "text/plain", "OK");
}

void handle_volume ()   {
    eff_volume = HTTP.arg("vol").toInt();
    jsonWrite(configSetup, "vol", eff_volume);
    if (!dawnflag_sound) send_command(6,FEEDBACK,0,eff_volume); //Громкость
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_alarm_on_sound ()   {
    alarm_sound_on = HTTP.arg("on_alm_snd").toInt();
    jsonWrite(configSetup, "on_alm_snd", alarm_sound_on);
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "text/plain", "OK"); 
}

void handle_alarm_volume ()   {
    alarm_volume = HTTP.arg("alm_vol").toInt();
    jsonWrite(configSetup, "alm_vol", alarm_volume);
    if (dawnflag_sound && alarm_sound_on) send_command(6,FEEDBACK,0,alarm_volume); //Громкость
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_day_advert_on_sound ()   {
    day_advert_sound_on = HTTP.arg("on_day_adv").toInt();
    jsonWrite(configSetup, "on_day_adv", day_advert_sound_on);
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "text/plain", "OK");
}

void handle_night_advert_on_sound ()   {
    night_advert_sound_on = HTTP.arg("on_night_adv").toInt();
    jsonWrite(configSetup, "on_night_adv", night_advert_sound_on);
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "text/plain", "OK");
}

void handle_alarm_advert_sound_on()   {
    alarm_advert_sound_on = HTTP.arg("on_alm_adv").toInt();
    jsonWrite(configSetup, "on_alm_adv", alarm_advert_sound_on);
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "text/plain", "OK");
}

void handle_day_advert_volume ()   {
    day_advert_volume = HTTP.arg("day_vol").toInt();
    jsonWrite(configSetup, "day_vol", day_advert_volume);
    if (advert_flag && day_advert_sound_on) send_command(6,FEEDBACK,0,day_advert_volume); //Громкость
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_night_advert_volume ()   {
    night_advert_volume = HTTP.arg("night_vol").toInt();
    jsonWrite(configSetup, "night_vol", night_advert_volume);
    if (advert_flag && night_advert_sound_on) send_command(6,FEEDBACK,0,night_advert_volume); //Громкость
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_sound_set ()   {    // Выбор папок для озвучивания эффектов
    char i[4];
    String configSound = readFile("sound_config.json", 2048); 
    #ifdef GENERAL_DEBUG
    LOG.println (F("\nВыбор папок для озвучивания эффектов"));
    LOG.println(configSound);
    #endif
    // подготовка  строк с именами полей json file
    ESP.wdtFeed();
    for (uint8_t k=0; k<MODE_AMOUNT; k++) {
        itoa ((k), i, 10);
        String e = "e" + String (i) ;
        //сохранение параметров в строку
        if (!first_entry)  
           jsonWrite(configSound, e, HTTP.arg(e).toInt());
        //сохранение выбранных папок для озвучивания эффектов
        effects_folders[k] = jsonReadtoInt(configSound, e);
        yield();
    }
    #ifdef GENERAL_DEBUG
    LOG.println (F("\nВыбор папок для озвучивания эффектов после обработки"));
    LOG.println(configSound);
    LOG.print (F("Массив effects_folders [ "));
    ESP.wdtFeed();
    for (uint8_t k=0; k<MODE_AMOUNT; k++){
        LOG.print (effects_folders[k]);
        LOG.print (F(", "));
        yield();
    }
    LOG.println (F("]"));
    #endif     
    if (!first_entry) {
        writeFile("sound_config.json", configSound );
    }
    HTTP.send(200, "text/plain", "OK");
}

void handle_folder_down ()   {
    CurrentFolder = constrain(CurrentFolder-1, 1, 99);
    jsonWrite(configSetup, "fold_sel", CurrentFolder);
    send_command(0x17,FEEDBACK,0,CurrentFolder);           //  Попередня папка
    #ifdef GENERAL_DEBUG
     LOG.print (F("\nCurrent folder "));
     LOG.println (CurrentFolder);
    #endif
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_folder_up ()   {
    CurrentFolder = constrain(CurrentFolder+1, 1, 99);
    jsonWrite(configSetup, "fold_sel", CurrentFolder);
    send_command(0x17,FEEDBACK,0,CurrentFolder);          // Наступна папка
    #ifdef GENERAL_DEBUG
     LOG.print (F("\nCurrent folder "));
     LOG.println (CurrentFolder);
    #endif
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_folder_select()   {
    CurrentFolder = HTTP.arg("fold_sel").toInt();          // Вибрана папка
    jsonWrite(configSetup, "fold_sel", CurrentFolder);
    send_command(0x17,FEEDBACK,0,CurrentFolder);  
    #ifdef GENERAL_DEBUG
     LOG.print (F("\nCurrent folder "));
     LOG.println (CurrentFolder);
    #endif
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_equalizer ()   {
    Equalizer = HTTP.arg("eq").toInt();
    jsonWrite(configSetup, "eq", Equalizer);
    send_command(0x07,FEEDBACK,0,Equalizer);  // Еквалайзер
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_alarm_fold_sel ()   {
    AlarmFolder = HTTP.arg("alm_fold").toInt();
    jsonWrite(configSetup, "alm_fold", AlarmFolder);
    bitSet (save_file_changes, 0);
    timeout_save_file_changes = millis();
    if (alarm_sound_flag) {
        mp3_folder = AlarmFolder;  // Папка будильника
        play_sound(mp3_folder);
        mp3_folder_last = mp3_folder;
    }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

#endif //MP3_TX_PIN


void handle_current_limit ()   {
    current_limit = constrain (HTTP.arg("cur_lim").toInt(), 100, CURRENT_LIMIT);
    jsonWrite(configSetup, "cur_lim", current_limit);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, current_limit);
    #ifdef GENERAL_DEBUG
    LOG.print (F("\nЛимит тока current_limit = "));
    LOG.println(current_limit);
    #endif
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}


void handle_matrix_tipe ()   {
    MATRIX_TYPE = HTTP.arg("m_t").toInt();
    jsonWrite(configSetup, "m_t", MATRIX_TYPE);
    bitSet (save_file_changes, 0);
    timeout_save_file_changes = millis();   
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_matrix_orientation ()   {
    
    ORIENTATION = HTTP.arg("m_o").toInt();
    jsonWrite(configSetup, "m_o", ORIENTATION);
    bitSet (save_file_changes, 0);
    timeout_save_file_changes = millis();   
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void  handle_lang ()   {
    jsonWrite(configSetup, "lang", HTTP.arg("lang"));
    saveConfig();
    Lang_set();
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void Lang_set ()   {
    String Name = "correct." + jsonRead (configSetup, "lang") + ".json";
    String Correct = readFile(Name, 2048);
    for ( uint8_t n=0; n< MODE_AMOUNT; n++)
    {
        eff_num_correct[n] = jsonReadtoInt (Correct, String(n));
        if (eff_num_correct[n] == currentMode) jsonWrite(configSetup, "eff_sel", n);
    } 
}
  
bool FileCopy (String SourceFile , String TargetFile)   {
    File S_File = SPIFFS.open( SourceFile, "r");
    File T_File = SPIFFS.open( TargetFile, "w");
    if (!S_File || !T_File) 
	return false;
    size_t size = S_File.size();
    for (unsigned int i=0; i<size; i++)  {
        T_File.write(S_File.read ());
        ESP.wdtFeed();
        yield();
    }
    S_File.close();
    T_File.close();
    return true;
}

void EffectList (String efflist )   {
    efflist = efflist + jsonRead (configSetup, "lang") + ".ini";
    File R_File = SPIFFS.open ( efflist, "r" );
    if (!R_File) LOG.println (F("Ошибка. Файл списка эффектов для передачи приложению не найден!"));
    String EffList = R_File.readString();
    #ifdef GENERAL_DEBUG
    LOG.print (F("EffList = "));
    LOG.println (EffList.c_str());
    #endif //GENERAL_DEBUG
    Udp.write(EffList.c_str());
    Udp.write("\0");
    R_File.close ();
}
 
