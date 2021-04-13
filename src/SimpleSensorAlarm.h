#pragma once
#include "Arduino.h"

enum AlarmStatus{
    AL_NO_ALARM,
    AL_HIGH_ALARM,
    AL_LOW_ALARM,
};

class SimpleSensorAlarm {
    using AlarmEventCB = void (*)(uint8_t, AlarmStatus);
    
    public:
        void setup(
            uint8_t id,
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
        uint8_t _id;

        AlarmEventCB alarmCallback;

        void _highAlarmCheck(float);
        void _lowAlarmCheck(float);

        bool _enableHighAlarm;
        float _highThreshold = false;

        float _lowThreshold;
        bool _enableLowAlarm = false;

        float _diff = 0;

        unsigned long _timeDelay = 0;
        unsigned long _lowBreachTime = 0;
        unsigned long _highBreachTime = 0;
        AlarmStatus _alarmStatus = AL_NO_ALARM;
};
