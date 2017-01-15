#include <Wifi_S08_v2.h>

#define SSID "MARKESP"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 1000
#define BLOCKCHECK 100

ESP8266 wifi = ESP8266(0,false);

String MAC;
unsigned long lastRequest = 0;

int led = 13;

void setup() {
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  while (!wifi.isConnected()); //wait for connection

  //check blocking
  pinMode(led, OUTPUT);
}

int count = 0;

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(BLOCKCHECK);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(BLOCKCHECK);               // wait for a second

  
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
