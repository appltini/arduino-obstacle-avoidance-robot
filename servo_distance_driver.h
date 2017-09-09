#include <Servo.h>
#define ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER

#ifdef ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER
  #include "newping_distance_sensor.h"
#endif

namespace myrobot
{
  class ServoDistanceDriver
  {

    public:
      ServoDistanceDriver(int pServoPin, int pTriggerPin, int pEchoPin, int pMaxDistance)
        : distanceSensor(pTriggerPin, pEchoPin, pMaxDistance),
        servoPin(pServoPin){      
      }

      void initializeServo(){
        // Setup the Servo
        robotServo.attach(servoPin);
        robotServo.write(SERVO_POSITIONS[1]); 
        state = stateForward;      
 
      }

      unsigned long getDistanceLeft(){
        log("Check Left\n");

        if (state != stateLeft){
          robotServo.write(SERVO_POSITIONS[0]);
          delay(300);
        }

        state = stateLeft;
        
        return distanceSensor.getDistance();
      }

      unsigned long getDistanceRight(){
        log("Check Right\n");
        if (state != stateRight){
          robotServo.write(SERVO_POSITIONS[2]);
          delay(300);
        }

        state = stateRight;
        
        return distanceSensor.getDistance();        
      }

      unsigned long getDistanceStraight(){
        log("Check Straight\n");
        if (state != stateForward){
          robotServo.write(SERVO_POSITIONS[1]);
          delay(300);
        }

        state = stateForward;
        
        return distanceSensor.getDistance();        
      }

      void rotateLeft(){
        robotServo.write(SERVO_POSITIONS[0]);
        state = stateLeft;
      }

      void rotateRight(){
        robotServo.write(SERVO_POSITIONS[2]);
        state = stateRight;
      }

      void rotateStraight(){
        robotServo.write(SERVO_POSITIONS[1]);
        state = stateForward;
      }


    private:
      DistanceSensor distanceSensor;

      Servo robotServo;
      const int servoPin;
      const int SERVO_POSITIONS[3] = {45,90,135};

      enum servo_state {stateLeft, stateForward, stateRight};
      servo_state state;
    
    
  };  
};

