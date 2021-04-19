#include "SimpleSensorAlarm.h"

void SimpleSensorAlarm::setup(
            uint8_t id,
            bool enableHighAlarm, 
            float highThreshold, 
            bool enableLowAlarm,
            float lowThreshold,
            float diff,
            unsigned long timeDelay){
    
    _id = id;
    _enableHighAlarm = enableHighAlarm;
    _highThreshold = highThreshold;

    _enableLowAlarm = enableLowAlarm;
    _lowThreshold = lowThreshold;

    _diff = diff;
    _timeDelay = timeDelay;
}

void SimpleSensorAlarm::update(float sensorValue){
    if(_enableHighAlarm)_highAlarmCheck(sensorValue);

    //alarms have been disabled
    if(_alarmStatus == AL_HIGH_ALARM && !_enableHighAlarm){
        _alarmStatus = AL_NO_ALARM;
        _highBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }

    if(_enableLowAlarm) _lowAlarmCheck(sensorValue);
    //alarms have been disabled
    if(_alarmStatus == AL_HIGH_ALARM && !_enableLowAlarm){
        _alarmStatus = AL_NO_ALARM;
        _lowBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }
}

void SimpleSensorAlarm::setCallback(AlarmEventCB cb){
    alarmCallback = cb;
}

void SimpleSensorAlarm::unsetCallback(){
    alarmCallback = nullptr;
}

float SimpleSensorAlarm::getHighAlarmSetpoint(){
    return _highThreshold;
}

void SimpleSensorAlarm::setHighAlarmSetpoint(float highThreshold){
    _highThreshold = highThreshold;
}

float SimpleSensorAlarm::getLowAlarmSetpoint(){
    return _lowThreshold;
}

void SimpleSensorAlarm::setLowAlarmSetpoint(float lowThreshold){
    _lowThreshold = lowThreshold;
}

float SimpleSensorAlarm::getDiffSetpoint(){
    return _diff;
}

void SimpleSensorAlarm::setDiffSetpoint(float diff){
    _diff = diff;
}

void SimpleSensorAlarm::enableHighAlarm(){
    _enableHighAlarm = true;
}

void SimpleSensorAlarm::disableHighAlarm(){
    _enableHighAlarm = false;
}

void SimpleSensorAlarm::enableLowAlarm(){
    _enableLowAlarm = true;
}

void SimpleSensorAlarm::disableLowAlarm(){
    _enableLowAlarm = false;
}

unsigned long SimpleSensorAlarm::getTimeDelay(){
    return _timeDelay;
}

void SimpleSensorAlarm::setTimeDelay(unsigned long timeDelay){
    _timeDelay = timeDelay;
}

AlarmStatus SimpleSensorAlarm::getAlarmStatus(){
    return _alarmStatus;
}

void SimpleSensorAlarm::_highAlarmCheck(float sensorValue){
      //sensor is higher than high threshold and no current breach in progress
    if(_highBreachTime == 0 && sensorValue >= _highThreshold ){
        _highBreachTime = millis();
    }

    // sensor become lower than high threshold while no alarm is set
    if(_highBreachTime > 0 && sensorValue < _highThreshold && _alarmStatus == AL_NO_ALARM){
        _highBreachTime = 0;
    }

    //detect high threshold breach
    if(_highBreachTime > 0 && _alarmStatus == AL_NO_ALARM && millis() - _highBreachTime >= _timeDelay){
        _alarmStatus = AL_HIGH_ALARM;
        if(alarmCallback != nullptr) alarmCallback(_id, AL_HIGH_ALARM);
    }

    //use diff high
    if(_alarmStatus == AL_HIGH_ALARM && sensorValue < _highThreshold - _diff){
        _alarmStatus = AL_NO_ALARM;
        _highBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, AL_NO_ALARM);
    } 
}

void SimpleSensorAlarm::_lowAlarmCheck(float sensorValue){
    //sensor is lower than high threshold and no current breach in progress
    if(_lowBreachTime == 0 && sensorValue <= _lowThreshold){
        _lowBreachTime = millis();
    }

    // sensor becomes higher than low threshold while no alarm is set
    if(_lowBreachTime > 0 && sensorValue > _lowThreshold && _alarmStatus == AL_NO_ALARM){
        _lowBreachTime = 0;
    }

    //detect low threshold breach
    if(_lowBreachTime > 0 && _alarmStatus == AL_NO_ALARM && millis() - _lowBreachTime >= _timeDelay){
        _alarmStatus = AL_LOW_ALARM;
        if(alarmCallback != nullptr) alarmCallback(_id, AL_LOW_ALARM);
    }

    //use diff to stop alarm
    if(_alarmStatus == AL_LOW_ALARM && sensorValue > _lowThreshold + _diff){
        _alarmStatus = AL_NO_ALARM;
        _lowBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, AL_NO_ALARM);
    } 
}
