#include <Arduino.h>

#define MAX_VEL 25.0 // cm per second at 7.2 volts
#define MIN_VEL -25.0

#define PERIOD 225

//allow serial communication and print statements
#define DEBUG true

class Rover {
  /*
   * Rover Class -> Written by Mark Chounlakone for MIT IAP course Momentum
   * 
   * This class is used to abstract the challenge of controlling the
   * robot chassis remotely.
   * 
   * Arguments:
   * RWEP - Right Wheel Enable Pin is used to command the right wheel
   *        velocity via PWM; 
   *        
   *        longer on state => faster rotation
   * 
   * RWPP - Right Wheel Phase Pin is used to command the direction
   *        of rotation; 
   *        
   *        LOW = clockwise looking from rightside view
   *        HIGH = counterclockwise looking from rightside view
   * 
   * LWEP - Left Wheel Enable Pin analogous to RWEP
   * 
   * LWPP - Left Wheel Phase Pin analogous to RWPP
   * 
   * 
   */
  public:
    Rover(int RWEP, int RWPP, int LWEP, int LWPP);
    Rover(int RWEP, int RWPP, int LWEP, int LWPP, bool enableEncoders);
    void begin();
    void init_encoders(int REI, int REII, int LEI, int LEII);
    void set_LWV(int vel);
    void set_RWV(int vel);
  
  private:
  //initialization variables and functions
    void init(int RWEP, int RWPP, int LWEP, int LWPP, bool enableEncoders);

    //Wheel command pins
    int R_enablePin;
    int L_enablePin;
    int R_phasePin;
    int L_phasePin;

    //encoder initialization
    bool PID;
    bool encoders;
    
    int R_encoderPin_One;
    int R_encoderPin_Two;
    int L_encoderPin_One;
    int L_encoderPin_Two;
  };

  
