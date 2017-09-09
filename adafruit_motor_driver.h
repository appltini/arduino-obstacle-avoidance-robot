#include "motor_driver.h"
#include <stdlib.h>
#include <AFMotor.h>

namespace myrobot
{
  /*
   * Create a MotorDriver ojbect to control the Adafruit Motor Shield (or smilar)
   * 
   */
  class Motor : public MotorDriver
  {
    public:
      Motor(int pNumber) : MotorDriver(), motor(pNumber), currentSpeed(0)
      {    
      }

      /*
       * Speed can be a value between -255 and 255
       * Positive values will make the motor go forward
       * Zero will make the motor stop
       * Negative values will make the motor go backwards
       * 
       * 
       */
      void setMotorSpeed(int pSpeed){
        currentSpeed = pSpeed;
        motor.setSpeed(abs(currentSpeed));   
             
        if (currentSpeed == 0) {
          motor.run(RELEASE);          
          
        } else if (currentSpeed > 0) {          
          motor.run(FORWARD);
          
        } else {          
          motor.run(BACKWARD);
          
        }        
      }

      int getSpeed() const{
        return currentSpeed;
      }

    private:
      AF_DCMotor motor;
      int currentSpeed;
  };  
};

