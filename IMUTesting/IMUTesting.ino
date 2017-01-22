//bring in some code to talk to the IMU!
#include <MPU9250.h>
#define GRAV 9.8*2
#define DEG 250

MPU9250 imu;

void setup() {
  Serial.begin(115200);
  delay(500); //wait 500ms
  Wire.begin();
  byte c = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
  Serial.print(" I should be "); Serial.println(0x73, HEX);
  if (c == 0x73) // WHO_AM_I   //IF YOU HAVE A BLUE IMU CHANGE TO 0x73
  {
    Serial.println("MPU9250 is online...");
       Serial.println("MPU9250 is online...");
    // Calibrate gyro and accelerometers, load biases in bias registers
    imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);
    } // if (c == 0x73)
  else
  {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
  }
}

void loop() {
  imu.readAccelData(imu.accelCount);
  float ax = imu.accelCount[0]/32768.0*GRAV;
  float ay = imu.accelCount[1]/32768.0*GRAV;
  float az = imu.accelCount[2]/32768.0*GRAV;
  imu.readGyroData(imu.gyroCount);
  float gx = imu.gyroCount[0]/32768.0*DEG;
  float gy = imu.gyroCount[1]/32768.0*DEG;
  float gz = imu.gyroCount[2]/32768.0*DEG;

  imu.pitch = imu.pitch + gx * .1;
  imu.roll = imu.roll + gy * .1;
  imu.yaw = imu.yaw + gz * .1;
  
  //feel free to print the accelerations and gyroscopic readings here.
  Serial.print("ax: ");
  Serial.println(ax);
  Serial.print("ay: ");
  Serial.println(ay);
  Serial.print("az: ");
  Serial.println(az);

  Serial.print("gx: ");
  Serial.println(gx);
  Serial.print("gy: ");
  Serial.println(gy);
  Serial.print("gz: ");
  Serial.println(gz);

  Serial.print("thetaX: ");
  Serial.println(imu.pitch);
  Serial.print("thetaY: ");
  Serial.println(imu.roll);
  Serial.print("thetaZ: ");
  Serial.println(imu.yaw);
  
  Serial.println();
  delay(100); //wait 100ms

}

