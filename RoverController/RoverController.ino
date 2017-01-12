#include "RoverLib.h"
#include <Arduino.h>
#include <Wifi_S08_v2.h>

#define SSID "ESPeezy"
#define PASSWD "MOMENTUM"
#define POLLPERIOD 100

unsigned long lastRequest = 0;

ESP8266 wifi;

String MAC;

char res[512];
char* rptr;

char cms[512];
char* cptr;

Rover WallE = Rover(23, 22, 21, 20);

void setup() {
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  while (!wifi.isConnected());
  WallE.begin();
}

String parsecmds(String response){
    
    String cmds;
    response.toCharArray(res, 512);
    Serial.println(res);
    rptr = res;
    for (int i = 0; i<=6; i++){
        rptr++;
      }
    while(*rptr != '<'){
      if (*rptr != ','){
        cmds += *rptr;
      }
      Serial.println(*rptr);
      rptr++;
      }
    return cmds;
  }

void RoverMove(char cmd){
  if (cmd == 'f'){
    WallE.set_RWV(20);
    WallE.set_LWV(20);
    }
  else if (cmd == 'b'){
    WallE.set_RWV(-20);
    WallE.set_LWV(-20);
    }
  else if (cmd == 'l'){
    WallE.set_RWV(20);
    WallE.set_LWV(-20);
    }
  else if (cmd == 'r'){
    WallE.set_RWV(-20);
    WallE.set_LWV(20);
    }
  else{
    WallE.set_RWV(0);
    WallE.set_LWV(0);
    }
    return;
  }

void loop() {
  String cmds = "No Args";
  if (wifi.hasResponse()) {
    String response = wifi.getResponse();
    Serial.print("RESPONSE: ");
    Serial.println(response);
    cmds = parsecmds(response);
  }
  if (!wifi.isBusy()&& millis()-lastRequest > POLLPERIOD) {
    String domain = "192.168.4.1";
    String path = "/"; 
    wifi.sendRequest(GET, domain, 80, path, "RECEIVED=1");
    lastRequest = millis();
  }
  if (cmds != "No Args"){
    cmds += 'e';
    cmds.toCharArray(cms, 512);
    cptr = cms;
    while (*cptr != 'e'){
      RoverMove(*cptr);
      cptr++;
    }
    }

  
}
