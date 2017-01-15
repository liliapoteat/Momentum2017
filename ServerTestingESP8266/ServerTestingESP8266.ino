#include <Wifi_S08_v2.h>

#define SSID "ESPeezy"
#define PASSWD "MOMENTUM"
#define BLINKPERIOD 100
#define UPDATEPERIOD 1000

ESP8266 wifi = ESP8266(1,true);
String html = "<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted.</p>\n</body>\n</html>";
int val = 0;
int led = 13;
int light = 0;

int lastupdate = millis();
int blinktime = millis();

char html2[500];

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  wifi.begin();
  Serial.println("startserver");
  wifi.startserver(SSID, PASSWD);
  Serial.println("setup complete");
  wifi.setPage("/",html);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println((millis()-lastupdate));
  int m = millis() - lastupdate;
  if(m > UPDATEPERIOD){
    val++;
    sprintf(html2,"<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted %d times.</p>\n</body>\n</html>",val);
    wifi.setPage("/",html2);
    lastupdate = millis();
  }
  m = millis() - blinktime;
  if(m > BLINKPERIOD){
    if (light == 1){
      Serial.println("blink");
      light = 0;
      digitalWrite(led, LOW);
    }
    else{
      light = 1;
      digitalWrite(led, HIGH);
    }
    blinktime = millis();
  }
}
