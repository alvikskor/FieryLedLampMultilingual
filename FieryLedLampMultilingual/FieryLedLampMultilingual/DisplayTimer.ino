#ifdef TM1637_USE
void Display_Timer ()   {
    
    if (!DisplayFlag && LastEffect != currentMode) {
        LastEffect = currentMode;
        DisplayTimer = millis();
        DisplayFlag = 1;
        display.point(1);
        display.displayByte(_E_, _F_, _empty, _empty);               // отображаем EF
        uint8_t n;
        for (n = 0; n < MODE_AMOUNT; n++)
        {
            if (eff_num_correct[n] == currentMode) break;
        }
        display.showNumberDec (n, 0, 2, 2);                // отображаем номер эффекта
    }
        if (DisplayFlag == 1 && (millis() - DisplayTimer > 3000)) {
            DisplayFlag = 0;
            if (timeSynched) clockTicker_blink ();                   // отображаем  время
            else display.displayByte(_dash, _dash, _dash, _dash);    // отображаем прочерки
        }
    #ifdef MP3_TX_PIN
    if (!DisplayFlag && LastCurrentFolder != CurrentFolder) {
        LastCurrentFolder = CurrentFolder;
        DisplayTimer = millis();
        DisplayFlag = 2;
        display.point(1);
        display.displayByte(_F_, _o_, _empty, _empty);               // отображаем Fo
        display.showNumberDec (CurrentFolder, 0, 2, 2);              // отображаем номер папки
    }
        if (DisplayFlag == 2 && (millis() - DisplayTimer > 3000)) {
            DisplayFlag = 0;
            if (timeSynched) clockTicker_blink ();                   // отображаем  время
            else display.displayByte(_dash, _dash, _dash, _dash);    // отображаем прочерки
        }
    #endif  //MP3_TX_PIN
}
#endif  //TM1637_USE