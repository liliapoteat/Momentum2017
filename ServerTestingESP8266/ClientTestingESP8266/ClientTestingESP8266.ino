#include <Wifi_S08_v2.h>

#define SSID "ESPeezy"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 1000

ESP8266 wifi = ESP8266(0,false);

String MAC;
unsigned long lastRequest = 0;


void setup() {
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  while (!wifi.isConnected()); //wait for connection

}

int count = 0;

void loop() {
  if (wifi.hasResponse()) {
    String response = wifi.getResponse();
    Serial.print("RESPONSE: ");
    Serial.println(response);
    count++;
    Serial.println(count);
  }

  if (!wifi.isBusy() && millis()-lastRequest > POLLPERIOD) {
    String domain = "192.168.4.1";
    String path = "/"; 

    //String domain = "iesc-s2.mit.edu";
    //String path = "/hello.html";

    
    wifi.sendRequest(GET, domain, 80, path, "");
    lastRequest = millis();
  }
}
