void SSDP_init(void) {
  String chipID = String( ESP.getChipId() ) + "-" + String( ESP.getFlashChipId() );
  // SSDP дескриптор
  HTTP.on(F("/description.xml"), HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
   // --------------------Получаем SSDP со страницы
  HTTP.on("/ssdp", HTTP_GET, []() {
    String ssdp = HTTP.arg("ssdp");
 // configSetup=jsonWrite(configJson, "SSDP", ssdp);
  jsonWrite(configSetup, "SSDP", ssdp);
  SSDP.setName(jsonRead(configSetup, "SSDP"));
  saveConfig();                 // Функция сохранения данных во Flash
  LAMP_NAME = jsonRead(configSetup, "SSDP");
  HTTP.send(200, F("text/plain"), "OK"); // отправляем ответ о выполнении
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType(F("upnp:rootdevice"));
  SSDP.setSchemaURL(F("description.xml"));
  SSDP.setHTTPPort(80);
  SSDP.setName(jsonRead(configSetup, "SSDP"));
  SSDP.setSerialNumber(chipID);
  SSDP.setURL("/");
  SSDP.setModelName(F("FieryLedLamp"));
  SSDP.setModelNumber(chipID + "/" + jsonRead(configSetup, "SSDP"));
  
  
  SSDP.setModelURL(F("https://github.com/alvikskor/FieryLedLampMultilingual"));
  SSDP.setManufacturer(F("alvikskor"));
  SSDP.setManufacturerURL(F("https://github.com/alvikskor/FieryLedLampMultilingual"));
  SSDP.begin();
}
