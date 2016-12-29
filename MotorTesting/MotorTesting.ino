float duty_cycle = .5;     //% in decimal form 
int period = 255;  //period in milliseconds

int incomingByte = 0; 

void setup() {
  // put your setup code here, to run once:
  //pinMode(9, OUTPUT);   //ENABLE PIN: 0-forward, 1-backward
  pinMode(10, OUTPUT);   //PWM SPD CONTROL PIN: duty cycle sets speed
  digitalWrite(10, LOW);  //foward
  analogWrite(9, period*duty_cycle);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                if(incomingByte == 115){  //s
                    Serial.println("Set a duty cycle between 0 and 9: ");
                    do{
                      duty_cycle = (Serial.read()-48);
                      }
                    while(duty_cycle < 0 || duty_cycle > 9);
                    duty_cycle = duty_cycle/10.0;
                    Serial.println(duty_cycle);
                    analogWrite(9, period*duty_cycle);
                  }
                else if(incomingByte == 98){ //b
                  Serial.println("Switching direction");
                  digitalWrite(10, HIGH); 
                }
                else if(incomingByte == 102){ //f
                  Serial.println("Switching direction");
                  digitalWrite(10, LOW);
                }
                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte);
        }
}
