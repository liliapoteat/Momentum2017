#include <Wifi_S08_v2.h>               //Include the wifi library

#define SSID "ESPeezy"				//Set SSID
#define PASSWD "MOMENTUM"			//Set Password

//Timing periods
#define BLINKPERIOD 1000
#define UPDATEPERIOD 10

//Sensor Pins
#define LR 14
#define UD 15

ESP8266 wifi = ESP8266(1,true);		//initialize the Server

//Sensor reading function -> returns a cmd char
char readSensors();

//Sensor values
int sval1;
int sval2;

//The command to be sent
char cmd;

//Block Checking LED (Used to make sure the wifi library isn't interfering with loop code)
int led = 13;
int light = 0;

//Time keeping variables
int lastupdate = millis();
int blinktime = millis();

//root html Page
char html[500];

void setup() {
  //Set up block checking LED
  pinMode(led, OUTPUT);

  //Start serial com at baud 115200
  Serial.begin(115200);

  //Start up the wifi
  wifi.begin();

  //Start up the server
  wifi.startserver(SSID, PASSWD);
}

void loop() {
  //Set the cmd based on sensor readings
  cmd = readSensors();
  

  //Time checking variable (could be implemented differently)
  int m = millis() - lastupdate;

  //html page update period
  if(m > UPDATEPERIOD){
    sprintf(html,"<html>\n%c</html>",cmd);
    wifi.setPage("/",html);
    lastupdate = millis();

    Serial.println();
    Serial.print("L/R val: ");
    Serial.println(sval1);
    Serial.print("U/D val: ");
    Serial.println(sval2);
    Serial.print("CMD: ");
    Serial.println(cmd);
  }


  m = millis() - blinktime;
  
  //Block checking -> if blinking is not consistent something is wrong
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

char readSensors(){
	//Read the values
	sval1 = analogRead(LR);
	sval2 = analogRead(UD);

	//define forward condition
	if(sval2 > 850 && (sval1 < 800 || sval1 > 300)){
	  return 'f';
	}
	//define left condition
	else if(sval1 < 400 && (sval2 < 800 || sval2 > 300)){
	  return 'l';
	}
	//define right condition
	else if(sval1 > 850 && (sval2 < 800 || sval2 > 300)){
	  return 'r';
	}
	//define back condition
	else if(sval2 < 400 && (sval1 < 800 || sval1 > 300)){
	  return 'b';
	}
	//define stop condition
	else{
	  return 's';
	}
}

