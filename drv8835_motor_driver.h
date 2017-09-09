#include "motor_driver.h"
#include <DRV8835MotorShield.h>

namespace myrobot
{
  /*
   * Create a MotorDriver ojbect to control the Pololu DRV8835 Motor Shield
   * 
   */
  class Motor : public MotorDriver
  {
    public:
      /*
       * Create an instance of the DRV8835 MotorShield
       * 
       */
      Motor(int pNumber) : MotorDriver(), motor(), motorNumber(pNumber), currentSpeed(0)
      {        
        if (motorNumber == 1){
          motor.flipM1(true);
          
        } else if (motorNumber == 2){
          motor.flipM2(true);          
        }
      }

      /*
       * Speed can be a value between -400 and 400
       * Positive values will make the motor go forward
       * Zero will make the motor stop
       * Negative values will make the motor go backwards
       * 
       */
      void setMotorSpeed(int pSpeed){
        currentSpeed = pSpeed;

        if (motorNumber == 1){
          motor.setM1Speed(currentSpeed);
          
        } else if (motorNumber == 2){
          motor.setM2Speed(currentSpeed);          
        }
      }

      int getSpeed() const{
        return currentSpeed;
      }

    private:
      DRV8835MotorShield motor;
      int motorNumber;
      int currentSpeed;
  };  
};
