#include "distance.h"
#include "Arduino.h"
#include "pin_config.h"

float distance_sensor_reading = 0;

void setup_ultrasonic(){
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}


void measure_distance_in_cm(){
    long duration;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG,LOW);

    duration = pulseIn(ECHO, HIGH);

    distance_sensor_reading = duration * 0.0343 / 2;

    Serial.print("distance:");
    Serial.println(distance_sensor_reading);
    // return(measured_distance);
}

