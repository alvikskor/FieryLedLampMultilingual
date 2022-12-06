//---------------------------------------
// My global variables
uint8_t custom_eff = 0;
//---------------------------------------
// --------------------------------------
/* выделяем в памяти масив для загружаемых бинарных изображений
  header= 16 | заголовок
  w=16, h=16 | ширина высота
  color  = 2 | байт на цвет
  frames = 5 | количество кадров
  масив на 5 кадров 16x16 | размером w * h * frames * color + header = 2 576
  размер можно увеличивать по мере надобности, постоянно занимает место в памяти
  возможно в будущем будет сделано динамическим */
//byte binImage[2576];

// ======================================
// espModeStat default lamp start effect
// ======================================
void  espModeState(uint8_t color) {
  if (loadingFlag) {
    loadingFlag = false;
    step = deltaValue;
    deltaValue = 1;
    hue2 = 0;
    deltaHue2 = 1;
    DrawLine(CENTER_X_MINOR, CENTER_Y_MINOR, CENTER_X_MAJOR + 1, CENTER_Y_MINOR, CHSV(color, 255, 210));
    DrawLine(CENTER_X_MINOR, CENTER_Y_MINOR - 1, CENTER_X_MAJOR + 1, CENTER_Y_MINOR - 1, CHSV(color, 255, 210));
    // setModeSettings(128U, 128U);
    pcnt = 1;
    FastLED.clear();
  }
  if (pcnt > 0 & pcnt < 200) {
    if (pcnt != 0) {
      pcnt++;
    }

    // animation esp state ===========
    dimAll(108);
    //    if (step % 2 == 0) {
    uint8_t w = validMinMax(hue2, 0, floor(WIDTH / 2) - 1);
    uint8_t posY = validMinMax(CENTER_Y_MINOR + deltaHue2, 0, HEIGHT - 1);
    DrawLine(CENTER_X_MINOR - w, posY, CENTER_X_MAJOR + w, posY, CHSV(color, 255, (210 - deltaHue2)));
    posY = validMinMax(CENTER_Y_MINOR - 1 - deltaHue2, 1, HEIGHT - 1);
    DrawLine(CENTER_X_MINOR - w, posY, CENTER_X_MAJOR + w, posY, CHSV(color, 255, (210 - deltaHue2)));

    if (deltaHue2 == 0) {
      deltaHue2 = 1;
    }
    hue2++;
    deltaHue2 = deltaHue2 << 1;
    if (deltaHue2 == 2) {
      deltaHue2 = deltaHue2 << 1;
    }
    if (CENTER_Y_MINOR + deltaHue2 > HEIGHT) {
      deltaHue2 = 0;
      hue2 = 0;
    }
    // LOG.printf_P(PSTR("espModeState | pcnt = %05d | deltaHue2 = %03d | step %03d | ONflag • %s\n"), pcnt, deltaHue2, step, (ONflag ? "TRUE" : "FALSE"));
  } else {

#ifdef USE_NTP
    // error ntp ------------------
    color = 255;        // если при включенном NTP время не получено, будем красным цветом мигать
#else
    color = 176U;       // иначе скромно синим - нормальная ситуация при отсутствии NTP
#endif //USE_NTP
    // animtion no time -----------
    leds[XY(CENTER_X_MINOR , 0U)] = CHSV( color, 255, (step % 4 == 0) ? 200 : 128);

  }
  // clear led lamp ---------------
  if ( pcnt >= 100) {
    pcnt = 0;
    //    FastLED.clear();
    //    FastLED.delay(2);
    FastLED.clear();
    delay(2);
    FastLED.show();
    loadingFlag = false;
  }
  step++;
}

//---------------------------------------
// Global Function
//---------------------------------------
void drawRec(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint32_t color) {
  for (uint8_t y = startY; y < endY; y++) {
    for (uint8_t x = startX; x < endX; x++) {
      drawPixelXY(x, y, color);
    }
  }
}

//---------------------------------------
void drawRecCHSV(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, CHSV color) {
  for (uint8_t y = startY; y < endY; y++) {
    for (uint8_t x = startX; x < endX; x++) {
      drawPixelXY(x, y, color);
    }
  }
}

//--------------------------------------
uint8_t validMinMax(float val, uint8_t minV, uint32_t maxV) {
  uint8_t result;
  if (val <= minV) {
    result = minV;
  } else if (val >= maxV) {
    result = maxV;
  } else {
    result = ceil(val);
  }
  //  LOG.printf_P(PSTR( "result: %f | val: %f \n\r"), result, val);
  return result;
}

// альтернативный градиент для ламп собраных из лент с вертикальной компоновкой
// gradientHorizontal | gradientVertical менее производительный но работает на всех видах ламп
//--------------------------------------
void gradientHorizontal(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t start_color, uint8_t end_color, uint8_t start_br, uint8_t end_br, uint8_t saturate) {
  float step_color = 0;
  float step_br = 0;
  if (startX == endX) {
    endX++;
  }
  if (startY == endY) {
    endY++;
  }
  step_color = (end_color - start_color) / abs(startX - endX);
  if (start_color >  end_color) {
    step_color -= 1.2;
  } else {
    step_color += 1.2;
  }

  step_br = (end_br - start_br) / abs(startX - endX);
  if (start_br >  end_color) {
    step_br -= 1.2;
  } else {
    step_br += 1.2;
  }

  // LOG.printf_P(PSTR( "\n step_color: %f | step_br: %f \n\n\r"), step_color, step_br);
  for (uint8_t x = startX; x < endX; x++) {
    for (uint8_t y = startY; y < endY; y++) {
      CHSV thisColor = CHSV((uint8_t) validMinMax((start_color + (x - startX) * step_color), 1, 254), saturate,
                            (uint8_t) validMinMax((start_br + (x - startX) * step_br), 0, 255) );
      drawPixelXY(x, y, thisColor);
    }
  }
}

//--------------------------------------
void gradientVertical(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t start_color, uint8_t end_color, uint8_t start_br, uint8_t end_br, uint8_t saturate) {
  float step_color = 0;
  float step_br = 0;
  if (startX == endX) {
    endX++;
  }
  if (startY == endY) {
    endY++;
  }
  step_color = (end_color - start_color) / abs(startY - endY);

  if (start_color >  end_color) {
    step_color -= 1.2;
  } else {
    step_color += 1.2;
  }

  step_br = (end_br - start_br) / abs(startY - endY);
  if (start_br >  end_color) {
    step_br -= 1.2;
  } else {
    step_br += 1.2;
  }
  for (uint8_t y = startY; y < endY; y++) {
    CHSV thisColor = CHSV( (uint8_t) validMinMax((start_color + (y - startY) * step_color), 0, 255), saturate,
                           (uint8_t) validMinMax((start_br + (y - startY) * step_br), 0, 255) );
    for (uint8_t x = startX; x < endX; x++) {
      drawPixelXY(x, y, thisColor);
    }
  }
}

//---------------------------------------
// gradientDownTop • более плавный градиент в отличие от gradientVertical
// но может некоректно работать на лампах собранных на ленточных светодиодах
//---------------------------------------
void gradientDownTop( uint8_t bottom, CHSV bottom_color, uint8_t top, CHSV top_color ) {
  //  FORWARD_HUES: hue always goes clockwise
  //  BACKWARD_HUES: hue always goes counter-clockwise
  //  SHORTEST_HUES: hue goes whichever way is shortest
  //  LONGEST_HUES: hue goes whichever way is longest
  if (STRIP_DIRECTION < 2) {
    // STRIP_DIRECTION to UP ========
    fill_gradient(leds, top * WIDTH, top_color, bottom * WIDTH, bottom_color, SHORTEST_HUES);
  } else {
    // STRIP_DIRECTION to DOWN ======
    fill_gradient(leds, NUM_LEDS - bottom * WIDTH - 1, bottom_color, NUM_LEDS - top * WIDTH, top_color, SHORTEST_HUES);
  }
}

// ======================================
// функции для работы с бинарными файлами
// ======================================

/*
// --------------------------------------
// функция чтения бинарного файла изображения
//    из файловой системы лампы

void readBinFile(String fileName, size_t len ) {

  File binFile = SPIFFS.open("/" + fileName, "r");
  if (!binFile) {
    LOG.println("File not found");
    printMSG("Bin File not found", true);
    return;
  }
  size_t size = binFile.size();
  if (size > len) {
    binFile.close();
    LOG.println("Large File");
    return;
  }

  byte buffer[size];
  uint16_t amount;

  if (binFile == NULL) exit (1);
  binFile.seek(0);

  while (binFile.available()) {
    amount = binFile.read(buffer, size);
  }

#ifdef GENERAL_DEBUG
  LOG.printf_P(PSTR("File size • %08d bytes\n"), amount);
#endif

  // binImage = malloc(amount);
  // byte *by = malloc(1024);
  // memset(binImage, 66, 1552);
  // byte *by = new byte[size];
  memcpy(binImage, buffer, amount);
  binFile.close();
}

// --------------------------------------
// функция получения размера изображения
//   из заголовка файла


uint16_t getSizeValue(byte* buffer, byte b ) {
  return  (buffer[b + 1] << 8) + buffer[b];
}

// --------------------------------------
// функция скрола изображения по оси X

void scrollImage(uint16_t imgW, uint16_t imgH, uint16_t start_row) {
  const byte HEADER = 16;
  const uint16_t BYTES_PER_PIXEL = 2U;
  // const uint16_t imgSize = imgW * imgH * BYTES_PER_PIXEL + HEADER;
  uint8_t r, g, b;
  uint8_t padding = (HEIGHT - imgH) / 2;
  uint8_t topPos = HEIGHT - padding - 1;
  uint16_t pixIndex;

  for (uint16_t x = 0; x < WIDTH; x++) {
    for (uint16_t y = 0; y < imgH; y++) {
      uint8_t delta = 0;
      pixIndex = HEADER + (start_row + x + y * imgW) * BYTES_PER_PIXEL;

      // convert rgb565 to rgb888 -----------
      // masc rgb565  0xF800 | 0x07E0 | 0x001F
      r = (binImage[pixIndex + 1] & 0xF8);
      g = ((binImage[pixIndex + 1] & 0x07) << 5) + ((binImage[pixIndex] & 0xE0) << 5);
      b = (binImage[pixIndex] & 0x1F) << 3;
      // вариант с изменением яркости ----
        //hue = abs(16 - start_row) * 4;
        //leds[XY(x, topPos - y - delta)] = CRGB(constrain(r - hue, 0, 255), constrain(g - hue, 0, 255), constrain(b - hue, 0, 255));
      // ------------------------------------
      
      leds[XY(x, topPos - y - delta)] = CRGB(r, g, b);
      // drawPixelXY(x, topPos - y - delta, CRGB(r, g, b));
      // draw background 
      if ((start_row == 0) && (y == 0) && (padding > 0)) {
        drawRec(0, HEIGHT - padding, WIDTH, HEIGHT, getPixColorXY(0, topPos));
        drawRec(0, 0, WIDTH, padding, getPixColorXY(0, topPos));
      }
    } // end for y
  }
}
*/

// ======================================
// New Effects
// ======================================
uint32_t colorDimm(uint32_t colorValue, long lenght, long pixel) {

  uint8_t red = (colorValue & 0x00FF0000) >> 16;
  uint8_t green = (colorValue & 0x0000FF00) >> 8;
  uint8_t blue = (colorValue & 0x000000FF);

  double prozent = 100 / lenght;

  red = red - red * ((prozent * pixel) / 100);
  green = green - green * ((prozent * pixel) / 100);
  blue = blue - blue * ((prozent * pixel) / 100);

  // colorValue = strip.Color(red,green,blue);
  colorValue = red;
  colorValue = (colorValue << 8) + green;
  colorValue = (colorValue << 8) + blue;
  return colorValue;
}

// =============== Wine ================
//    © SlingMaster | by Alex Dovby
//               EFF_WINE
//--------------------------------------

void colorsWine() {
  uint8_t divider;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(20U + random8(200U), 200U);
    }
#endif
    loadingFlag = false;
    fillAll(CHSV(55U, 255U, 65U));
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    // minspeed 230 maxspeed 250 ============
    // minscale  40 maxscale  75 ============
    // красное вино hue > 0 & <=10
    // розовое вино hue > 10 & <=20
    // белое вино   hue > 20U & <= 40
    // шампанское   hue > 40U & <= 60

    deltaHue2 = 0U;                         // count для замедления смены цвета
    step = deltaValue;                      // чтообы при старте эффекта сразу покрасить лампу
    deltaHue = 1U;                          // direction | 0 hue-- | 1 hue++ |
    hue = 55U;                              // Start Color
    hue2 = 65U;                             // Brightness
    pcnt = 0;
  }

  deltaHue2++;
  // маштаб задает скорость изменения цвета 5 уровней
  divider = 5 - floor((modes[currentMode].Scale - 1) / 20);

  // возвращаем яркость для перехода к белому
  if (hue >= 10 && hue2 < 100U) {
    hue2++;
  }
  // уменьшаем яркость для красного вина
  if (hue < 10 && hue2 > 40U) {
    hue2--;
  }

  // изменение цвета вина -----
  if (deltaHue == 1U) {
    if (deltaHue2 % divider == 0) {
      hue++;
    }
  } else {
    if (deltaHue2 % divider == 0) {
      hue--;
    }
  }
  // --------

  // LOG.printf_P(PSTR("Wine | hue = %03d | Dir = %d | Brightness %03d | deltaHue2 %03d | divider %d | %d\n"), hue, deltaHue, hue2, deltaHue2, divider, step);

  // сдвигаем всё вверх -----------
  for (uint8_t x = 0U; x < WIDTH; x++) {
    for (uint8_t y = HEIGHT; y > 0U; y--) {
      drawPixelXY(x, y, getPixColorXY(x, y - 1U));
    }
  }

  if (hue > 40U) {
    // добавляем перляж для шампанского
    pcnt = random(0, WIDTH);
  } else {
    pcnt = 0;
  }

  // заполняем нижнюю строку с учетом перляжа
  for (uint8_t x = 0U; x < WIDTH; x++) {
    if ((x == pcnt) && (pcnt > 0)) {
      // с перляжем ------
      drawPixelXY(x, 0U, CHSV(hue, 150U, hue2 + 20U + random(0, 50U)));
    } else {
      drawPixelXY(x, 0U, CHSV(hue, 255U, hue2));
    }
  }

  // меняем направление изменения цвета вина от красного к шампанскому и обратно
  // в диапазоне шкалы HUE |0-60|
  if  (hue == 0U) {
    deltaHue = 1U;
  }
  if (hue == 60U) {
    deltaHue = 0U;
  }
  step++;
}

// ============== Swirl ================
//    © SlingMaster | by Alex Dovby
//              EFF_SWIRL
//--------------------------------------
void Swirl() {
  uint8_t divider;
  uint8_t lastHue;
  static const uint32_t colors[5][6] PROGMEM = {
    {CRGB::Blue, CRGB::DarkRed, CRGB::Aqua, CRGB::Magenta, CRGB::Gold, CRGB::Green },
    {CRGB::Yellow, CRGB::LemonChiffon, CRGB::LightYellow, CRGB::Gold, CRGB::Chocolate, CRGB::Goldenrod},
    {CRGB::Green, CRGB::DarkGreen, CRGB::LawnGreen, CRGB::SpringGreen, CRGB::Cyan, CRGB::Black },
    {CRGB::Blue, CRGB::DarkBlue, CRGB::MidnightBlue, CRGB::MediumSeaGreen, CRGB::MediumBlue, CRGB:: DeepSkyBlue },
    {CRGB::Magenta, CRGB::Red, CRGB::DarkMagenta, CRGB::IndianRed, CRGB::Gold, CRGB::MediumVioletRed }
  };
  uint32_t color;

  if (loadingFlag) {

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(50U + random8(190U), 250U);
    }
#endif
    loadingFlag = false;
    FastLED.clear();
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    step = deltaValue;                      // чтообы при старте эффекта сразу покрасить лампу
    deltaHue2 = 0U;                         // count для замедления смены цвета
    deltaHue = 0U;                          // direction | 0 hue-- | 1 hue++ |
    hue2 = 0U;                              // x
  }

  if (step >= deltaValue) {
    step = 0U;
  }
  divider = floor((modes[currentMode].Scale - 1) / 20); // маштаб задает смену палитры
  //  if (deltaValue > 50U && deltaHue2 == 0U) {
  //    hue = random8(6);                       // если низкая скорость меняем цвет после каждого витка
  //  }
  // задаем цвет и рисуем завиток --------
  color = colors[divider][hue];
  // drawPixelXY((hue2 + 1), (deltaHue2 - 1), 0x000000); // aded shadow
  drawPixelXY(hue2, deltaHue2, color);
  // LOG.printf_P(PSTR("Swirl | hue = %03d | x= %03d | y= %03d | divider %d | %d\n"), hue, hue2, deltaHue2, divider, step);
  // -------------------------------------

  hue2++;                     // x
  // два варианта custom_eff задается в сетапе лампы ----
  if (custom_eff == 1) {
    // blurScreen(beatsin8(5U, 20U, 5U));
    deltaHue2++;              // y
  } else {
    // blurScreen(10U);
    if (hue2 % 2 == 0) {
      deltaHue2++;            // y
    }
  }
  // -------------------------------------

  if  (hue2 > WIDTH) {
    hue2 = 0U;
  }

  if (deltaHue2 >= HEIGHT) {
    deltaHue2 = 0U;
    // new swirl ------------
    hue2 = random8(WIDTH - 2);
    // hue2 = hue2 + 2;
    // select new color -----
    hue = random8(6);

    if (lastHue == hue) {
      hue = hue + 1;
      if (hue >= 6) {
        hue = 0;
      }
    }
    lastHue = hue;
  }
  // blurScreen(beatsin8(5U, 20U, 5U));
  blurScreen(4U + random8(8));
  step++;
}

// -------------------------------------------
// for effect Ukraine
// -------------------------------------------
void drawCrest() {
  static const uint32_t data[9][5] PROGMEM = {
    {0x000000, 0x000000, 0xFFD700, 0x000000, 0x000000 },
    {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
    {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
    {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
    {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
    {0xFFD700, 0xFFD700, 0xFFD700, 0xFFD700, 0xFFD700 },
    {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
    {0x000000, 0xFFD700, 0xFFD700, 0xFFD700, 0x000000 },
    {0x000000, 0x000000, 0xFFD700, 0x000000, 0x000000 }
  };

  uint8_t posX = CENTER_X_MAJOR - 3;
  uint8_t posY = 9;
  uint32_t color;
  if (HEIGHT > 16) {
    posY = CENTER_Y_MINOR - 1;
  }
  FastLED.clear();
  for (uint8_t y = 0U; y < 9; y++) {
    for (uint8_t x = 0U; x < 5; x++) {
      color = data[y][x];
      drawPixelXY(posX + x, posY - y, color);
    }
  }
}

// ============== Ukraine ==============
//      © SlingMaster | by Alex Dovby
//              EFF_UKRAINE
//--------------------------------------
void Ukraine() {
  uint8_t divider;
  uint32_t color;
  static const uint16_t MAX_TIME = 500;
  uint16_t tMAX = 100;
  static const uint8_t timeout = 100;
  static const uint32_t colors[2][5] = {
    {CRGB::Blue, CRGB::MediumBlue, 0x0F004F, 0x02002F, 0x1F2FFF },
    {CRGB::Yellow, CRGB::Gold, 0x4E4000, 0xFF6F00, 0xFFFF2F }
  };

  // Initialization =========================
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(250U), 200U + random8(50U));
    }
#endif
    loadingFlag = false;
    drawCrest();
    // minspeed 200 maxspeed 250 ============
    // minscale   0 maxscale 100 ============
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    step = deltaValue;                        // чтообы при старте эффекта сразу покрасить лампу
    deltaHue2 = 0U;                           // count для замедления смены цвета
    deltaHue = 0U;                            // direction | 0 hue-- | 1 hue++ |
    hue2 = 0U;                                // Brightness
    ff_x = 1U;                                // counter
    tMAX = 100U;                              // timeout
  }
  divider = floor((modes[currentMode].Scale - 1) / 10); // маштаб задает режим рестарта
  tMAX = timeout + 100 * divider;

  if ((ff_x > timeout - 10) && (ff_x < timeout)) { // таймаут блокировки отрисовки флага
    if (ff_x < timeout - 5) {                  // размытие тризуба
      blurScreen(beatsin8(5U, 60U, 5U));
    } else {
      blurScreen(210U - ff_x);
    }
  }

  if (ff_x > tMAX) {
    if (divider == 0U) {                       // отрисовка тризуба только раз
      ff_x = 0U;
      tMAX += 20;
    } else {
      if (ff_x > tMAX + 100U * divider) {      // рестар эффект
        drawCrest();
        ff_x = 1U;
      }
    }
  }
  if ((ff_x != 0U) || (divider > 0)) {
    ff_x++;
  }

  // Flag Draw =============================
  if ((ff_x > timeout) || (ff_x == 0U))  {     // отрисовка флага
    if (step >= deltaValue) {
      step = 0U;
      hue2 = random8(WIDTH - 2);               // случайное смещение мазка по оси Y
      hue = random8(5);                        // flag color
      // blurScreen(dim8_raw(beatsin8(3, 64, 100)));
      // blurScreen(beatsin8(5U, 60U, 5U));
      // dimAll(200U);
    }
    if (step % 8 == 0 && modes[currentMode].Speed > 230) {
      blurScreen(beatsin8(5U, 5U, 72U));
    }
    hue2++;                                    // x
    deltaHue2++;                               // y

    if (hue2 >= WIDTH) {
      if (deltaHue2 > HEIGHT - 2 ) {           // если матрица высокая дорисовываем остальные мазки
        deltaHue2 = random8(5);                // изменяем положение по Y только отрисовав весь флаг
      }
      if (step % 2 == 0) {
        hue2 = 0U;
      } else {
        hue2 = random8(WIDTH);                 // смещение первого мазка по оси X
      }
    }

    if (deltaHue2 >= HEIGHT) {
      deltaHue2 = 0U;
      if (deltaValue > 200U) {
        hue = random8(5);                      // если низкая скорость меняем цвет после каждого витка
      }
    }

    if (deltaHue2 > floor(HEIGHT / 2) - 1) {    // меняем цвет для разных частей флага
      color = colors[0][hue];
    } else {
      color = colors[1][hue];
    }

    // LOG.printf_P(PSTR("color = %08d | hue2 = %d | speed = %03d | custom_eff = %d\n"), color, hue2, deltaValue, custom_eff);
    drawPixelXY(hue2, deltaHue2, color);
    // ----------------------------------
    step++;
  }
}

// ============ Oil Paints ==============
//      © SlingMaster | by Alex Dovby
//              EFF_PAINT
//           Масляные Краски
//---------------------------------------
void OilPaints() {

  uint8_t divider;
  uint8_t entry_point;
  uint16_t value;
  uint16_t max_val;
  if (loadingFlag) {

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                          scale | speed 210
      setModeSettings(1U + random8(252U), 1 + random8(219U));
    }
#endif
    loadingFlag = false;
    FastLED.clear();
    // blurScreen(beatsin8(5U, 50U, 5U));
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    step = deltaValue;                    // чтообы при старте эффекта сразу покрасить лампу
    hue = floor(21.25 * (random8(11) + 1)); // next color
    deltaHue = hue - 22;                  // last color
    deltaHue2 = 80;                       // min bright
    max_val = pow(2, WIDTH);
    //    for ( int i = WIDTH; i < (NUM_LEDS - WIDTH); i++) {
    //      leds[i] = CHSV(120U, 24U, 64U);
    //    }
  }

  if (step >= deltaValue) {
    step = 0U;
    // LOG.printf_P(PSTR("%03d | log: %f | val: %03d\n\r"), modes[currentMode].Brightness, log(modes[currentMode].Brightness), deltaHue2);
  }

  // Create Oil Paints --------------
  // выбираем краски  ---------------
  if (step % CENTER_Y_MINOR == 0) {
    divider = floor((modes[currentMode].Scale - 1) / 10);             // маштаб задает диапазон изменения цвета
    deltaHue = hue;                                                   // set last color
    hue += 6 * divider;                                               // new color
    hue2 = 255;                                                       // restore brightness
    deltaHue2 = 80 - floor(log(modes[currentMode].Brightness) * 6);   // min bright
    entry_point = random8(WIDTH);                                     // start X position
    trackingObjectHue[entry_point] = hue;                             // set start position
    drawPixelXY(entry_point,  HEIGHT - 2, CHSV(hue, 255U, 255U));
    // !!! ********
    if (custom_eff == 1) {
      drawPixelXY(entry_point + 1,  HEIGHT - 3, CHSV(hue + 30, 255U, 255U));
    }
    // ************
    // LOG.printf_P(PSTR("BR %03d | SP %03d | SC %03d | hue %03d\n\r"), modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale, hue);
  }

  // формируем форму краски, плавно расширяя струю ----
  if (random8(3) == 1) {
    // LOG.println("<--");
    for (uint8_t x = 1U; x < WIDTH; x++) {
      if (trackingObjectHue[x] == hue) {
        trackingObjectHue[x - 1] = hue;
        break;
      }
    }
  } else {
    // LOG.println("-->");
    for (uint8_t x = WIDTH - 1; x > 0U ; x--) {
      if (trackingObjectHue[x] == hue) {
        trackingObjectHue[x + 1] = hue;
        break;
      }
      // LOG.printf_P(PSTR("x = %02d | value = %03d | hue = %03d \n\r"), x, trackingObjectHue[x], hue);
    }
  }
  // LOG.println("------------------------------------");

  // выводим сформированную строку --------------------- максимально яркую в момент смены цвета
  for (uint8_t x = 0U; x < WIDTH; x++) {
    //                                                                                set color  next |    last  |
    drawPixelXY(x,  HEIGHT - 1, CHSV(trackingObjectHue[x], 255U, (trackingObjectHue[x] == hue) ? hue2 : deltaHue2));
  }
  //  LOG.println("");
  // уменьшаем яркость для следующих строк
  if ( hue2 > (deltaHue2 + 16)) {
    hue2 -= 16U;
  }
  // сдвигаем неравномерно поток вниз ---
  value = random16(max_val);
  //LOG.printf_P(PSTR("value = %06d | "), value);
  for (uint8_t x = 0U; x < WIDTH; x++) {
    if ( bitRead(value, x ) == 0) {
      //LOG.print (" X");
      for (uint8_t y = 0U; y < HEIGHT - 1; y++) {
        drawPixelXY(x, y, getPixColorXY(x, y + 1U));
      }
    }
  }
  // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
  // -------------------------------------

  step++;
}


// ========== Botswana Rivers ===========
//      © SlingMaster | by Alex Dovby
//              EFF_RIVERS
//            Реки Ботсваны

//---------------------------------------
void flora() {
  uint32_t  FLORA_COLOR = 0x2F1F00;
  uint8_t posX =  floor(CENTER_X_MINOR - WIDTH * 0.3);
  uint8_t h =  random8(HEIGHT - 6U) + 4U;
  DrawLine(posX + 1, 1U, posX + 1, h - 1, 0x000000);
  DrawLine(posX + 2, 1U, posX + 2, h, FLORA_COLOR );
  drawPixelXY(posX + 2, h - random8(floor(h * 0.5)), random8(2U) == 1 ? 0xFF00E0 :  random8(2U) == 1 ? 0xFFFF00 : 0x00FF00);
  drawPixelXY(posX + 1, h - random8(floor(h * 0.25)), random8(2U) == 1 ? 0xFF00E0 : 0xFFFF00);
  if (random8(2U) == 1) {
    drawPixelXY(posX + 1, floor(h * 0.5), random8(2U) == 1 ? 0xEF001F :  0x9FFF00);
  }
  h =  floor(h * 0.65);
  if (WIDTH > 8) {
    DrawLine(posX - 1, 1U, posX - 1, h - 1, 0x000000);
  }
  DrawLine(posX, 1U, posX, h, FLORA_COLOR);
  drawPixelXY(posX, h - random8(floor(h * 0.5)), random8(2U) == 1 ? 0xFF00E0 : 0xFFFF00);
}

//---------------------------------------
void animeBobbles() {
  // сдвигаем всё вверх ----
  for (uint8_t x = CENTER_X_MAJOR; x < WIDTH; x++) {
    for (uint8_t y = HEIGHT; y > 0U; y--) {
      if (getPixColorXY(x, y - 1) == 0xFFFFF7) {
        drawPixelXY(x, y, 0xFFFFF7);
        drawPixelXY(x, y - 1, getPixColorXY(0, y - 1));
      }
    }
  }
  // ----------------------
  if ( step % 4 == 0) {
    drawPixelXY(CENTER_X_MAJOR + random8(5), 0U, 0xFFFFF7);
    if ( step % 12 == 0) {
      drawPixelXY(CENTER_X_MAJOR + 2 + random8(3), 0U, 0xFFFFF7);
    }
  }
}

//---------------------------------------
void createScene(uint8_t idx) {
  switch (idx) {
    case 0:     // blue green ------
      gradientDownTop(floor((HEIGHT - 1) * 0.5), CHSV(96, 255, 100), HEIGHT, CHSV(160, 255, 255));
      gradientDownTop(0, CHSV(96, 255, 255), CENTER_Y_MINOR, CHSV(96, 255, 100));
      break;
    case 1:     // aquamarine green
      gradientDownTop(floor((HEIGHT - 1) * 0.3), CHSV(96, 255, 100), HEIGHT, CHSV(130, 255, 220));
      gradientDownTop(0, CHSV(96, 255, 255), floor(HEIGHT * 0.3), CHSV(96, 255, 100));
      break;
    case 2:     // blue aquamarine -
      gradientDownTop(floor((HEIGHT - 1) * 0.5), CHSV(170, 255, 100), HEIGHT, CHSV(160, 255, 200));
      gradientDownTop(0, CHSV(100, 255, 255), CENTER_Y_MINOR, CHSV(170, 255, 100));
      break;
    case 3:     // yellow green ----
      gradientDownTop(floor((HEIGHT - 1) * 0.5), CHSV(95, 255, 55), HEIGHT, CHSV(70, 255, 200));
      gradientDownTop(0, CHSV(95, 255, 255), CENTER_Y_MINOR, CHSV(100, 255, 55));
      break;
    case 4:     // sea green -------
      gradientDownTop(floor((HEIGHT - 1) * 0.3), CHSV(120, 255, 55), HEIGHT, CHSV(175, 255, 200));
      gradientDownTop(0, CHSV(120, 255, 255), floor(HEIGHT * 0.3), CHSV(120, 255, 55));
      break;
    default:
      gradientDownTop(floor((HEIGHT - 1) * 0.25), CHSV(180, 255, 85), HEIGHT, CHSV(160, 255, 200));
      gradientDownTop(0, CHSV(80, 255, 255), floor(HEIGHT * 0.25), CHSV(180, 255, 85));
      break;
  }
  flora();
}

//---------------------------------------
void createSceneM(uint8_t idx) {
  switch (idx) {
    case 0:     // blue green ------
      gradientVertical(0, CENTER_Y_MINOR, WIDTH, HEIGHT, 96, 150, 100, 255, 255U);
      gradientVertical(0, 0, WIDTH, CENTER_Y_MINOR, 96, 96, 255, 100, 255U);
      break;
    case 1:     // aquamarine green
      gradientVertical(0, floor(HEIGHT  * 0.3), WIDTH, HEIGHT, 96, 120, 100, 220, 255U);
      gradientVertical(0, 0, WIDTH, floor(HEIGHT  * 0.3), 96, 96, 255, 100, 255U);
      break;
    case 2:     // blue aquamarine -
      gradientVertical(0, CENTER_Y_MINOR, WIDTH, HEIGHT, 170, 160, 100, 200, 255U);
      gradientVertical(0, 0, WIDTH, CENTER_Y_MINOR, 100, 170, 255, 100, 255U);
      break;
    case 3:     // yellow green ----
      gradientVertical(0, CENTER_Y_MINOR, WIDTH, HEIGHT, 95, 65, 55, 200, 255U);
      gradientVertical(0, 0, WIDTH, CENTER_Y_MINOR, 95, 100, 255, 55, 255U);
      break;
    case 4:     // sea green -------
      gradientVertical(0, floor(HEIGHT  * 0.3), WIDTH, HEIGHT, 120, 160, 55, 200, 255U);
      gradientVertical(0, 0, WIDTH, floor(HEIGHT  * 0.3), 120, 120, 255, 55, 255U);
      break;
    default:
      drawRec(0, 0, WIDTH, HEIGHT, 0x000050);
      break;
  }
  flora();
}

//---------------------------------------
void BotswanaRivers() {
  // альтернативный градиент для ламп собраных из лент с вертикальной компоновкой
  // для корректной работы ALT_GRADIENT = true
  // для ламп из лент с горизонтальной компоновкой и матриц ALT_GRADIENT = false
  // ALT_GRADIENT = false более производительный и более плавная растяжка
  //------------------------------------------------------------------------------
  static const bool ALT_GRADIENT = false;

  uint8_t divider;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                          scale | speed 210
      setModeSettings(1U + random8(252U), 20 + random8(180U));
    }
#endif
    loadingFlag = false;
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    step = deltaValue;                                          // чтообы при старте эффекта сразу покрасить лампу
    divider = floor((modes[currentMode].Scale - 1) / 20);       // маштаб задает смену палитры воды
    if (ALT_GRADIENT) {
      createSceneM(divider);
    } else {
      createScene(divider);
    }
  }

  if (step >= deltaValue) {
    step = 0U;
  }

  // restore scene after power on ---------
  if (getPixColorXY(0U, HEIGHT - 2) == CRGB::Black) {
    if (ALT_GRADIENT) {
      createSceneM(divider);
    } else {
      createScene(divider);
    }
  }

  // light at the bottom ------------------
  if (!ALT_GRADIENT) {
    if (step % 2 == 0) {
      if (random8(6) == 1) {
        //fill_gradient(leds, NUM_LEDS - WIDTH, CHSV(96U, 255U, 200U), NUM_LEDS, CHSV(50U, 255U, 255U), SHORTEST_HUES);
        if (STRIP_DIRECTION < 2) {
          fill_gradient(leds, 0, CHSV(96U, 255U, 190U), random8(WIDTH + random8(6)), CHSV(90U, 200U, 255U), SHORTEST_HUES);
        } else {
          fill_gradient(leds, NUM_LEDS - random8(WIDTH + random8(6)), CHSV(96U, 255U, 190U), NUM_LEDS, CHSV(90U, 200U, 255U), SHORTEST_HUES);
        }
      } else {
        //fill_gradient(leds, NUM_LEDS - WIDTH, CHSV(50U, 128U, 255U), NUM_LEDS, CHSV(90U, 255U, 180U), SHORTEST_HUES);
        if (STRIP_DIRECTION < 2) {
          fill_gradient(leds, 0, CHSV(85U, 128U, 255U), random8(WIDTH), CHSV(90U, 255U, 180U), SHORTEST_HUES);
        } else {
          fill_gradient(leds, NUM_LEDS - random8(WIDTH), CHSV(85U, 128U, 255U), NUM_LEDS, CHSV(90U, 255U, 180U), SHORTEST_HUES);
        }
      }
    }
  }

  // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
  // -------------------------------------
  animeBobbles();
  if (custom_eff == 1) {
    blurRows(leds, WIDTH, 3U, 10U);
    // blurScreen(beatsin8(0U, 5U, 0U));
  }
  step++;
}


// ============ Watercolor ==============
//      © SlingMaster | by Alex Dovby
//            EFF_WATERCOLOR
//               Акварель
//---------------------------------------
void SmearPaint(uint8_t obj[trackingOBJECT_MAX_COUNT]) {
  uint8_t divider;
  int temp;
  static const uint32_t colors[6][8] PROGMEM = {
    {0x2F0000,  0xFF4040, 0x6F0000, 0xAF0000, 0xff5f00, CRGB::Red, 0x480000, 0xFF0030},
    {0x002F00, CRGB::LawnGreen, 0x006F00, 0x00AF00, CRGB::DarkMagenta, 0x00FF00, 0x004800, 0x00FF30},
    {0x002F1F, CRGB::DarkCyan, 0x00FF7F, 0x007FFF, 0x20FF5F, CRGB::Cyan, 0x004848, 0x7FCFCF },
    {0x00002F, 0x5030FF, 0x00006F, 0x0000AF, CRGB::DarkCyan, 0x0000FF, 0x000048, 0x5F5FFF},
    {0x2F002F, 0xFF4040, 0x6F004A, 0xFF0030, CRGB::DarkMagenta, CRGB::Magenta, 0x480048, 0x3F00FF},
    {CRGB::Blue, CRGB::Red, CRGB::Gold, CRGB::Green, CRGB::DarkCyan, CRGB::DarkMagenta, 0x000000, 0xFF7F00 }
  };
  if (trackingObjectHue[5] == 1) {  // direction >>>
    obj[1]++;
    if (obj[1] >= obj[2]) {
      trackingObjectHue[5] = 0;     // swap direction
      obj[3]--;                     // new line
      if (step % 2 == 0) {
        obj[1]++;
      } else {
        obj[1]--;
      }

      obj[0]--;
    }
  } else {                          // direction <<<
    obj[1]--;
    if (obj[1] <= (obj[2] - obj[0])) {
      trackingObjectHue[5] = 1;     // swap direction
      obj[3]--;                     // new line
      if (obj[0] >= 1) {
        temp = obj[0] - 1;
        if (temp < 0) {
          temp = 0;
        }
        obj[0] = temp;
        obj[1]++;
      }
    }
  }

  if (obj[3] == 255) {
    deltaHue = 255;
  }

  divider = floor((modes[currentMode].Scale - 1) / 16.7);
  if ( (obj[1] >= WIDTH) || (obj[3] == obj[4]) ) {
    // deltaHue value == 255 activate -------
    // set new parameter for new smear ------
    deltaHue = 255;
  }
  drawPixelXY(obj[1], obj[3], colors[divider][hue]);

  // alternative variant without dimmer effect
  // uint8_t h = obj[3] - obj[4];
  // uint8_t br = 266 - 12 * h;
  // if (h > 0) {
  // drawPixelXY(obj[1], obj[3], makeDarker(colors[divider][hue], br));
  // } else {
  // drawPixelXY(obj[1], obj[3], makeDarker(colors[divider][hue], 240));
  // }
}



//---------------------------------------
void Watercolor() {
  // #define DIMSPEED (254U - 500U / WIDTH / HEIGHT)
  uint8_t divider;
  if (loadingFlag) {

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                          scale | speed 250
      setModeSettings(1U + random8(252U), 1 + random8(250U));
    }
#endif
    loadingFlag = false;
    FastLED.clear();
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    step = deltaValue;                    // чтообы при старте эффекта сразу покрасить лампу
    hue = 0;
    deltaHue = 255;                       // last color
    trackingObjectHue[1] = floor(WIDTH * 0.25);
    trackingObjectHue[3] = floor(HEIGHT * 0.25);
  }

  if (step >= deltaValue) {
    step = 0U;
    // LOG.printf_P(PSTR("%03d | log: %f | val: %03d | divider: %d \n\r"), modes[currentMode].Brightness, log(modes[currentMode].Brightness), deltaHue2, divider);
  }

  // ******************************
  // set random parameter for smear
  // ******************************
  if (deltaHue == 255) {

    trackingObjectHue[0] = 4 + random8(floor(WIDTH * 0.25));                // width
    trackingObjectHue[1] = random8(WIDTH - trackingObjectHue[0]);           // x
    int temp =  trackingObjectHue[1] + trackingObjectHue[0];
    if (temp >= (WIDTH - 1)) {
      temp = WIDTH - 1;
      if (trackingObjectHue[1] > 1) {
        trackingObjectHue[1]--;
      } else {
        trackingObjectHue[1]++;
      }
    }
    trackingObjectHue[2] = temp;                                            // x end
    trackingObjectHue[3] = 3 + random8(HEIGHT - 4);                         // y
    temp = trackingObjectHue[3] - random8(3) - 3;
    if (temp <= 0) {
      temp = 0;
    }
    trackingObjectHue[4] = temp;                                            // y end
    trackingObjectHue[5] = 1;
    divider = floor((modes[currentMode].Scale - 1) / 16.7);                 // маштаб задает смену палитры
    hue = random8(8);
    //    if (step % 127 == 0) {
    //      LOG.printf_P(PSTR("BR %03d | SP %03d | SC %03d | divider %d | [ %d ]\n\r"), modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale, divider, hue);
    //    }
    hue2 = 255;
    deltaHue = 0;
  }
  // ******************************
  SmearPaint(trackingObjectHue);

  // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
  // -------------------------------------
  //  if (custom_eff == 1) {
  // dimAll(DIMSPEED);
  if (step % 2 == 0) {
    blurScreen(beatsin8(1U, 1U, 6U));
    // blurRows(leds, WIDTH, 3U, 10U);
  }
  //  }
  step++;
}

// =========== FeatherCandle ============
//         адаптация © SottNick
//    github.com/mnemocron/FeatherCandle
//      modify & design © SlingMaster
//           EFF_FEATHER_CANDLE
//                Свеча
//---------------------------------------
#include "data7x15flip.h"                       // FeatherCandle animation data
const uint8_t  level = 160;
const uint8_t  low_level = 110;
const uint8_t *ptr  = anim;                     // Current pointer into animation data
const uint8_t  w    = 7;                        // image width
const uint8_t  h    = 15;                       // image height
uint8_t        img[w * h];                      // Buffer for rendering image
uint8_t        deltaX = CENTER_X_MAJOR - 3;     // position img
uint8_t last_brightness;
void FeatherCandleRoutine() {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
  if (selectedSettings) {
    // brightness | scale | speed
    // { 21, 220,  40}
    setModeSettings(1U + random8(99U), 190U + random8(65U));
  }
#endif
  if (loadingFlag) {
    FastLED.clear();
    hue = 0;
    trackingObjectState[0] = low_level;
    trackingObjectState[1] = low_level;
    trackingObjectState[2] = low_level;
    trackingObjectState[4] = CENTER_X_MAJOR;
    loadingFlag = false;
  }

  uint8_t a = pgm_read_byte(ptr++);     // New frame X1/Y1
  if (a >= 0x90) {                      // EOD marker? (valid X1 never exceeds 8)
    ptr = anim;                         // Reset animation data pointer to start
    a   = pgm_read_byte(ptr++);         // and take first value
  }
  uint8_t x1 = a >> 4;                  // X1 = high 4 bits
  uint8_t y1 = a & 0x0F;                // Y1 = low 4 bits
  a  = pgm_read_byte(ptr++);            // New frame X2/Y2
  uint8_t x2 = a >> 4;                  // X2 = high 4 bits
  uint8_t y2 = a & 0x0F;                // Y2 = low 4 bits

  // Read rectangle of data from anim[] into portion of img[] buffer
  for (uint8_t y = y1; y <= y2; y++)
    for (uint8_t x = x1; x <= x2; x++) {
      img[y * w + x] = pgm_read_byte(ptr++);
    }
  int i = 0;
  uint8_t color = (modes[currentMode].Scale - 1U) * 2.57;



  // draw flame -------------------
  for (uint8_t y = 1; y < h; y++) {
    if ((HEIGHT < 15) || (WIDTH < 9)) {
      // for small matrix -----
      if (y % 2 == 0) {
        leds[XY(CENTER_X_MAJOR - 1, 7)] = CHSV(color, 255U, 55 + random8(200));
        leds[XY(CENTER_X_MAJOR, 6)] = CHSV(color, 255U, 160 + random8(90));
        leds[XY(CENTER_X_MAJOR + 1, 6)] = CHSV(color, 255U, 205 + random8(50));
        leds[XY(CENTER_X_MAJOR - 1, 5)] = CHSV(color, 255U, 155 + random8(100));
        leds[XY(CENTER_X_MAJOR, 5)] = CHSV(color - 10U , 255U, 120 + random8(130));
        leds[XY(CENTER_X_MAJOR, 4)] = CHSV(color - 10U , 255U, 100 + random8(120));
        DrawLine(0, 2U, WIDTH - 1, 2U, 0x000000);
      }
    } else {
      for (uint8_t x = 0; x < w; x++) {
        uint8_t brightness = img[i];
        leds[XY(deltaX + x, y)] = CHSV(brightness > 240 ? color : color - 10U , 255U, brightness);
        i++;
      }
    }

    // draw body FeatherCandle ------
    if (y <= 3) {
      if (y % 2 == 0) {
        gradientVertical(0, 0, WIDTH, 2, color, color, 48, 128, 20U);
      }
    }

    // drops of wax move -------------
    switch (hue ) {
      case 0:
        if (trackingObjectState[0] < level) {
          trackingObjectState[0]++;
        }
        break;
      case 1:
        if (trackingObjectState[0] > low_level) {
          trackingObjectState[0] --;
        }
        if (trackingObjectState[1] < level) {
          trackingObjectState[1] ++;
        }
        break;
      case 2:
        if (trackingObjectState[1] > low_level) {
          trackingObjectState[1] --;
        }
        if (trackingObjectState[2] < level) {
          trackingObjectState[2] ++;
        }
        break;
      case 3:
        if (trackingObjectState[2] > low_level) {
          trackingObjectState[2] --;
        } else {
          hue++;
          // set random position drop of wax
          trackingObjectState[4] = CENTER_X_MAJOR - 3 + random8(6);
        }
        break;
    }

    if (hue > 3) {
      hue++;
    } else {
      // LOG.printf_P(PSTR("[0] = %03d | [1] = %03d | [2] = %03d \n\r"), trackingObjectState[0], trackingObjectState[1], trackingObjectState[2]);
      if (hue < 2) {
        leds[XY(trackingObjectState[4], 2)] = CHSV(50U, 20U, trackingObjectState[0]);
      }
      if ((hue == 1) || (hue == 2)) {
        leds[XY(trackingObjectState[4], 1)] = CHSV(50U, 15U, trackingObjectState[1]); // - 10;
      }
      if (hue > 1) {
        leds[XY(trackingObjectState[4], 0)] = CHSV(50U, 5U, trackingObjectState[2]); // - 20;
      }
    }
  }

  // next -----------------
  if ((trackingObjectState[0] == level) || (trackingObjectState[1] == level) || (trackingObjectState[2] == level)) {
    hue++;
  }
}

// ============= Hourglass ==============
//             © SlingMaster
//             EFF_HOURGLASS
//             Песочные Часы
//---------------------------------------

void Hourglass() {
  uint8_t divider;
  uint8_t h;
  float SIZE = 0.43;
  const uint8_t topPos  = HEIGHT - floor(HEIGHT * SIZE);
  const uint8_t route = HEIGHT - floor(HEIGHT * SIZE) + 1;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                          scale | speed 210
      setModeSettings(15U + random8(225U), random8(255U));
    }
#endif
    loadingFlag = false;
    deltaValue = 255U - modes[currentMode].Speed + 1U;
    step = deltaValue;                                          // чтообы при старте эффекта сразу покрасить лампу
    deltaHue2 = 0;
    h = floor(NUM_LEDS * SIZE / WIDTH) * WIDTH;
    FastLED.clear();
    hue2 = 0;
  }
  if (hue2 == 0) {


    divider = modes[currentMode].Scale;
    if (divider < 10) {
      hue = step;
    } else {
      hue = divider * 2.55;
    }

    if (STRIP_DIRECTION < 2) {
      // STRIP_DIRECTION to UP ========
      fill_gradient(leds, NUM_LEDS - h, CHSV(hue, 255, 254), NUM_LEDS - deltaHue2, CHSV(hue, 255, 30), SHORTEST_HUES);
      fill_gradient(leds, 0, CHSV(hue, 255, 254), deltaHue2, CHSV(hue, 255, 30), SHORTEST_HUES);
    } else {
      // STRIP_DIRECTION to DOWN ======
      fill_gradient(leds, deltaHue2, CHSV(hue, 255, 30), h, CHSV(hue, 255, 254), SHORTEST_HUES);
      fill_gradient(leds, NUM_LEDS - deltaHue2, CHSV(hue, 255, 30), NUM_LEDS, CHSV(hue, 255, 254), SHORTEST_HUES);
    }

    if (custom_eff == 1) {
      if (deltaHue2 < WIDTH) {
        blurScreen(8U);
      }
    }
    deltaHue2++;
    if (deltaHue2 > h) {
      deltaHue2 = 0;
      hue2 = 1;
      // борьба с артифактом -----
      DrawLine(0, topPos, WIDTH - 1, topPos, 0x000000);
    }
    step++;
  }

  // сдвигаем всё вверх -----------
  // имитация переворота песочных часов
  if (hue2 > 0) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      for (uint8_t y = HEIGHT; y > 0U; y--) {
        drawPixelXY(x, y, getPixColorXY(x, y - 1U));
        drawPixelXY(x, y - 1, 0x000000);
      }
    }
    hue2++;
    if (hue2 > route) {
      hue2 = 0;
    }
  }
}

// ============== Spectrum ==============
//             © SlingMaster
//              EFF_SPECTRUM
//                Spectrum
//---------------------------------------

void  Spectrum() {
  uint8_t divider;
  uint8_t deltaY;
  uint8_t posX;

  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed 210
      setModeSettings(random8(100U), 180U + random8(75U));
    }
#endif
    loadingFlag = false;
    step = deltaValue;
    hue = 120;
    hue2 = hue + 30;
    posX = 0;
    deltaHue2 = 0;
    divider = floor(modes[currentMode].Scale / 12.5);
    if (divider % 2 == 0) {
      blurScreen(dim8_raw(beatsin8(3, 64, 3)));
      DrawLine(3, 0, 4, 0 , 0x00FF00);
    } else {
      FastLED.clear();
      DrawLine(3, 0, 4, 0 , 0xFF3F00);
    }
  }

  deltaHue2++;
  if (deltaHue2 >= WIDTH) {
    deltaHue2 = 0;
  }
  divider = floor(modes[currentMode].Scale / 12.5);
  posX = deltaHue2;
  deltaY = floor((HEIGHT - validMinMax(random8(HEIGHT - 1), 3, HEIGHT)) * 0.5);

  switch (divider ) {
    case 0:
    case 1:
      if (step % 32 == 0) {
        hue++;
      }
      break;
    case 2:
    case 3:
      if (step % 2 == 0) {
        hue++;
      }
      break;
    case 4:
    case 5:
      if (step == 0) {
        hue += 30;
      }
      break;
    default:
      if (step == 0) {
        hue += 10;
        hue2 -= 10;
      }
      break;
  }

  if (divider < 6) {
    if (hue >= 225) {
      hue2 = 224;
    } else {
      hue2 = hue + 30;
    }
  }

  // if (step % 32 == 0) {
  //   LOG.printf_P(PSTR("divider = %01d | hue = %03d | hue2 = %03d | scale = %03d | BR = %03d\n\r"), divider, hue, hue2, modes[currentMode].Scale, (deltaValue));
  // }

  deltaValue = random8(128);          // delta brightness
  if (posX % 2 == 0) {
    if (divider < 6) {
      gradientVertical(posX, CENTER_Y_MINOR, posX + 1, CENTER_Y_MINOR + random8(2), hue + 15, hue2 - 15, 128 + deltaValue, 255 - deltaValue, 255U);
    }
    DrawLine(posX, HEIGHT - deltaY, posX, HEIGHT - 1, 0x000000);
    DrawLine(posX, 0, posX, deltaY , 0x000000);
  } else {
    gradientVertical(posX, deltaY, posX + 1, HEIGHT - deltaY, hue, hue2, 128 + deltaValue, 255 - deltaValue, 255U);
  }

  if (divider % 2 == 0) {
    blurScreen(6U);
  } else {
    DrawLine(posX, HEIGHT - deltaY, posX, HEIGHT - 1, 0x000000);
    DrawLine(posX, 0, posX, deltaY , 0x000000);
    dimAll(255 - posX);
  }
  step++;
}

// ============ Lotus Flower ============
//             © SlingMaster
//               EFF_LOTUS
//             Цветок Лотоса
//---------------------------------------
void  Flower() {
  uint8_t br;
  if (step < 128) {
    br = 255 - step;  // 255 >> 128
  } else {
    br = step;        // 128 >> 255
  }
  if (modes[currentMode].Scale > 10) {
    dimAll(90);
    hue = floor(modes[currentMode].Scale * 1.9) + floor(br / 4);
  } else {
    FastLED.clear();
    hue = step;
  }
  if (step > 190) {
    hue2 = validMinMax(hue - 64 + floor(br / 4), 190, 250);
  } else {
    hue2 = hue + 64 - floor(br / 4);
  }

  for (uint8_t x = 0U ; x < WIDTH ; x++) {
    if (x % 6 == 0) {
      gradientVertical( x - deltaValue, 2U, x + 1 - deltaValue, HEIGHT - floor((255 - br) / 24) - random8(2), hue, hue2, 255, floor(br * 0.5), 255U);
      gradientVertical( x + 3U - deltaValue, 0U, x + 4U - deltaValue, HEIGHT - floor(br / 24) + random8(3), hue, hue2, 255, floor((255 - br * 0.5)), 255U);
      drawPixelXY(x - deltaValue, 0, 0x005F00);
    }
  }
}

//---------------------------------------
void LotusFlower() {
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(100U), 50U + random8(190U));
    }
#endif
    loadingFlag = false;
    deltaValue = 0;
    step = deltaValue;
    deltaValue = 0;
    hue = 120;
    hue2 = 0;
    deltaHue = 0;
    deltaHue2 = 0;
    FastLED.clear();
  }

  Flower();
  if (deltaHue == 0) {               // rotation
    deltaValue--;
    if (deltaValue <= 0) {
      deltaValue = 3;
    }
  } else {
    deltaValue++;
    if (deltaValue >= 3) {
      deltaValue = 0;
    }
  }
  deltaHue2++;
  if (deltaHue2 >= 18) {           // swap direction rotate
    deltaHue2 = 0;
    deltaHue = (deltaHue == 0) ? 1 : 0;
  }
  step++;
}

// =========== Christmas Tree ===========
//             © SlingMaster
//           EFF_CHRISTMAS_TREE
//            Новогодняя Елка
//---------------------------------------
void VirtualSnow() {
  for (uint8_t x = 0U; x < WIDTH; x++) {
    for (uint8_t y = 0U; y < HEIGHT - 1; y++) {
      noise3d[0][x][y] = noise3d[0][x][y + 1];
      if (noise3d[0][x][y] > 0) {
        drawPixelXY(x, y, CHSV(170, 5U, 127 + random8(128)));
      }
    }
  }
  uint8_t posX = random(WIDTH);
  for (uint8_t x = 0U; x < WIDTH; x++) {
    // заполняем случайно верхнюю строку
    if (posX == x) {
      if (step % 3 == 0) {
        noise3d[0][x][HEIGHT - 1U] = 1;
      } else {
        noise3d[0][x][HEIGHT - 1U]  = 0;
      }
    } else {
      noise3d[0][x][HEIGHT - 1U]  = 0;
    }
  }
}

//---------------------------------------
void GreenTree() {
  hue = floor(step / 32) * 32U;
  FastLED.clear();
  for (uint8_t x = 0U; x < WIDTH + 1 ; x++) {
    if (x % 8 == 0) {
      if (modes[currentMode].Scale < 60) {
        // nature -----
        DrawLine(x - 1U - deltaValue, floor(HEIGHT * 0.70), x + 1U - deltaValue, floor(HEIGHT * 0.70), 0x002F00);
        DrawLine(x - 1U - deltaValue, floor(HEIGHT * 0.55), x + 1U - deltaValue, floor(HEIGHT * 0.55), 0x004F00);
        DrawLine(x - 2U - deltaValue, floor(HEIGHT * 0.35), x + 2U - deltaValue, floor(HEIGHT * 0.35), 0x005F00);
        DrawLine(x - 2U - deltaValue, floor(HEIGHT * 0.15), x + 2U - deltaValue, floor(HEIGHT * 0.15), 0x007F00);
        drawPixelXY(x - 3U - deltaValue, floor(HEIGHT * 0.15), 0x001F00);
        drawPixelXY(x + 3U - deltaValue, floor(HEIGHT * 0.15), 0x001F00);
        gradientVertical( x - deltaValue, 0U, x + 1U - deltaValue, HEIGHT, 96U, 97U, 190U, 32U, 255U);
      } else {
        // holiday -----
        //        drawPixelXY(x - 1 - deltaValue, CENTER_Y_MINOR, CHSV(floor(random8(255) / 32) * 32U, 255U, 128 + random8(128)));
        //        drawPixelXY(x + 1 - deltaValue, CENTER_Y_MINOR, CHSV(floor(random8(90) / 32) * 32U, 255U, 128 + random8(128)));
        //        drawPixelXY(x - deltaValue, floor(HEIGHT * 0.2), CHSV(floor(random8(255) / 32) * 32U, 255U, 128 + random8(128)));
        //        drawPixelXY(x - 2 - deltaValue, floor(HEIGHT * 0.25), CHSV(floor(random8(90) / 32) * 32U, 255U, 128 + random8(128)));
        //        drawPixelXY(x + 2 - deltaValue, floor(HEIGHT * 0.25), CHSV(floor(random8(255) / 32) * 32U, 255U, 128 + random8(128)));
        //        drawPixelXY(x - deltaValue, 0U, CHSV(96U, 255U, 200U));

        drawPixelXY(x - 1 - deltaValue, floor(HEIGHT * 0.6), CHSV(step, 255U, 128 + random8(128)));
        drawPixelXY(x + 1 - deltaValue, floor(HEIGHT * 0.6), CHSV(step, 255U, 128 + random8(128)));

        drawPixelXY(x - deltaValue, floor(HEIGHT * 0.4), CHSV(step, 255U, 200U));

        drawPixelXY(x - deltaValue, floor(HEIGHT * 0.2), CHSV(step, 255U, 190 + random8(65)));
        drawPixelXY(x - 2 - deltaValue, floor(HEIGHT * 0.25), CHSV(step, 255U, 96 + random8(128)));
        drawPixelXY(x + 2 - deltaValue, floor(HEIGHT * 0.25), CHSV(step, 255U, 96 + random8(128)));

        drawPixelXY(x - 2 - deltaValue, 1U, CHSV(step, 255U, 200U));
        drawPixelXY(x - deltaValue, 0U, CHSV(step, 255U, 250U));
        drawPixelXY(x + 2 - deltaValue, 1U, CHSV(step, 255U, 200U));

        gradientVertical( x - deltaValue, floor(HEIGHT * 0.75), x + 1U - deltaValue, HEIGHT,  hue, hue + 2, 250U, 0U, 128U);
      }
    }
  }
}

//---------------------------------------
void ChristmasTree() {
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(100U), 10U + random8(128U));
    }
#endif
    loadingFlag = false;
    deltaValue = 0;
    step = deltaValue;
    FastLED.clear();
  }

  GreenTree();
  if (modes[currentMode].Scale < 60) {
    VirtualSnow();
  }
  if (modes[currentMode].Scale > 30) {
    deltaValue++;
  }
  if (deltaValue >= 8) {
    deltaValue = 0;
  }
  step++;
}

// ============== ByEffect ==============
//             © SlingMaster
//             EFF_BY_EFFECT
//            Побочный Эффект
// --------------------------------------
void ByEffect() {
  uint8_t saturation;
  uint8_t delta;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed 210
      setModeSettings(random8(100U), random8(200U));
    }
#endif
    loadingFlag = false;
    deltaValue = 0;
    step = deltaValue;
    FastLED.clear();
  }

  hue = floor(step / 32) * 32U;
  dimAll(180);
  // ------
  saturation = 255U;
  delta = 0;
  for (uint8_t x = 0U; x < WIDTH + 1 ; x++) {
    if (x % 8 == 0) {
      gradientVertical( x - deltaValue, floor(HEIGHT * 0.75), x + 1U - deltaValue, HEIGHT,  hue, hue + 2, 250U, 0U, 255U);
      if (modes[currentMode].Scale > 50) {
        delta = random8(200U);
      }
      drawPixelXY(x - 2 - deltaValue, floor(HEIGHT * 0.7), CHSV(step, saturation - delta, 128 + random8(128)));
      drawPixelXY(x + 2 - deltaValue, floor(HEIGHT * 0.7), CHSV(step, saturation, 128 + random8(128)));

      drawPixelXY(x - deltaValue, floor(HEIGHT * 0.6), CHSV(hue, 255U, 190 + random8(65)));
      if (modes[currentMode].Scale > 50) {
        delta = random8(200U);
      }
      drawPixelXY(x - 1 - deltaValue, CENTER_Y_MINOR, CHSV(step, saturation, 128 + random8(128)));
      drawPixelXY(x + 1 - deltaValue, CENTER_Y_MINOR, CHSV(step, saturation - delta, 128 + random8(128)));

      drawPixelXY(x - deltaValue, floor(HEIGHT * 0.4), CHSV(hue, 255U, 200U));
      if (modes[currentMode].Scale > 50) {
        delta = random8(200U);
      }
      drawPixelXY(x - 2 - deltaValue, floor(HEIGHT * 0.3), CHSV(step, saturation - delta, 96 + random8(128)));
      drawPixelXY(x + 2 - deltaValue, floor(HEIGHT * 0.3), CHSV(step, saturation, 96 + random8(128)));

      gradientVertical( x - deltaValue, 0U, x + 1U - deltaValue, floor(HEIGHT * 0.25),  hue + 2, hue, 0U, 250U, 255U);

      if (modes[currentMode].Scale > 50) {
        drawPixelXY(x + 3 - deltaValue, HEIGHT - 3U, CHSV(step, 255U, 255U));
        drawPixelXY(x - 3 - deltaValue, CENTER_Y_MINOR, CHSV(step, 255U, 255U));
        drawPixelXY(x + 3 - deltaValue, 2U, CHSV(step, 255U, 255U));
      }
    }
  }
  // ------
  deltaValue++;
  if (deltaValue >= 8) {
    deltaValue = 0;
  }
  step++;
}


// =====================================
//            Строб Хаос Дифузия
//          Strobe Haos Diffusion
//             © SlingMaster
// =====================================
/*должен быть перед эффектом Матрицf бегунок Скорость не регулирует задержку между кадрами,
  но меняет частоту строба*/
void StrobeAndDiffusion() {
  const uint8_t SIZE = 3U;
  const uint8_t DELTA = 1U;         // центровка по вертикали
  uint8_t STEP = 2U;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(1U + random8(100U), 1U + random8(150U));
    }
#endif
    loadingFlag = false;
    FPSdelay = 25U; // LOW_DELAY;
    hue2 = 1;
    FastLED.clear();
  }

  STEP = floor((255 - modes[currentMode].Speed) / 64) + 1U; // for strob
  if (modes[currentMode].Scale > 50) {
    // diffusion ---
    blurScreen(beatsin8(3, 64, 80));
    FPSdelay = LOW_DELAY;
    STEP = 1U;
    if (modes[currentMode].Scale < 75) {
      // chaos ---
      FPSdelay = 30;
      VirtualSnow();
    }

  } else {
    // strob -------
    if (modes[currentMode].Scale > 25) {
      dimAll(200);
      FPSdelay = 30;
    } else {
      dimAll(240);
      FPSdelay = 40;
    }
  }

  const uint8_t rows = (HEIGHT + 1) / 3U;
  deltaHue = floor(modes[currentMode].Speed / 64) * 64;
  bool dir = false;
  for (uint8_t y = 0; y < rows; y++) {
    if (dir) {
      if ((step % STEP) == 0) {   // small layers
        drawPixelXY(WIDTH - 1, y * 3 + DELTA, CHSV(step, 255U, 255U ));
      } else {
        drawPixelXY(WIDTH - 1, y * 3 + DELTA, CHSV(170U, 255U, 1U));
      }
    } else {
      if ((step % STEP) == 0) {   // big layers
        drawPixelXY(0, y * 3 + DELTA, CHSV((step + deltaHue), 255U, 255U));
      } else {
        drawPixelXY(0, y * 3 + DELTA, CHSV(0U, 255U, 0U));
      }
    }

    // сдвигаем слои  ------------------
    for (uint8_t x = 0U ; x < WIDTH; x++) {
      if (dir) {  // <==
        drawPixelXY(x - 1, y * 3 + DELTA, getPixColorXY(x, y * 3 + DELTA));
      } else {    // ==>
        drawPixelXY(WIDTH - x, y * 3 + DELTA, getPixColorXY(WIDTH - x - 1, y * 3 + DELTA));
      }
    }
    dir = !dir;
  }

  if (hue2 == 1) {
    step ++;
    if (step >= 254) hue2 = 0;
  } else {
    step --;
    if (step < 1) hue2 = 1;
  }
}

// =====================================
//               Фейерверк
//                Firework
//             © SlingMaster
// =====================================
void VirtualExplosion(uint8_t f_type, int8_t timeline) {
  const uint8_t DELAY_SECOND_EXPLOSION = HEIGHT * 0.25;
  uint8_t horizont = 1U; // HEIGHT * 0.2;
  const int8_t STEP = 255 / HEIGHT;
  uint8_t firstColor = random8(255);
  uint8_t secondColor = 0;
  uint8_t saturation = 255U;
  switch (f_type) {
    case 0:
      secondColor =  random(50U, 255U);
      saturation = random(245U, 255U);
      break;
    case 1: /* сакура */
      firstColor = random(210U, 230U);
      secondColor = random(65U, 85U);
      saturation = 255U;
      break;
    case 2: /* день Независимости */
      firstColor = random(160U, 170U);
      secondColor = random(25U, 50U);
      saturation = 255U;
      break;
    default: /* фризантемы */
      firstColor = random(30U, 40U);
      secondColor = random(25U, 50U);
      saturation = random(128U, 255U);
      break;
  }
  if ((timeline > HEIGHT - 1 ) & (timeline < HEIGHT * 1.75)) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      for (uint8_t y =  horizont; y < HEIGHT - 1; y++) {
        noise3d[0][x][y] = noise3d[0][x][y + 1];
        uint8_t bri = y * STEP;
        if (noise3d[0][x][y] > 0) {
          if (timeline > (HEIGHT + DELAY_SECOND_EXPLOSION) ) {
            /* second explosion */
            drawPixelXY((x - 2 + random8(4)), y - 1, CHSV(secondColor + random8(16), saturation, bri));
          }
          if (timeline < ((HEIGHT - DELAY_SECOND_EXPLOSION) * 1.75) ) {
            /* first explosion */
            drawPixelXY(x, y, CHSV(firstColor, 255U, bri));
          }
        } else {
          // drawPixelXY(x, y, CHSV(175, 255U, floor((255 - bri) / 4)));
        }
      }
    }
    uint8_t posX = random8(WIDTH);
    for (uint8_t x = 0U; x < WIDTH; x++) {
      // заполняем случайно верхнюю строку
      if (posX == x) {
        if (step % 2 == 0) {
          noise3d[0][x][HEIGHT - 1U] = 1;
        } else {
          noise3d[0][x][HEIGHT - 1U]  = 0;
        }
      } else {
        noise3d[0][x][HEIGHT - 1U]  = 0;
      }
    }
  }
}

// --------------------------------------
void Firework() {
  const uint8_t MAX_BRIGHTNESS = 40U;            /* sky brightness */
  const uint8_t DOT_EXPLOSION = HEIGHT * 0.95;
  const uint8_t HORIZONT = HEIGHT * 0.25;
  const uint8_t DELTA = 1U;                      /* центровка по вертикали */
  const float stepH = HEIGHT / 128.0;
  const uint8_t FPS_DELAY = 20U;
  const uint8_t STEP = 3U;
  const uint8_t skyColor = 156U;
  uint8_t sizeH;

  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(1U + random8(100U), 1U + random8(250U));
    }
#endif
    loadingFlag = false;
    deltaHue2 = 0;
    FPSdelay = 255U;
    FastLED.clear();
    step = 0U;
    deltaHue2 = floor(modes[currentMode].Scale / 26);
    hue = 48U;            // skyBright
    if (modes[currentMode].Speed > 85U) {
      sizeH = HORIZONT;
      FPSdelay = FPS_DELAY;
    }
  }

  if (FPSdelay > 128U) {
    /* вечерело */
    FPSdelay--;
    sizeH = (FPSdelay - 128U) * stepH;
    // LOG.printf_P(PSTR("• [%03d] | %03d | %0.2f | \n"), FPSdelay, stepH, sizeH);
    dimAll(200);

    if (STRIP_DIRECTION % 2 == 0) {
      gradientDownTop( 0, CHSV(skyColor, 255U, floor(FPSdelay / 2.2)), sizeH, CHSV(skyColor, 255U, 2U));
    } else {
      gradientVertical(0, 0, WIDTH, sizeH, skyColor, skyColor, floor(FPSdelay / 2.2), 2U, 255U);
    }

    if (sizeH > HORIZONT) return;
    if (sizeH == HORIZONT )  FPSdelay = FPS_DELAY;
  }

  if (step > DOT_EXPLOSION ) {
    blurScreen(beatsin8(3, 64, 80));
  }
  if (step == DOT_EXPLOSION - 1) {
    /* включаем фазу затухания */
    FPSdelay = 70;
  }
  if (step > CENTER_Y_MAJOR) {
    dimAll(140);
  } else {
    dimAll(100);
  }


  /* ============ draw sky =========== */
  if (modes[currentMode].Speed < 180U) {
    if (STRIP_DIRECTION % 2 == 0) {
      gradientDownTop( 0, CHSV(skyColor, 255U, hue ), HORIZONT, CHSV(skyColor, 255U, 0U ));
    } else {
      gradientVertical(0, 0, WIDTH, HORIZONT, skyColor, skyColor, hue + 1, 0U, 255U);
    }
  }

  /* deltaHue2 - Firework type */
  VirtualExplosion(deltaHue2, step);

  if ((step > DOT_EXPLOSION ) & (step < HEIGHT * 1.5)) {
    /* фаза взрыва */
    FPSdelay += 5U;
  }
  const uint8_t rows = (HEIGHT + 1) / 3U;
  deltaHue = floor(modes[currentMode].Speed / 64) * 64;
  if (step > CENTER_Y_MAJOR) {
    bool dir = false;
    for (uint8_t y = 0; y < rows; y++) {
      /* сдвигаем слои / эмитация разлета */
      for (uint8_t x = 0U ; x < WIDTH; x++) {
        if (dir) {  // <==
          drawPixelXY(x - 1, y * 3 + DELTA, getPixColorXY(x, y * 3 + DELTA));
        } else {    // ==>
          drawPixelXY(WIDTH - x, y * 3 + DELTA, getPixColorXY(WIDTH - x - 1, y * 3 + DELTA));
        }
      }
      dir = !dir;
      /* --------------------------------- */
    }
  }

  /* ========== фаза полета ========== */
  if (step < DOT_EXPLOSION ) {
    FPSdelay ++;
    if (HEIGHT < 20) {
      FPSdelay ++;
    }
    /* закоментируйте следующие две строки если плоская лампа
      подсветка заднего фона */
    if (custom_eff == 1) {
      DrawLine(0U, 0U, 0U, HEIGHT - step, CHSV(skyColor, 255U, 32U));
      DrawLine(WIDTH - 1, 0U, WIDTH - 1U, HEIGHT - step, CHSV(skyColor, 255U, 32U));
    }
    /* ------------------------------------------------------ */

    uint8_t saturation = (step > (DOT_EXPLOSION - 2U)) ? 192U : 20U;
    //uint8_t rndPos = deltaHue2;  //uint8_t rndPos = 3U * deltaHue2 * 0.5;
    drawPixelXY(CENTER_X_MINOR + deltaHue2, step,  CHSV(50U, saturation, 80U));                 // first
    drawPixelXY(CENTER_X_MAJOR - deltaHue2, step - HORIZONT,  CHSV(50U, saturation, 80U));  // second
    /* sky brightness */
    if (hue > 2U) {
      hue -= 1U;
    }
  }
  if (step > HEIGHT * 1.25) {
    /* sky brightness */
    if (hue < MAX_BRIGHTNESS) {
      hue += 2U;
    }
  }

  if (step >= (HEIGHT * 2.5)) {
    step = 0U;
    FPSdelay = FPS_DELAY;
    if (modes[currentMode].Scale <= 1) {
      deltaHue2++;
    }
    if (deltaHue2 >= 4U) deltaHue2 = 0U;  // next Firework type
  }
  //  LOG.printf_P(PSTR("• [%03d] | %03d | sky Bright • [%03d]\n"), step, FPSdelay, hue);
  step ++;
}

/*
// =====================================
//             Планета Земля
//              PlanetEarth
//             © SlingMaster
// =====================================
void PlanetEarth() {
  static uint16_t imgW = 0;
  static uint16_t imgH = 0;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(128U, 10U + random8(230U));
    }
#endif
    loadingFlag = false;
    FPSdelay = 96U;
    FastLED.clear();
    String file_name = (modes[currentMode].Scale < 50) ? "globe0" : "globe1";
    readBinFile("bin/" + file_name + ".img", 2048 );

    imgW = getSizeValue(binImage, 8 );
    imgH = getSizeValue(binImage, 10 );

#ifdef GENERAL_DEBUG
    LOG.printf_P(PSTR("Image • %03d x %02d px\n"), imgW, imgH);
#endif
    scrollImage(imgW, imgH, 1U);
    ff_x = 1U;
  }

  // scrool index reverse --> 
  if (ff_x < 1) ff_x = (imgW - imgH);
  scrollImage(imgW, imgH, ff_x - 1);
  ff_x--;

  // <-- scrool index ------- 
  //  if (ff_x > (imgW - imgH)) ff_x = 1U;
  //  scrollImage(imgW, imgH, ff_x - 1);
  //  ff_x++;
}
*/

// =====================================
//             Мечта Дизайнера
//                WebTools
//             © SlingMaster
// =====================================
/* --------------------------------- */
int getRandomPos(uint8_t STEP) {
  uint8_t val = floor(random(0, (STEP * 16 - WIDTH - 1)) / STEP) * STEP;
  return -val;
}

/* --------------------------------- */
int getHue(uint8_t x, uint8_t y) {
  return ( x * 32 +  y * 24U );
}

/* --------------------------------- */
uint8_t getSaturationStep() {
  return (modes[currentMode].Speed > 170U) ? ((HEIGHT > 24) ? 12 : 24) : 0;
}

/* --------------------------------- */
uint8_t getBrightnessStep() {
  return (modes[currentMode].Speed < 85U) ? ((HEIGHT > 24) ? 16 : 24) : 0;
}

/* --------------------------------- */
void drawPalette(int posX, int posY, uint8_t STEP) {
  int PX, PY;
  const uint8_t SZ = STEP - 1;
  const uint8_t maxY = floor(HEIGHT / SZ);
  uint8_t sat = getSaturationStep();
  uint8_t br  = getBrightnessStep();

  FastLED.clear();
  for (uint8_t y = 0; y < maxY; y++) {
    for (uint8_t x = 0; x < 16; x++) {
      PY = y * STEP;
      PX = posX + x * STEP;
      if ((PX >= - STEP ) && (PY >= - STEP) && (PX < WIDTH) && (PY < HEIGHT)) {
        // LOG.printf_P(PSTR("y: %03d | br • %03d | sat • %03d\n"), y, (240U - br * y), sat);
        drawRecCHSV(PX, PY, PX + SZ, PY + SZ, CHSV( getHue(x, y), (255U - sat * y), (240U - br * y)));
      }
    }
  }
}

/* --------------------------------- */
void selectColor(uint8_t sc) {
  uint8_t offset = (WIDTH >= 16) ? WIDTH * 0.25 : 0;
  hue = getHue(random(offset, WIDTH - offset), random(HEIGHT));
  uint8_t sat = getSaturationStep();
  uint8_t br  = getBrightnessStep();

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = offset; x < (WIDTH - offset); x++) {
      CHSV curColor = CHSV(hue, (255U - sat * y), (240U - br * y));
      if (curColor == getPixColorXY(x, y)) {
        /* show srlect color */
        drawRecCHSV(x, y, x + sc, y + sc, CHSV( hue, 64U, 255U));
        FastLED.show();
        delay(400);
        drawRecCHSV(x, y, x + sc, y + sc, CHSV( hue, 255U, 255U));
        y = HEIGHT;
        x = WIDTH;
      }
    }
  }
}

/* --------------------------------- */
void WebTools() {
  const uint8_t FPS_D = 24U;
  static uint8_t STEP = 3U;
  static int posX = -STEP;
  static int posY = 0;
  static int nextX = -STEP * 2;
  static bool stop_moving = true;
  uint8_t speed = constrain (modes[currentMode].Speed, 65, 255);
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
       setModeSettings(random(10U, 90U), random(10U, 255U));
    }
#endif
    loadingFlag = false;
    FPSdelay = 1U;
    step = 0;
    STEP = 2U + floor(modes[currentMode].Scale / 35);
    posX = 0;
    posY = 0;
    drawPalette(posX, posY, STEP);
  }
  /* auto scenario */
  //switch (step) {
    if (step == 0){     /* restart ----------- */
      nextX = 0;
      FPSdelay = FPS_D;
    }
    else 
    if (step == speed/16+1){    /* start move -------- 16*/
      nextX = getRandomPos(STEP);
      FPSdelay = FPS_D;
    }
    else
    if (step == speed/10+1){    /* find --------------100 */
      nextX = getRandomPos(STEP);
      FPSdelay = FPS_D;
    }
    else
    if (step == speed/7+1){    /* find 2 ----------- 150*/
      nextX = getRandomPos(STEP);
      FPSdelay = FPS_D;
    }
    else
    if (step == speed/6+1){    /* find 3 -----------200 */
      nextX = - STEP * random(4, 8);
      // nextX = getRandomPos(STEP);
      FPSdelay = FPS_D;
    }
    else
    if (step == speed/5+1){   /* select color ------220 */
      FPSdelay = 200U;
      selectColor(STEP - 1);
    }
    else
    if (step == speed/4+1){   /* show color -------- 222*/
      FPSdelay = FPS_D;
      nextX = WIDTH;
    }
    else
    if (step == speed/4+3){
      step = 252;
    }
    
  //}
  if (posX < nextX) posX++;
  if (posX > nextX) posX--;

  if (stop_moving)   {
    FPSdelay = 80U;
    step++;
  } else {
    drawPalette(posX, posY, STEP);
    if ((nextX == WIDTH) || (nextX == 0)) {
      /* show select color bar gradient */
      // LOG.printf_P(PSTR("step: %03d | Next x: %03d • %03d | fps %03d\n"), step, nextX, posX, FPSdelay);
      if (posX > 1) {
        gradientHorizontal(0, 0, (posX - 1), HEIGHT, hue, hue, 255U, 96U, 255U);
      }
      if (posX > 3) DrawLine(posX - 3, CENTER_Y_MINOR, posX - 3, CENTER_Y_MAJOR, CHSV( hue, 192U, 255U));
    }
  }

  stop_moving = (posX == nextX); 
}

// =====================================
//                Contacts
//             © Yaroslaw Turbin
//        Adaptation © SlingMaster
// =====================================

void Contacts() {
  static const uint8_t exp_gamma[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
    4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
    7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  12,  12,
    12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
    19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,
    28,  29,  30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,
    39,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  50,  51,  52,
    53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
    68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  85,
    87,  89,  91,  92,  93,  95,  96,  98,  99,  100, 101, 102, 105, 106, 108,
    109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 125, 126, 128, 130, 131,
    133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 152, 154, 156, 158,
    160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
    190, 192, 194, 196, 198, 200, 202, 204, 207, 209, 211, 213, 216, 218, 220,
    222, 225, 227, 229, 232, 234, 236, 239, 241, 244, 246, 249, 251, 253, 254,
    255
  };
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(random(25U, 90U), random(5U, 250U));
    }
#endif
    loadingFlag = false;
    FPSdelay = 80U;
    FastLED.clear();
  }

  int a = millis() / floor((255 - modes[currentMode].Speed) / 10);
  hue = floor(modes[currentMode].Scale / 17);
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      int index = XY(x, y);
      uint8_t color1 = exp_gamma[sin8((x - 8) * cos8((y + 20) * 4) / 4)];
      uint8_t color2 = exp_gamma[(sin8(x * 16 + a / 3) + cos8(y * 8 + a / 2)) / 2];
      uint8_t color3 = exp_gamma[sin8(cos8(x * 8 + a / 3) + sin8(y * 8 + a / 4) + a)];
      if (hue == 0) {
        leds[index].b = color3 / 4;
        leds[index].g = color2;
        leds[index].r = 0;
      } else if (hue == 1) {
        leds[index].b = color1;
        leds[index].g = 0;
        leds[index].r = color3 / 4;
      } else if (hue == 2) {
        leds[index].b = 0;
        leds[index].g = color1 / 4;
        leds[index].r = color3;
      } else if (hue == 3) {
        leds[index].b = color1;
        leds[index].g = color2;
        leds[index].r = color3;
      } else if (hue == 4) {
        leds[index].b = color3;
        leds[index].g = color1;
        leds[index].r = color2;
      } else if (hue == 5) {
        leds[index].b = color2;
        leds[index].g = color3;
        leds[index].r = color1;
      }
    }
  }
}

// ============ Magic Lantern ===========
//             © SlingMaster
//            Чарівний Ліхтар
// --------------------------------------
void MagicLantern() {
  static uint8_t saturation;
  static uint8_t brightness;
  static uint8_t low_br;
  uint8_t delta;
  const uint8_t PADDING = HEIGHT * 0.25;
  const uint8_t WARM_LIGHT = 55U;
  const uint8_t STEP = 4U;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed 210
      setModeSettings(random8(100U), random8(40, 200U));
    }
#endif
    loadingFlag = false;
    deltaValue = 0;
    step = deltaValue;
    if (modes[currentMode].Speed > 52) {
      // brightness = 50 + modes[currentMode].Speed;
      brightness = map(modes[currentMode].Speed, 1, 255, 50U, 250U);
      low_br = 50U;
    } else {
      brightness = 0U;
      low_br = 0U;
    }
    saturation = (modes[currentMode].Scale > 50U) ? 64U : 0U;
    if (abs (70 - modes[currentMode].Scale) <= 5) saturation = 170U;
    FastLED.clear();

  }
  dimAll(170);
  hue = (modes[currentMode].Scale > 95) ? floor(step / 32) * 32U : modes[currentMode].Scale * 2.55;

  // ------
  for (uint8_t x = 0U; x < WIDTH + 1 ; x++) {

    // light ---
    if (low_br > 0) {
      gradientVertical( x - deltaValue, CENTER_Y_MAJOR, x + 1U - deltaValue, HEIGHT - PADDING - 1,  WARM_LIGHT, WARM_LIGHT, brightness, low_br, saturation);
      gradientVertical( WIDTH - x + deltaValue, CENTER_Y_MAJOR, WIDTH - x + 1U + deltaValue, HEIGHT - PADDING - 1,  WARM_LIGHT, WARM_LIGHT, brightness, low_br, saturation);
      gradientVertical( x - deltaValue, PADDING + 1, x + 1U - deltaValue, CENTER_Y_MAJOR, WARM_LIGHT, WARM_LIGHT, low_br + 10, brightness, saturation);
      gradientVertical( WIDTH - x + deltaValue, PADDING + 1, WIDTH - x + 1U + deltaValue, CENTER_Y_MAJOR, WARM_LIGHT, WARM_LIGHT, low_br + 10, brightness, saturation);
    } else {
      if (x % (STEP + 1) == 0) {
        leds[XY(random8(WIDTH), random8(PADDING + 2, HEIGHT - PADDING - 2))] = CHSV(step - 32U, random8(128U, 255U), 255U);
      }
      if ((modes[currentMode].Speed < 25) & (low_br == 0)) {
        deltaValue = 0;
        if (x % 2 != 0) {
          gradientVertical( x - deltaValue, HEIGHT - PADDING, x + 1U - deltaValue, HEIGHT,  hue, hue + 2, 64U, 20U, 255U);
          gradientVertical( (WIDTH - x + deltaValue), 0U,  (WIDTH - x + 1U + deltaValue), PADDING,  hue, hue, 42U, 64U, 255U);
        }
        //        deltaValue = 0;
      }
    }
    if (x % STEP == 0) {
      // body --
      gradientVertical( x - deltaValue, HEIGHT - PADDING, x + 1U - deltaValue, HEIGHT,  hue, hue + 2, 255U, 20U, 255U);
      gradientVertical( (WIDTH - x + deltaValue), 0U,  (WIDTH - x + 1U + deltaValue), PADDING,  hue, hue, 42U, 255U, 255U);
    }
  }
  // ------

  deltaValue++;
  if (deltaValue >= STEP) {
    deltaValue = 0;
  }

  step++;
}

// ============ Octopus ===========
//        © Stepko and Sutaburosu
//    Adapted and modifed © alvikskor
//             Восьминіг
// --------------------------------------
//Idea from https://www.youtube.com/watch?v=HsA-6KIbgto&ab_channel=GreatScott%21

void Octopus() {
    
  //FastLED.clear();
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(random(10U, 101U), random(150U, 255U));
    }
#endif
    loadingFlag = false;
    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR + (WIDTH % 2); x++) {
      for (int8_t y = -CENTER_Y_MAJOR; y < CENTER_Y_MAJOR + (HEIGHT % 2); y++) {
        noise3d[0][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = (atan2(x, y) / PI) * 128 + 127; // thanks ldirko
        noise3d[1][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = hypot(x, y); // thanks Sutaburosu
      }
    }
  }
  
  uint8_t legs = modes[currentMode].Scale / 10;
  uint16_t color_speed;
  step = modes[currentMode].Scale % 10;
  if (step < 5) color_speed = scale / (3 - step/2);
  else color_speed = scale * (step/2 - 1);
  scale ++;
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      byte angle = noise3d[0][x][y];
      byte radius = noise3d[1][x][y];
      leds[XY(x, y)] = CHSV(color_speed - radius * (255 / WIDTH), 255,sin8(sin8((angle*4-(radius * (255 / WIDTH)))/4+scale) + radius * (255 / WIDTH) - scale*2 + angle * legs));
    }
  }
  //delay(255 - modes[currentMode].Speed);
}