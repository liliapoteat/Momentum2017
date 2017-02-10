#include <Wifi_S08_v2.h>

#define SSID "HELLO"
#define PASSWD "MOMENTUM"

String orientations[] = {"up", "right", "down", "left"};
int orientation = 0;

// encoder variables for turning
int encoderPinA = 12; // white
int encoderPinB = 11; // yellow
int encoderVal = 0;
int encoderPinALast = LOW;
int encoderThreshold = 275;

int count = 0;
int constspd = 120;
int turnspd = 150;


#define POLLPERIOD 200
#define CMDPERIOD 500
#define SAMPLINGPERIOD 100
#define TURNPERIOD 1800

ESP8266 wifi = ESP8266(0, false); // debugging
String ultraSonic(int trigPin, int echoPin);

String MAC;
unsigned long lastRequest = 0;
unsigned long lastSample = 0;
unsigned long lastTurn = 0;
unsigned long cmddur = 0;

int trigPinArray[4] = {22, 9, 3, 5};
int echoPin;
int arrayIndex = 0;
long duration, cm, inchez;

int led = 13;

int spdR = 21;
int dirR = 19;
int spdL = 20;
int dirL = 18;
String ultraSonicDistance = "V";

int pulse_state = 1;
int pulseChange = micros();

bool turnState = false;
void moveRover(String movRoverDir);
void stopRover();

void setup()
{
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  //Serial.println("connectWifi");
  while (!wifi.isConnected());

  //Serial.println("I made it");
  //TURN LIGHT ON
  pinMode(led, OUTPUT);
  //delay(10);
  digitalWrite(led, HIGH);
  //SET DIRECTION FORWARD
  pinMode(dirL, OUTPUT);
  pinMode(dirR, OUTPUT);
  digitalWrite(dirL, HIGH);
  digitalWrite(dirR, HIGH);
  //Serial Port begin
  //Serial.begin (9600);
  //Define inputs and outputs
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(10, INPUT);
  pinMode(23, INPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
}

void loop()
{
  if (wifi.hasResponse() && millis() - cmddur > CMDPERIOD)
  {
    String response = wifi.getResponse();

    // Extract the information sent by the server
    int beginDirection = response.indexOf(">") + 1;
    int endDirection = response.indexOf("<", 2);
    String theDirection = response.substring(beginDirection, endDirection);
    //Serial.print("Direction: ");
    //Serial.println(theDirection);

    count++;

    if (!(turnState) && theDirection == "U")
    {
      turnState = false;
      moveRover(theDirection);
    }
    else if (!(turnState) && theDirection == "D")
    {
      turnState = false;
      moveRover(theDirection);
    }
    else if (!(turnState) && theDirection == "L")
    {
      Serial.println("left");
      turnState = true;
      orientation -= 1;
      lastTurn = millis();
      analogWrite(spdL, turnspd);
      analogWrite(spdR, turnspd);
      digitalWrite(dirL, LOW);
      digitalWrite(dirR, HIGH);
    }
    else if (!(turnState) && theDirection == "R")
    {
      turnState = true;
      orientation += 1;
      lastTurn = millis();
      analogWrite(spdL, turnspd);
      analogWrite(spdR, turnspd);
      digitalWrite(dirL, HIGH);
      digitalWrite(dirR, LOW);
    }
    else if (!(turnState) && theDirection == "S")
    {
      turnState = false;
      stopRover();
    }

    /*
       Attempt at encoders for 90 degree turns
    */

    if (turnState)
    {
      encoderTics();
      stopRover();
      if (wifi.hasResponse()){
        wifi.getResponse();
        }
    }

       


    /*
        Previous code for 90 degree turns
    */
//        if (turnState && millis() - lastTurn > TURNPERIOD)
//        {
//          turnState = false;
//        }
  cmddur = millis();
  }

  if (millis() - lastSample > SAMPLINGPERIOD)
    // Take UltraSonic Sensor Readings
  {
    int trigPin = trigPinArray[arrayIndex % 4];
    int echoPin = trigPin + 1;
    ultraSonicDistance = ultraSonic(trigPin, echoPin);
    //Serial.println("Ultrasonic Distance: " + ultraSonicDistance);
    arrayIndex++;
    lastSample = millis();
  }
  if (!wifi.isBusy() && millis() - lastRequest > POLLPERIOD)
    // Receive vibrating motors command
  {
    String domain = "192.168.4.1";
    String path = "/";

    //String domain = "iesc-s2.mit.edu";
    //String path = "/hello.html";
    //Serial.println("Requesting");
    wifi.sendRequest(GET, domain, 80, path, ultraSonicDistance);
    //Serial.println("Request sent");
    lastRequest = millis();
  }
}

void encoderTics()
{
  while(encoderVal < encoderThreshold){
    int newEnc = digitalRead(encoderPinA);
    if ((encoderPinALast == LOW) && (newEnc == HIGH))
    {
      encoderVal++;
      Serial.println("encoder value: " + String(encoderVal));
     
    }
    encoderPinALast = newEnc;
  }
  turnState = false;
  encoderVal = 0;
}

void stopRover()
{
  analogWrite(spdL, 0);
  analogWrite(spdR, 0);
}

void moveRover(String movRoverDir)
{
  if (movRoverDir == "U")
  {
    analogWrite(spdL, constspd);
    analogWrite(spdR, constspd);
    digitalWrite(dirL, HIGH);
    digitalWrite(dirR, HIGH);
  }
  else if (movRoverDir == "D")
  {
    analogWrite(spdL, constspd);
    analogWrite(spdR, constspd);
    digitalWrite(dirL, LOW);
    digitalWrite(dirR, LOW);
  }
}

String ultraSonic(int trigPin, int echoPin)
{
  //The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //  if (pulse_state == 1)
  //  { //start state = 1
  //    digitalWrite(trigPin, LOW);
  //    pulseChange = micros();
  //    pulse_state = 2;
  //  }
  //  else if (pulse_state == 2 && micros() - pulseChange > 5) { //trigger state = 2
  //    digitalWrite(trigPin, HIGH);
  //    pulseChange = micros();
  //    pulse_state = 3;
  //  }
  //  else if (pulse_state == 3 && micros() - pulseChange > 10) { //end state = 3
  //    digitalWrite(trigPin, LOW);
  //    pulse_state = 1;
  //  }

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = (duration / 2) / 29.1;
  inchez = (duration / 2) / 74;

  String distance = ""; // Close, near, mid, far, or very far

  if (cm <= 20)
  {
    distance = "C";
  }
  else if (cm > 20 && cm <= 30)
  {
    distance = "N";
  }
  else if (cm > 30 && cm <= 35)
  {
    distance = "M";
  }
  else if (cm > 35 && cm <= 40)
  {
    distance = "F";
  }
  else
  {
    distance = "V";
  }

  String sensor = "";

  if (arrayIndex % 4 == 0)
  {
    sensor = "^";
  }
  else if (arrayIndex % 4 == 1)
  {
    sensor = ">";
  }
  else if (arrayIndex % 4 == 2)
  {
    sensor = "D";
  }
  else // arrayIndex == 3
  {
    sensor = "<";
  }
  return (distance + sensor);
}

