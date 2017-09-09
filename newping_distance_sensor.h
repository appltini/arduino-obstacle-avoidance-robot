#include "distance_sensor.h"
#include <NewPing.h>

namespace myrobot
{
  class DistanceSensor : public DistanceSensorDriver
  {
    public:
      DistanceSensor(int triggerPin, int echoPin, unsigned long maxDistance)
        : DistanceSensorDriver(maxDistance), sensor(triggerPin, echoPin, maxDistance)
      {
      }

      /*
       * Return the distance in inches
       */
      virtual unsigned long getDistance(){
        unsigned long distance = sensor.ping_in();
        if (distance <= 0){
          return maxDistance;
        }

        return distance;        
      }

      private:
        NewPing sensor;
    
  };  
};

