namespace myrobot
{
  class DistanceSensorDriver
  {
    public:
      DistanceSensorDriver(unsigned long distance) : maxDistance(distance){}

      virtual unsigned long getDistance() = 0;

    protected:
      unsigned long maxDistance;
    
  };  
};

