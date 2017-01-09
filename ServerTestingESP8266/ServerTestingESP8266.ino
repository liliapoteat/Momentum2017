#include "Wifi_S08_v2.h"

#define SSID "ESP8266"
#define PASSWD "MOMENTUM"

ESP8266 wifi = ESP8266(1,true);
String html = "<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted.</p>\n</body>\n</html>";
int val = 0;

void setup() {
  Serial.begin(115200);
  wifi.begin();
  Serial.println("startserver");
  wifi.startserver(SSID, PASSWD);
  Serial.println("setup complete");
  wifi.setPage("/",html);
}

void loop() {
  // put your main code here, to run repeatedly:
  char html2[500];
  val++;
  sprintf(html2,"<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted %d times.</p>\n</body>\n</html>",val);
  wifi.setPage("/",html2);
  Serial.println("looping");
  delay(3000);
}
