void SSDP_init(void) {
  String chipID = String( ESP.getChipId() ) + "-" + String( ESP.getFlashChipId() );
  // SSDP дескриптор
  HTTP.on("/description.xml", HTTP_GET, []() {
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
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(jsonRead(configSetup, "SSDP"));
  SSDP.setSerialNumber(chipID);
  SSDP.setURL("/");
  SSDP.setModelName("gunner47_v2.2_web_v2.1_94in1");
  SSDP.setModelNumber(chipID + "/" + jsonRead(configSetup, "SSDP"));
  
  
  SSDP.setModelURL("https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proshivki-ot-gunner47.2418/page-72#post-33652");
  SSDP.setManufacturer("alvikskor");
  SSDP.setManufacturerURL("https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proshivki-ot-gunner47.2418/page-72#post-33652");
  SSDP.begin();
}
