#include "quaternionFilters.h"
#include "MPU9250.h"
#include <Wifi_S08_v2.h>
#ifdef LCD
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7, 5, 6);
#endif // LCD
#define AHRS true         // Set to false for basic data read
#define SerialDebug false  // Set to true to get Serial output for debugging
#define SSID "HELLO"
#define PASSWD "MOMENTUM"
#define BLINKPERIOD 1000
#define UPDATEPERIOD 100
#define DATAPERIOD 100
const String HTML_START = "<html>";
const String HTML_END = "</html>";

ESP8266 wifi = ESP8266(1, false);
String html = "<html><p>Hi</p></html>";
int val = 0;
int light = 0;

MPU9250 myIMU;

// GENERAL VARIABLES
unsigned long lastupdate;
unsigned long blinktime;
unsigned long dataupdate;
char html2[500];

// VIBRATION MOTOR VARIABLES
unsigned long inits[4] = {millis(), millis(), millis(), millis()};
int spacings[4] = {0, 0, 0, 0};
bool vibeStates[4] = {false, false, false, false};
int duration = 300;

// ORIENTATION VARIABLES
int orientations[4] = {440, 311, 261, 370};
// orientations:       up, right, down, left
int orientation = 0;
int speakerPin = 9;
int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A
// mid C: left
// D# = forwards
// F# = backwards
// A = right

// LED PIN
int myLed  = 13;

// VIBRATION MOTOR PINS
int frontVib = 14;
int leftVib = 15;
int rightVib = 16;
int backVib = 17;
int vibMotors[4] = {frontVib, leftVib, rightVib, backVib};

// GLOBAL VARIABLES
String direction = "S";
int turnSpeed = 200;

// IMU VARIABLES
// Minimum and maximum values for a stationary IMU
float minYaw;
float maxYaw;
float minPitch;
float maxPitch;
float minRoll;
float maxRoll;

void setup()
{
  lastupdate = millis();
  blinktime = millis();
  dataupdate = millis();

  Wire.begin();
  // TWBR = 12;  // 400 kbit/sec I2C speed
  Serial.begin(115200);
  wifi.begin();
  Serial.println("startserver");
  wifi.startserver(SSID, PASSWD);
  Serial.println("setup complete");
  wifi.setPage("/", html);

  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);
  pinMode(frontVib, OUTPUT);
  pinMode(leftVib, OUTPUT);
  pinMode(backVib, OUTPUT);
  pinMode(rightVib, OUTPUT);

  calibration();
}


void loop()
{
  buzzVibrations();
  IMUInterpretation();
  UltraSonicDataInterpretation();
  teensyBlink();
  quaternions();
  val++;
}


void IMUInterpretation()
{
  int m = millis() - lastupdate;
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
  myIMU.count = millis();
  myIMU.sumCount = 0;
  myIMU.sum = 0;

  if (val == 0)
  {
    minYaw = myIMU.yaw;
    maxYaw = myIMU.yaw;
    minPitch = myIMU.pitch;
    maxPitch = myIMU.pitch;
    minRoll = myIMU.roll;
    maxRoll = myIMU.roll;
  }
  else if (val > 1000 && val < 4000)
  {
    if (myIMU.yaw < minYaw)
    {
      minYaw = myIMU.yaw;
    }
    else if (myIMU.yaw > maxYaw)
    {
      maxYaw = myIMU.yaw;
    }

    if (myIMU.pitch < minPitch)
    {
      minPitch = myIMU.pitch;
    }
    else if (myIMU.pitch > maxPitch)
    {
      maxPitch = myIMU.pitch;
    }

    if (myIMU.roll < minRoll)
    {
      minRoll = myIMU.roll;
    }
    else if (myIMU.roll > maxRoll)
    {
      maxRoll = myIMU.roll;
    }
  }
  else if (val == 3000)
  {
    float percentAdjustment = 0.50;
    float deltaPitch = maxPitch - minPitch;
    float deltaRoll = maxRoll - minRoll;

    minPitch = minPitch - percentAdjustment * deltaPitch;
    maxPitch = maxPitch + percentAdjustment * deltaPitch;
    minRoll = minRoll - percentAdjustment * deltaRoll;
    maxRoll = maxRoll + percentAdjustment * deltaRoll;
  }
  else
  {
    if (myIMU.roll > minRoll && myIMU.roll < maxRoll && myIMU.pitch > minPitch && myIMU.pitch < maxPitch)
    {
      direction = "S";
      makeNoise(orientation);
    }
    if (myIMU.pitch > maxPitch && myIMU.roll > minRoll && myIMU.roll < maxRoll)
    {
      noTone(speakerPin);
      direction = "D";
    }
    else if (myIMU.pitch < minPitch && myIMU.roll > minRoll && myIMU.roll < maxRoll)
    {
      noTone(speakerPin);
      direction = "U";
    }
    else if (myIMU.roll > maxRoll && myIMU.pitch > minPitch && myIMU.pitch < maxPitch)
    {
      noTone(speakerPin);
      direction = "L";
      orientation -= 1;
    }
    else if (myIMU.roll < minRoll && myIMU.pitch > minPitch && myIMU.pitch < maxPitch)
    {
      noTone(speakerPin);
      direction = "R";
      orientation += 1;
    }
    // otherwise: don't change the value of direction
    if (m > UPDATEPERIOD)
    {
      Serial.println(direction);

      String cmd = String(HTML_START + direction + HTML_END);
      //Serial.println(cmd);
      wifi.setPage("/", cmd);
      lastupdate = millis();
    }
  }
}

int getSpacing(String data)
{
  int spacing = 0;
  // Read distance to UltraSonic Sensor
  if (data.startsWith("C"))
  {
    spacing = 100;
  }
  else if (data.startsWith("N"))
  {
    spacing = 300;
  }
  else if (data.startsWith("M"))
  {
    spacing = 600;
  }
  else if (data.startsWith("F"))
  {
    spacing = 900;
  }
  else if (data.startsWith("V"))
  {
    spacing = 99999999;
  }
  return spacing;
}

void makeNoise(int dir)
{
  if (dir % 4 == 1 or dir % 4 == 2) // left or right
  {
    tone(speakerPin, orientations[orientation % 4]);
    tone(speakerPin, orientations[orientation % 4]);
  }
  else
  {
    tone(speakerPin, orientations[orientation % 4]);
  }
}

void UltraSonicDataInterpretation()
{
  int mm = millis() - dataupdate;
  if (mm > DATAPERIOD && wifi.hasData())
  {
    String data = wifi.getData();
    data.replace("?", "");
    data.trim();

    if (data != "\0")
    {
      Serial.println(data);
      // Determine which sensor the data is coming from
      if (data.substring(1) == "^")
      {
        spacings[0] = getSpacing(data);
      }
      else if (data.substring(1) == "<")
      {
        spacings[1] = getSpacing(data);
      }
      else if (data.substring(1) == ">")
      {
        spacings[2] = getSpacing(data);
      }
      else if (data.substring(1) == "D")
      {
        spacings[3] = getSpacing(data);
      }
    }
    dataupdate = millis();
  }
}


void buzzVibrations()
{
  for (int n = 0; n <= 3; n++)
  {
    if (millis() - inits[n] < duration )
    {
      if (vibeStates[n] == false)
      {
        digitalWrite(vibMotors[n % 4], HIGH);
        vibeStates[n] = true;
      }
    }
    else if (millis() - inits[n] < spacings[n] + duration)
    {
      if (vibeStates[n] == true)
      {
        digitalWrite(vibMotors[n % 4], LOW);
        vibeStates[n] = false;
      }
    }
    else
    {
      inits[n] = millis();
    }
  }
}


void teensyBlink()
{
  int m = millis() - blinktime;
  if (m > BLINKPERIOD)
  {
    if (light == 1)
    {
      Serial.println("blink");
      light = 0;
      digitalWrite(myLed, LOW);
    }
    else
    {
      light = 1;
      digitalWrite(myLed, HIGH);
    }
    blinktime = millis();
  }
}

void calibration()
{
  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
  Serial.print(" I should be "); Serial.println(0x73, HEX);
  if (c == 0x73) // WHO_AM_I should always be 0x71
  {
    Serial.println("MPU9250 is online...");

    // Start by performing self test and reporting values
    myIMU.MPU9250SelfTest(myIMU.SelfTest);
    Serial.print("x-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[0], 1); Serial.println("% of factory value");
    Serial.print("y-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[1], 1); Serial.println("% of factory value");
    Serial.print("z-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[2], 1); Serial.println("% of factory value");
    Serial.print("x-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[3], 1); Serial.println("% of factory value");
    Serial.print("y-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[4], 1); Serial.println("% of factory value");
    Serial.print("z-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[5], 1); Serial.println("% of factory value");

    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
    myIMU.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    Serial.println("MPU9250 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print("AK8963 ");
    Serial.print("I AM ");
    Serial.print(d, HEX);
    Serial.print(" I should be ");
    Serial.println(0x48, HEX);

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK8963(myIMU.factoryMagCalibration);
    // Initialize device for active mode read of magnetometer
    Serial.println("AK8963 initialized for active data mode....");

    // Get sensor resolutions, only need to do this once
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();

    // The next call delays for 4 seconds, and then records about 15 seconds of
    // data to calculate bias and scale.
    myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
    Serial.println("AK8963 mag biases (mG)");
    Serial.println(myIMU.magBias[0]);
    Serial.println(myIMU.magBias[1]);
    Serial.println(myIMU.magBias[2]);

    Serial.println("AK8963 mag scale (mG)");
    Serial.println(myIMU.magScale[0]);
    Serial.println(myIMU.magScale[1]);
    Serial.println(myIMU.magScale[2]);
    delay(2000); // Add delay to see results before serial spew of data
  } // if (c == 0x71)
  else
  {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
    while (1) ; // Loop forever if communication doesn't happen
  }
}

void quaternions()
{
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes; // - myIMU.accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes; // - myIMU.accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes; // - myIMU.accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
               * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
    myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
               * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
    myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
               * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

  // Must be called before updating quaternions!
  myIMU.updateTime();

  MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
                         myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
                         myIMU.mx, myIMU.mz, myIMU.deltat);



  // Serial print and/or display at 0.5 s rate independent of data rates
  myIMU.delt_t = millis() - myIMU.count;

  // Define output variables from updated quaternion---these are Tait-Bryan
  // angles, commonly used in aircraft orientation. In this coordinate system,
  // the positive z-axis is down toward Earth. Yaw is the angle between Sensor
  // x-axis and Earth magnetic North (or true North if corrected for local
  // declination, looking down on the sensor positive yaw is counterclockwise.
  // Pitch is angle between sensor x-axis and Earth ground plane, toward the
  // Earth is positive, up toward the sky is negative. Roll is angle between
  // sensor y-axis and Earth ground plane, y-axis up is positive roll. These
  // arise from the definition of the homogeneous rotation matrix constructed
  // from quaternions. Tait-Bryan angles as well as Euler angles are
  // non-commutative; that is, the get the correct orientation the rotations
  // must be applied in the correct order which for this configuration is yaw,
  // pitch, and then roll.
  // For more see
  // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
  // which has additional links.
  myIMU.yaw   = atan2(2.0f * (*(getQ() + 1) * *(getQ() + 2) + *getQ()
                              * *(getQ() + 3)), *getQ() * *getQ() + * (getQ() + 1)
                      * *(getQ() + 1) - * (getQ() + 2) * *(getQ() + 2) - * (getQ() + 3)
                      * *(getQ() + 3));
  myIMU.pitch = -asin(2.0f * (*(getQ() + 1) * *(getQ() + 3) - *getQ()
                              * *(getQ() + 2)));
  myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ() + 1) + * (getQ() + 2)
                              * *(getQ() + 3)), *getQ() * *getQ() - * (getQ() + 1)
                      * *(getQ() + 1) - * (getQ() + 2) * *(getQ() + 2) + * (getQ() + 3)
                      * *(getQ() + 3));
  myIMU.pitch *= RAD_TO_DEG;
  myIMU.yaw   *= RAD_TO_DEG;

  // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
  //   8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
  // - http://www.ngdc.noaa.gov/geomag-web/#declination
  myIMU.yaw  -= 8.5;
  myIMU.roll *= RAD_TO_DEG;
  //  Serial.print("Yaw, Pitch, Roll: ");
  //  Serial.print(myIMU.yaw, 2);
  //  Serial.print(", ");
  //  Serial.print(myIMU.pitch, 2);
  //  Serial.print(", ");
  //  Serial.println(myIMU.roll, 2);
}

