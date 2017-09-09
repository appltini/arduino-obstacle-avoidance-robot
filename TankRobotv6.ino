#define ENABLE_ADAFRUIT_DRIVER
//#define ENABLE_DVR8835_DRIVER

#define LOGGING
#include "logging.h"

#include "moving_average.h"
#include "servo_distance_driver.h"

#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

#ifdef ENABLE_ADAFRUIT_DRIVER
  #include "adafruit_motor_driver.h"
  #define MAX_SPEED_FORWARD 255
  #define MAX_SPEED_BACKWARD -255
#endif

#ifdef ENABLE_DVR8835_DRIVER
  #include "drv8835_motor_driver.h"
  #define MAX_SPEED_FORWARD 400
  #define MAX_SPEED_BACKWARD -400
#endif

#define SPEED_STOP 0

#define TRIGGER_PIN A5
#define ECHO_PIN A4
#define MIN_DISTANCE 10
#define MAX_DISTANCE 300

#define RUN_TIME 30

#define SERVO_PIN 10

// Need to blink the LED
#define LED_PIN A3

namespace myrobot
{
  class TankRobot
  {
    public: 
      TankRobot() 
        : servoDistanceDriver(SERVO_PIN,TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE),
         distanceAverage(MAX_DISTANCE),
         leftMotor(LEFT_MOTOR), rightMotor(RIGHT_MOTOR){         
      }

      void initializeRobot(){
        servoDistanceDriver.initializeServo();
        endTime = millis() + (RUN_TIME * 1000);
        move();      
      }

      void run(){

        if (stopped()){
          return;
        }

        unsigned long currentTime = millis();
        unsigned long distance = distanceAverage.add(servoDistanceDriver.getDistanceStraight());
        log("state: %d, currentTime: %lu, avg distance: %lu \n", state, currentTime, distance);       
        
          
        if (doneRunning(currentTime)) {
          stop();
          
        } else if (moving()) {

          if (obstacleAhead(distance)){
            stop();
            backUp(currentTime);
          }
          
        } else if(backingUp()){
          if(doneBackingUp(currentTime)){
            stop();
            turn(currentTime);
          }
             
        } else if (turning()) {
          if (doneTurning(currentTime,distance)){
            move();
          }
        } 

        //blink the LED here
        if(doneBlinking(currentTime)){
          toggleLed(currentTime);
        }     
      }

      protected:

        bool moving() { return (state == stateMoving); }
        
        bool turning() { return (state == stateTurning); }
        
        bool stopped() { return (state == stateStopped); }

        bool backingUp() { return (state == stateBackingUp); }

        void toggleLed(unsigned long pTime){
          log("Blink LED\n");
          ledState = !ledState;
          digitalWrite(LED_PIN, ledState);

          endBlinkState = pTime + blinkInterval;
          
        }
  
        void move(){
          log("Move\n");
          leftMotor.setMotorSpeed(MAX_SPEED_FORWARD);
          rightMotor.setMotorSpeed(MAX_SPEED_FORWARD);
          state = stateMoving;
        }
  
        void stop(){
          log("Stop\n");
          leftMotor.setMotorSpeed(SPEED_STOP);
          rightMotor.setMotorSpeed(SPEED_STOP);
          state = stateStopped;
        }

        void backUp(unsigned long pTime){
          log("BackUp\n");
          leftMotor.setMotorSpeed(MAX_SPEED_BACKWARD);
          rightMotor.setMotorSpeed(MAX_SPEED_BACKWARD);

          endStateTime = pTime + 750;
          state = stateBackingUp;
        }
  
        void turn(unsigned long pTime){
          log("Turn\n");

          unsigned long left_distance = 0;
          for(int i=0; i<3; i++){
            left_distance += servoDistanceDriver.getDistanceLeft();
          }
          left_distance = left_distance/3;

          unsigned long right_distance = 0;
          for(int i=0; i<3; i++){
            right_distance += servoDistanceDriver.getDistanceRight();
          }
          right_distance = right_distance/3;
      
  
          log("LeftDistance: %lu, RightDistance: %lu \n", left_distance, right_distance);
  
          if (left_distance > right_distance){
            log("Turning Left\n");
            leftMotor.setMotorSpeed(MAX_SPEED_BACKWARD);
            rightMotor.setMotorSpeed(MAX_SPEED_FORWARD);
            
          }else{
            log("Turning Right\n");
            leftMotor.setMotorSpeed(MAX_SPEED_FORWARD);
            rightMotor.setMotorSpeed(MAX_SPEED_BACKWARD);          
          }

          servoDistanceDriver.rotateStraight();
  
          state = stateTurning;
          endStateTime = pTime + 1500;
        }

        bool obstacleAhead(unsigned long distance){       
          return (distance <= MIN_DISTANCE);
        }
  
        bool doneRunning(unsigned long pTime){
          return (pTime >= endTime);
        }

        bool doneTurning(unsigned long pTime, unsigned long distance){
          if (pTime >= endStateTime){
            return (distance > MIN_DISTANCE);
          }
          return false;
        }

        bool doneBackingUp(unsigned long pTime){
          return (pTime >= endStateTime);
        }

        bool doneBlinking(unsigned long pTime){
          return(pTime >= endBlinkState);
        }

  private:
    Motor leftMotor;
    Motor rightMotor;

    enum state_t {stateStopped, stateMoving, stateTurning, stateBackingUp};
    state_t state;

    unsigned long endStateTime;
    unsigned long endTime;

    //Need to blink the LED
    unsigned long blinkInterval = 1000;
    unsigned long endBlinkState = 0;
    bool ledState = false;

    MovingAverage<unsigned long, 3> distanceAverage;

    ServoDistanceDriver servoDistanceDriver;
  };  
};

myrobot::TankRobot tankRobot;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.flush();

  pinMode(LED_PIN, OUTPUT);

  tankRobot.initializeRobot();

  Serial.println("Starting in 1 seconds...");
  delay(1000);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  tankRobot.run();

}
