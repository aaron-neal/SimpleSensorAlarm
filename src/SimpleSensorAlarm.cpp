#include "SimpleSensorAlarm.h"

void SimpleSensorAlarm::setup(
            uint8_t id,
            bool EnableHighAlarm, 
            float highThreshold, 
            bool EnableLowAlarm,
            float lowThreshold,
            float diff,
            unsigned long timeDelay){
    
    _id = id;
    EnableHighAlarm ? enableHighAlarm() : disableHighAlarm();
    EnableLowAlarm ? enableLowAlarm() : disableLowAlarm();
    setHighAlarmSetpoint(highThreshold);
    setLowAlarmSetpoint(lowThreshold);
    setDiffSetpoint(diff);
    setTimeDelay(timeDelay);
}

void SimpleSensorAlarm::update(float sensorValue){
    _sensorValue = sensorValue;

    if(_enableHighAlarm)_highAlarmCheck(_sensorValue);

    //alarms have been disabled
    if(_alarmStatus == AL_HIGH_ALARM && !_enableHighAlarm){
        _alarmStatus = AL_NO_ALARM;
        _highBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }

    if(_enableLowAlarm) _lowAlarmCheck(_sensorValue);
    //alarms have been disabled
    if(_alarmStatus == AL_LOW_ALARM && !_enableLowAlarm){
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
    if(_highThreshold == highThreshold) return; // No change
    if(highThreshold <= _lowThreshold) return; // Invalid high threshold
    //if in alarm and (new threshold is lower, or higher but still below the sensor value) 
    bool staysInAlarm = (_alarmStatus == AL_HIGH_ALARM && (highThreshold <= _highThreshold || _sensorValue > highThreshold));

    log_i("Setting high threshold to %f\n (continue alarm %d)", highThreshold, staysInAlarm);

    _highThreshold = highThreshold;
    _highBreachTime = 0;

    if(_alarmStatus == AL_HIGH_ALARM){
        _alarmStatus = AL_NO_ALARM;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }
}

float SimpleSensorAlarm::getLowAlarmSetpoint(){
    return _lowThreshold;
}

void SimpleSensorAlarm::setLowAlarmSetpoint(float lowThreshold){
    if(_lowThreshold == lowThreshold) return; // No change
    if(lowThreshold >= _highThreshold) return; // Invalid low threshold
    //if in alarm and (new threshold is higher, or lower but still above the sensor value) 
    bool staysInAlarm = (_alarmStatus == AL_LOW_ALARM && (lowThreshold >= _lowThreshold || _sensorValue < lowThreshold));

    log_i("Setting low threshold to %f\n (continue alarm %d)", lowThreshold, staysInAlarm);
    
    _lowThreshold = lowThreshold;
    _lowBreachTime = 0;
    
    if(!staysInAlarm && _alarmStatus == AL_LOW_ALARM){
        _alarmStatus = AL_NO_ALARM;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }
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
    log_i("High Alarm Check: Sensor Value: %f, High Threshold: %f, Diff: %f, Time Delay: %lu, Current Status: %d\n", sensorValue, _highThreshold, _diff, _timeDelay, _alarmStatus);
      //sensor is higher than high threshold and no current breach in progress
    if(_highBreachTime == 0 && sensorValue >= _highThreshold ){
        log_i("High threshold breach detected. Sensor Value: %f, High Threshold: %f\n", sensorValue, _highThreshold);
        _highBreachTime = millis();
    }

    // sensor become lower than high threshold while no alarm is set
    if(_highBreachTime > 0 && sensorValue < _highThreshold && _alarmStatus == AL_NO_ALARM){
        log_i("High threshold breach cleared before alarm. Sensor Value: %f, High Threshold: %f\n", sensorValue, _highThreshold);
        _highBreachTime = 0;
    }

    //detect high threshold breach
    if(_highBreachTime > 0 && _alarmStatus == AL_NO_ALARM && millis() - _highBreachTime >= _timeDelay){
        log_i("High threshold alarm triggered! Sensor Value: %f, High Threshold: %f\n", sensorValue, _highThreshold);
        _alarmStatus = AL_HIGH_ALARM;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }

    //use diff high
    if(_alarmStatus == AL_HIGH_ALARM && sensorValue < _highThreshold - _diff){
        log_i("High threshold alarm cleared. Sensor Value: %f, High Threshold: %f\n", sensorValue, _highThreshold);
        _alarmStatus = AL_NO_ALARM;
        _highBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    } 
}

void SimpleSensorAlarm::_lowAlarmCheck(float sensorValue){
    log_i("Low Alarm Check: Sensor Value: %f, Low Threshold: %f, Diff: %f, Time Delay: %lu, Current Status: %d\n", sensorValue, _lowThreshold, _diff, _timeDelay, _alarmStatus);
    //sensor is lower than high threshold and no current breach in progress
    if(_lowBreachTime == 0 && sensorValue <= _lowThreshold){
        log_i("Low threshold breach detected. Sensor Value: %f, Low Threshold: %f\n", sensorValue, _lowThreshold);
        _lowBreachTime = millis();
    }

    // sensor becomes higher than low threshold while no alarm is set
    if(_lowBreachTime > 0 && sensorValue > _lowThreshold && _alarmStatus == AL_NO_ALARM){
        log_i("Low threshold breach cleared before alarm. Sensor Value: %f, Low Threshold: %f\n", sensorValue, _lowThreshold);
        _lowBreachTime = 0;
    }

    //detect low threshold breach
    if(_lowBreachTime > 0 && _alarmStatus == AL_NO_ALARM && millis() - _lowBreachTime >= _timeDelay){
        log_i("Low threshold alarm triggered! Sensor Value: %f, Low Threshold: %f\n", sensorValue, _lowThreshold);
        _alarmStatus = AL_LOW_ALARM;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    }

    //use diff to stop alarm
    if(_alarmStatus == AL_LOW_ALARM && sensorValue > _lowThreshold + _diff){
        log_i("Low threshold alarm cleared. Sensor Value: %f, Low Threshold: %f\n", sensorValue, _lowThreshold);
        _alarmStatus = AL_NO_ALARM;
        _lowBreachTime = 0;
        if(alarmCallback != nullptr) alarmCallback(_id, _alarmStatus);
    } 
}
