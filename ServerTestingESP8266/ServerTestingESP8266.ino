#include "Wifi_S08_v2.h"

#define SSID "ESP8266"
#define PASSWD "MOMENTUM"

ESP8266 wifi = ESP8266(1,true);

void setup() {
  Serial.begin(115200);
  wifi.begin();
  wifi.startserver(SSID, PASSWD);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(wifi.hasResponse()){
    Serial.println(wifi.getResponse());
    Serial.print(1);
  }
}
