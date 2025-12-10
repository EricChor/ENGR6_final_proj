#include "bluetooth.h"
#include "MicroBlue.h"
#include "SoftwareSerial.h"
#include "pin_config.h"
#include "Arduino.h"

int x_axis = 0;
int y_axis = 0;

bool store_calibration_pressed = false;
bool store_max_values_pressed = false;
bool start_autonomous_pressed = false;
bool print_accel_values_pressed = false;

SoftwareSerial SSerial(bluetooth_RX,bluetooth_TX);

MicroBlueManager manager (SSerial);

void setup_bluetooth(){
    SSerial.begin(9600);
}

bool update_bluetooth(){
    MicroBlueMessage msg = manager.read();

    if (msg.id == "d1"){
        sscanf(msg.value.c_str(), "%d,%d", &x_axis, &y_axis);
        x_axis -= 512;
        y_axis -= 512;
    }

    if(msg.id == "b0"){
        if(msg.value == "1"){
            store_calibration_pressed = true;
        } else {
            store_calibration_pressed = false;
        }
    }

    if(msg.id == "b1"){
        if(msg.value == "1"){
            store_max_values_pressed = true;
        } else {
            store_max_values_pressed = false;
        }
    }

    if(msg.id == "b2"){
        if(msg.value == "1"){
            start_autonomous_pressed = true;
        } 
    }

    if(msg.id == "b3"){
        if(msg.value == "1"){
            print_accel_values_pressed = true;
        } else {
            print_accel_values_pressed = false;
        }
    }


    return true;

}