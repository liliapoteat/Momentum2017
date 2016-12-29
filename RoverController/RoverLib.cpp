#include "RoverLib.h"
#include <Arduino.h>

//Rover * Rover::_instance;

Rover::Rover(int RWEP, int RWPP, int LWEP, int LWPP) {
    init(RWEP, RWPP, LWEP, LWPP, false);
}

Rover::Rover(int RWEP, int RWPP, int LWEP, int LWPP, bool enableEncoders) {
  init(RWEP, RWPP, LWEP, LWPP, enableEncoders);
}

void Rover::init(int RWEP, int RWPP, int LWEP, int LWPP, bool enableEncoders){
  /*Initializes all important variables for the rover
      -PID => used to determine if PID will be used
      -encoders => are the encoder pins set?
      -R/L pins => setup the relevant wheel control pins
  */
  PID = enableEncoders;
  encoders = false;
  
  R_enablePin = RWEP;
  L_enablePin = LWEP;
  R_phasePin = RWPP;
  L_phasePin = LWPP;

  pinMode(R_phasePin, OUTPUT);
  pinMode(L_phasePin, OUTPUT);
  
  digitalWrite(R_phasePin, 0);
  digitalWrite(L_phasePin, 1);
    
  }

void Rover::begin(){
  if(DEBUG){
    Serial.begin(9600);
    Serial.println("Initializing Momentum Rover...");
    Serial.print("right speed control pin: ");
    Serial.println(R_enablePin);
    Serial.print("Right direction control pin: ");
    Serial.println(R_phasePin);
    Serial.print("Left speed control pin: ");
    Serial.println(L_enablePin);
    Serial.print("Left direction control pin: ");
    Serial.println(L_phasePin);
    Serial.flush();
    }
  }

void Rover::init_encoders(int REI, int REII, int LEI, int LEII){
  R_encoderPin_One = REI;
  R_encoderPin_Two = REII;
  L_encoderPin_One = LEI;
  L_encoderPin_Two = LEII;
  encoders = true;
  if(DEBUG){
    Serial.println("Initializing Encoders...");
    Serial.print("Right encoder one: ");
    Serial.println(REI);
    Serial.print("Right encoder two: ");
    Serial.println(REII);
    Serial.print("Left encoder one: ");
    Serial.println(LEI);
    Serial.print("Left encoder two: ");
    Serial.println(LEII);
    }
  }

void Rover::set_LWV(int vel){
  if(DEBUG){
    Serial.print("Left wheel velocity: ");
    Serial.println(vel);
    }
    float duty = vel/MAX_VEL;
    if (duty > 0){
        digitalWrite(L_phasePin, 1);
      } 
    else {
        digitalWrite(L_phasePin, 0);
      }
    analogWrite(L_enablePin, PERIOD*abs(duty));
  }

void Rover::set_RWV(int vel){
  if(DEBUG){
    Serial.print("Right wheel velocity: ");
    Serial.println(vel);
    }
    float duty = vel/MAX_VEL;
    if (duty > 0){
        digitalWrite(R_phasePin, 0);
      } 
    else {
        digitalWrite(R_phasePin, 1);
      }
    analogWrite(R_enablePin, PERIOD*abs(duty));
  }
