namespace myrobot
{
  class MotorDriver
  {
    public:
      MotorDriver(){};
      
      virtual void setMotorSpeed(int pSpeed) = 0;
  
      virtual int getSpeed() const = 0;
  };
};
