# SimpleSensorAlarm
An easy to use library for generating high / low alarms from sensors such as temperature and humidity

# NOTE
This library should still be considered as in development and should not be relied upon for critical systems.
Any improvements / suggestions welcome in PRs but will be considered on a case by case basis. 

# Usage
 - Include the library ```#include <SimpleSensorAlarm.h>```
 - Declare an instance or many ```SimpleSensorAlarm temperatureAlarm;```
 - In your setup, configure the instance, optionally use a callback function for alarm changes: 
   ```
   temperatureAlarm.setup(true, 18.0, true, -10.0, 0, 30000);
   temperatureAlarm.setCallback(temperatureAlarmCallback) // optional;
   ```
 - Update the instance with the latest sensor value ```temperatureAlarm.update(currentTemp);```
 - Use the callback function to handle alarms 
   ```
   void temperatureAlarmCallback(AlarmStatus _status){
    switch (_status){
        case AL_HIGH_ALARM:
            Serial.println("High Temperature Alarm!");
            break;
        case AL_LOW_ALARM:
            Serial.println("Low Temperature Alarm!");
            break;
        default:
            Serial.println("Alarm Cleared!");
            break;
       }
   }
   ```
   OR 
   call ```temperatureAlarm.getAlarmStatus()``` to check the status
   
   ```
   switch (temperatureAlarm.getAlarmStatus()){
        case AL_HIGH_ALARM:
            DEBUG_APP("High Temperature Alarm!");
            break;
        case AL_LOW_ALARM:
            DEBUG_APP("Low Temperature Alarm!");
            break;
        default:
            DEBUG_APP("Alarm Cleared!");
            break;
    }
    ```
