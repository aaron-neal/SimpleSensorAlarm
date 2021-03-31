#pragma once
#include "Arduino.h"

enum AlarmStatus{
    AL_NO_ALARM,
    AL_HIGH_ALARM,
    AL_LOW_ALARM,
};

class SimpleSensorAlarm {
    using AlarmEventCB = void (*)(AlarmStatus);
    
    public:
        void setup(
            bool enableHighAlarm, 
            float highThreshold, 
            bool enableLowAlarm,
            float lowThreshold,
            float diff,
            unsigned long timeDelay
        );
        void update(float sensorValue);
        
        void setCallback(AlarmEventCB cb);
        void unsetCallback();

        float getHighAlarmSetpoint();
        void setHighAlarmSetpoint(float);
        void enableHighAlarm();
        void disableHighAlarm();

        float getLowAlarmSetpoint();
        void setLowAlarmSetpoint(float);
        void enableLowAlarm();
        void disableLowAlarm();
    
        float getDiffSetpoint();
        void setDiffSetpoint(float);

        unsigned long getTimeDelay();
        void setTimeDelay(unsigned long);

        AlarmStatus getAlarmStatus();
    private:

        AlarmEventCB alarmCallback;

        void _highAlarmCheck(float);
        void _lowAlarmCheck(float);

        bool _enableHighAlarm;
        float _highThreshold;

        float _lowThreshold;
        bool _enableLowAlarm;

        float _diff;

        unsigned long _timeDelay;
        unsigned long _lowBreachTime;
        unsigned long _highBreachTime;
        AlarmStatus _alarmStatus;
};
