#include "SimpleSensorAlarm.h"

// we can create and monitor many alarm instances
// i.e. digitalInput, temperature, humidity etc...
SimpleSensorAlarm alarmMonitor[1]; 

#define INPUT_PIN 1 // set an input pin for a timed input alarm (i.e. from a door switch)

void alarmTriggerCB (uint8_t id, AlarmStatus newAlarmStatus){

    //When an alarm occurs this function will be called
    //You can react to the alarm type (high/low) and which alarm was raised
    switch (newAlarmStatus){
        case AL_HIGH_ALARM:
            Serial.printf("High Alarm, alarmID %d\n", _id);
            break;
        case AL_LOW_ALARM:
            Serial.printf("Low Alarm, alarmID %d\n", _id);
            break;
        default:
            Serial.printf("Alarm Cleared, alarmID %d\n", _id);
            break;
    }
}

void getTemperature(){
    // Add code here to return temperature input from your sensor
    return 15.5;
}

void setup(){
    // give your alarm a unique ID, this means all alarms can use the same callback
    alarmMonitor[0].setup(0, true, 21.5, true, -2, 0, 60000); // temperature based alarm (1 minute delay)
    alarmMonitor[0].setCallback(alarmTriggerCB);

    alarmMonitor[1].setup(1, true, 1, false, 0, 0, 30000); // digital input based alarm, i.e. door open alarm (30 second delay)
    alarmMonitor[1].setCallback(alarmTriggerCB);
}

void loop(){
    alarmMonitor[0].update(getTemperature()); // provide your latest temperature reading to the alarm instance
    alarmMonitor[1].update(digitalRead(INPUT_PIN)); // provide the latest digital input state to the alarm instance
}