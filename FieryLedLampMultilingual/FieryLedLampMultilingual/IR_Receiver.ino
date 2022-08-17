// 
#ifdef IR_RECEIVER_USE

#define IR_REPEET_TIMER      1000   // Час очікування повтору
#define IR_TICK_TIMER        100    // Час між автоповтором
#define IR_DIGIT_ENTER_TIMER 3000   // час для введення другої цифри номеру ефекту

void IR_Receive_Handle ()   {       // Обробка прийнятого сигналу
    if (irrecv.decode(&results)) {
        if (results.repeat) { 
            if (millis() - IR_Repeet_Timer > IR_REPEET_TIMER) {
                //Serial.print("Repeat  ");
                IR_Data_Ready = 2;
            }
        }
        else {
            IR_Code = (uint32_t)results.value;
            IR_Repeet_Timer = millis();
            IR_Data_Ready = 1;
        }
    irrecv.resume();  // Receive the next value
    }

    if (Enter_Digit_1 && millis() - IR_Dgit_Enter_Timer > IR_DIGIT_ENTER_TIMER){  // Якщо одна цифра натиснута та час очікування натискання другої цифри вийшов
        Enter_Digit_1 = 0;
        currentMode = eff_num_correct[Enter_Number];
	    jsonWrite(configSetup, "eff_sel", Enter_Number);
	    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
        FastLED.setBrightness(modes[currentMode].Brightness);
        loadingFlag = true;
        settChanged = true;
        eepromTimeout = millis();
        timeout_save_file_changes = millis();
        bitSet (save_file_changes, 0);
        if (random_on && FavoritesManager::FavoritesRunning)
            selectedSettings = 1U;
        #if (USE_MQTT)
           if (espMode == 1U) MqttManager::needToPublish = true;
        #endif
        #ifdef USE_BLYNK
          updateRemoteBlynkParams();
        #endif
        #ifdef USE_MULTIPLE_LAMPS_CONTROL
          multiple_lamp_control ();
        #endif  //USE_MULTIPLE_LAMPS_CONTROL
        //Serial.println(Enter_Number);
        //Serial.println("  1 цифра");
  }
}

void IR_Receive_Button_Handle()   {     //Обробка прийнятих команд (натиснутих кнопок пульта ДК)
    switch(IR_Code) {
        case IR_ON_OFF:
        if (IR_Data_Ready != 2)  // No repeet
        IR_Power();
        break;
        case IR2_ON_OFF:
        if (IR_Data_Ready != 2)  // No repeet
        IR_Power();
        break;
        
        case IR_MUTE:
        if (IR_Data_Ready != 2)  // No repeet
        Mute();
        break;
        case IR2_MUTE:
        if (IR_Data_Ready != 2)  // No repeet
        Mute();
        break;
        
        case IR_PREV:
        Prev_eff();
        break;
        case IR2_PREV:
        Prev_eff();
        break;
        case IR_NEXT:
        Next_eff();
        break;
        case IR2_NEXT:
        Next_eff();
        break;
        
        case IR_CYCLE:
        if (IR_Data_Ready != 2)  // No repeet
        Cycle_on_off();
        break;
        case IR2_CYCLE:
        if (IR_Data_Ready != 2)  // No repeet
        Cycle_on_off();
        break;
        
        case IR_BR_UP:
        Bright_Up_Down(true);
        break;
        case IR2_BR_UP:
        Bright_Up_Down(true);
        break;
        case IR_BR_DOWN:
        Bright_Up_Down(false);
        break;
        case IR2_BR_DOWN:
        Bright_Up_Down(false);
        break;
       
        case IR_SP_UP:
        Speed_Up_Down(true);
        break;
        case IR2_SP_UP:
        Speed_Up_Down(true);
        break;
        case IR_SP_DOWN:
        Speed_Up_Down(false);
        break;
        case IR2_SP_DOWN:
        Speed_Up_Down(false);
        break;
       
        case IR_SC_UP:
        Scale_Up_Down(true);
        break;
        case IR2_SC_UP:
        Scale_Up_Down(true);
        break;
        case IR_SC_DOWN:
        Scale_Up_Down(false);
        break;
        case IR2_SC_DOWN:
        Scale_Up_Down(false);
        break;
       
        case IR_VOL_UP:
        Volum_Up_Down(true);
        break;
        case IR2_VOL_UP:
        Volum_Up_Down(true);
        break;
        case IR_VOL_DOWN:
        Volum_Up_Down(false);
        break;
        case IR2_VOL_DOWN:
        Volum_Up_Down(false);
        break;
        
        case IR_TIME:
        if (IR_Data_Ready != 2)  // No repeet
        printTime(thisTime, true, ONflag);
        break;
        case IR2_TIME:
        if (IR_Data_Ready != 2)  // No repeet
        printTime(thisTime, true, ONflag);
        break;
        
        case IR_IP:
        if (IR_Data_Ready != 2)  // No repeet
        Print_IP();
        break;
        case IR2_IP:
        if (IR_Data_Ready != 2)  // No repeet
        Print_IP();
        break;
        
        case IR_FOLD_PREV:
        Folder_Next_Prev(false);
        break;
        case IR2_FOLD_PREV:
        Folder_Next_Prev(false);
        break;
        case IR_FOLD_NEXT:
        Folder_Next_Prev(true);
        break;
        case IR2_FOLD_NEXT:
        Folder_Next_Prev(true);
        break;
        
        case IR_RND:
        if (IR_Data_Ready != 2)  // No repeet
        Current_Eff_Rnd_Def(true);
        break;
        case IR2_RND:
        if (IR_Data_Ready != 2)  // No repeet
        Current_Eff_Rnd_Def(true);
        break;
        case IR_DEF:
        if (IR_Data_Ready != 2)  // No repeet
        Current_Eff_Rnd_Def(false);
        break;
        case IR2_DEF :
        if (IR_Data_Ready != 2)  // No repeet
        Current_Eff_Rnd_Def(false);
        break;
        
        case IR_EQ:
        if (IR_Data_Ready != 2)  // No repeet
        IR_Equalizer();
        break;
        case IR2_EQ:
        if (IR_Data_Ready != 2)  // No repeet
        IR_Equalizer();
        break;
        
        case IR_FAV_ADD:
        if (IR_Data_Ready != 2)  // No repeet
        Favorit_Add_Del(true);
        break;
        case IR2_FAV_ADD:
        if (IR_Data_Ready != 2)  // No repeet
        Favorit_Add_Del(true);
        break;
        case IR_FAV_DEL:
        if (IR_Data_Ready != 2)  // No repeet
        Favorit_Add_Del(false);
        break;
        case IR2_FAV_DEL :
        if (IR_Data_Ready != 2)  // No repeet
        Favorit_Add_Del(false);
        break;

        case IR_1:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(1);
        break;
        case IR2_1 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(1);
        break;
        case IR_2:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(2);
        break;
        case IR2_2 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(2);
        break;
        case IR_3:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(3);
        break;
        case IR2_3 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(3);
        break;
        case IR_4:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(4);
        break;
        case IR2_4 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(4);
        break;
        case IR_5:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(5);
        break;
        case IR2_5 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(5);
        break;
        case IR_6:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(6);
        break;
        case IR2_6 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(6);
        break;
        case IR_7:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(7);
        break;
        case IR2_7 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(7);
        break;
        case IR_8:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(8);
        break;
        case IR2_8 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(8);
        break;
        case IR_9:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(9);
        break;
        case IR2_9 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(9);
        break;
        case IR_0:
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(0);
        break;
        case IR2_0 :
        if (IR_Data_Ready != 2)  // No repeet
        Digit_Handle(0);
        break;

        default: break;
    }
    #ifdef GENERAL_DEBUG
    LOG.print("IR_CODE = ");
    LOG.println(IR_Code, HEX);
    #endif  //GENERAL_DEBUG
}

void IR_Power()   {
    if (dawnFlag) {
        #ifdef MP3_TX_PIN
        if (alarm_sound_flag) {
           //myDFPlayer.pause();
           send_command(0x0E,0,0,0); //Пауза
           mp3_stop = true;
           alarm_sound_flag = false;
        }
        else
        #endif  //MP3_TX_PIN
        {
            manualOff = true;
            dawnFlag = false;
            #ifdef TM1637_USE
            clockTicker_blink();
            #endif
            FastLED.setBrightness(modes[currentMode].Brightness);
            changePower();
       }
       return;
    }
    else
    {
    //Serial.print ("До ONflag=");
    //Serial.println (ONflag);
        ONflag = !ONflag;
	    jsonWrite(configSetup, "Power", ONflag);
        changePower();
    //Serial.print ("После ONflag=");
   // Serial.println (ONflag);
    }
    settChanged = true;
    save_file_changes = 7;
    if (ONflag)  {
        eepromTimeout = millis();
        timeout_save_file_changes = millis();
    }
    else {
        eepromTimeout = millis() - EEPROM_WRITE_DELAY;
        timeout_save_file_changes = millis() - SAVE_FILE_DELAY_TIMEOUT;
    }
    loadingFlag = true;

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
    multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
}

void Mute()   {                // Вкл / Откл звука
    #ifdef MP3_TX_PIN
    if (mp3_player_connect == 4) {
      if (eff_sound_on) {
        eff_sound_on = 0;
        #ifdef GENERAL_DEBUG
        LOG.println (F("Звук выключен"));
        #endif
      }
      else {
        eff_sound_on = eff_volume;
        #ifdef GENERAL_DEBUG
        LOG.println (F("Звук включен"));
        #endif
      }
    }
    else  {
        showWarning(CRGB::Red, 1000, 250U);                    // мигание красным цветом 1 секунду
        #ifdef GENERAL_DEBUG
        LOG.println (F("mp3 player не подключен"));
        #endif
    }
    jsonWrite(configSetup, "on_sound", constrain (eff_sound_on,0,1));
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
  #endif  //MP3_TX_PIN
}

void Prev_eff()   {
  if (ONflag)
  {
    String Name = "correct." + jsonRead (configSetup, "lang") + ".json";
    String Correct = readFile(Name, 2048);
    uint8_t temp = jsonReadtoInt(configSetup, "eff_sel");
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
    multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
  }
}

void Next_eff()   {
  if (ONflag)    
  {
    String Name = "correct." + jsonRead (configSetup, "lang") + ".json";
    String Correct = readFile(Name, 2048);
    uint8_t temp = jsonReadtoInt(configSetup, "eff_sel");
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
    multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
  }
}

void Cycle_on_off()   {
    uint8_t tmp;
    if (ONflag)   {
        jsonReadtoInt(configSetup, "cycle_on") == 0? tmp = 1 : tmp = 0;
	    jsonWrite(configSetup, "cycle_on", tmp);
	    FavoritesManager::FavoritesRunning = tmp;
        #if (USE_MQTT)
        if (espMode == 1U)
        {
          MqttManager::needToPublish = true;
        }
        #endif
    }
}

void Bright_Up_Down(bool direction)   {
    uint8_t delta = IR_Data_Ready == 1 ? 1U : 10U;
    modes[currentMode].Brightness = constrain(direction ? modes[currentMode].Brightness + delta : modes[currentMode].Brightness - delta, 1, 255);
	jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    FastLED.setBrightness(modes[currentMode].Brightness);

    #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение яркости: %d\n"), modes[currentMode].Brightness);
    #endif
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    settChanged = true;
    eepromTimeout = millis();
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
        multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
    #if (USE_MQTT)
    if (espMode == 1U)
    {
        MqttManager::needToPublish = true;
    }
    #endif
}

void Speed_Up_Down(bool direction)   {
    uint8_t delta = IR_Data_Ready == 1 ? 1U : 10U;
    modes[currentMode].Speed = constrain(direction ? modes[currentMode].Speed + delta : modes[currentMode].Speed - delta, 1, 255);
	jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    loadingFlag = true; // без перезапуска эффекта ничего и не увидишь

    #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение скорости: %d\n"), modes[currentMode].Speed);
    #endif
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    settChanged = true;
    eepromTimeout = millis();
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
        multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
    #if (USE_MQTT)
    if (espMode == 1U)
    {
        MqttManager::needToPublish = true;
    }
    #endif
}

void Scale_Up_Down(bool direction)   {
    uint8_t delta = IR_Data_Ready == 1 ? 1U : 5U;
    modes[currentMode].Scale = constrain(direction ? modes[currentMode].Scale + delta : modes[currentMode].Scale - delta, 1, 100);
	jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    loadingFlag = true; // без перезапуска эффекта ничего и не увидишь

    #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение масштаба: %d\n"), modes[currentMode].Scale);
    #endif
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    settChanged = true;
    eepromTimeout = millis();
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
        multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
    #if (USE_MQTT)
    if (espMode == 1U)
    {
        MqttManager::needToPublish = true;
    }
    #endif
}

void Volum_Up_Down (bool direction)   {
    #ifdef MP3_TX_PIN
    eff_volume = constrain(direction ? eff_volume + 1 : eff_volume - 1, 1, 30);
    jsonWrite(configSetup, "vol", eff_volume);
    if (!dawnflag_sound) send_command(6,FEEDBACK,0,eff_volume); //Громкость
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
    #endif  //MP3_TX_PIN
}

void Print_IP()   {
    #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, матрица должна быть включена на время вывода текста
      digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
    #endif
    if (espMode == 1U)
    {
      loadingFlag = true;
      while(!fillString(WiFi.localIP().toString().c_str(), CRGB::White, false)) { delay(1); ESP.wdtFeed();}
      loadingFlag = true;
    }
    else
    {
      loadingFlag = true;
      String str = "Access Point 192.168.4.1";
      while(!fillString(str.c_str(), CRGB::White, false)) { delay(1); ESP.wdtFeed();}
      loadingFlag = true;
    }
    #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы или будильника
      digitalWrite(MOSFET_PIN, ONflag || (dawnFlag && !manualOff) ? MOSFET_LEVEL : !MOSFET_LEVEL);
    #endif
}

void Folder_Next_Prev(bool direction)    {
    CurrentFolder = constrain(direction ? CurrentFolder + 1 : CurrentFolder - 1, 1, 99);
    jsonWrite(configSetup, "fold_sel", CurrentFolder);
    send_command(0x17,FEEDBACK,0,CurrentFolder);           //  Попередня папка
    #ifdef GENERAL_DEBUG
     LOG.print (F("\nCurrent folder "));
     LOG.println (CurrentFolder);
    #endif
}

void Current_Eff_Rnd_Def(bool direction)   {
    if (direction) {
    selectedSettings = 1U;
    updateSets();
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
    }
    else {
    setModeSettings();
    updateSets();    
    #ifdef USE_MULTIPLE_LAMPS_CONTROL
    multiple_lamp_control ();
    #endif  //USE_MULTIPLE_LAMPS_CONTROL
    }
}

void IR_Equalizer()   {     // Устанавливаем эквалайзер
    Equalizer++;
    if (Equalizer > 5) Equalizer = 0;
    jsonWrite(configSetup, "eq", Equalizer);
    send_command(0x07, FEEDBACK, 0, Equalizer); 
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 0);
}

void Favorit_Add_Del(bool direction)   {
    String configCycle = readFile("cycle_config.json", 2048);
    String e = "e" + String (currentMode);
    jsonWrite(configCycle, e, direction ? 1 : 0);
    FavoritesManager::FavoriteModes[currentMode] = (direction ? 1 : 0);
    //writeFile("cycle_config.json", configCycle );
    timeout_save_file_changes = millis();
    bitSet (save_file_changes, 2);
}

void Digit_Handle (uint8_t digit)   {
    if (!Enter_Digit_1){
        Enter_Digit_1 = 1;
        IR_Dgit_Enter_Timer = millis();
        Enter_Number = digit;
     Serial.println(Enter_Number);
    }
    else {
        Enter_Digit_1 = 0;
        Enter_Number = Enter_Number * 10 + digit;
        currentMode = eff_num_correct[Enter_Number];
	    jsonWrite(configSetup, "eff_sel", Enter_Number);
	    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
        FastLED.setBrightness(modes[currentMode].Brightness);
        loadingFlag = true;
        settChanged = true;
        eepromTimeout = millis();
        timeout_save_file_changes = millis();
        bitSet (save_file_changes, 0);
        if (random_on && FavoritesManager::FavoritesRunning)
            selectedSettings = 1U;
        #if (USE_MQTT)
           if (espMode == 1U) MqttManager::needToPublish = true;
        #endif
        #ifdef USE_BLYNK
          updateRemoteBlynkParams();
        #endif
        #ifdef USE_MULTIPLE_LAMPS_CONTROL
          multiple_lamp_control ();
        #endif  //USE_MULTIPLE_LAMPS_CONTROL
        //Serial.println(Enter_Number);
        //Serial.println("  2 цифри");
    }
}

#endif //IR_RECEIVER_USE
