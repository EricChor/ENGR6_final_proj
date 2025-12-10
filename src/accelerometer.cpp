#include <EEPROM.h>
#include "pin_config.h"
#include "Arduino.h"
#include "bluetooth.h"
#include <math.h>

void update_accel_vals();

void setup_accelerometer(){
    pinMode(X_OUT,INPUT);
    pinMode(Y_OUT,INPUT);
}

int xRawMin = 1023;
int xRawMax = 0;
int xRaw = 0;

int yRawMin = 1023;
int yRawMax = 0;
int yRaw = 0;

float x_accel = 0;
float y_accel = 0;

float max_stored_x_accel = 0;
float max_stored_y_accel = 0;

const int sampleSize = 10;

void auto_calibrate(int xNew, int yNew){
    if(xNew < xRawMin){
        xRawMin = xNew;
    }

    if(xNew > xRawMax){
        xRawMax = xNew;
    }

    if(yNew < yRawMin){
        yRawMin = yNew;
    }

    if(yNew > yRawMax){
        yRawMax = yNew;
    }
}

void load_calibration_from_mem(){
    EEPROM.get(0,xRawMin);
    EEPROM.get(sizeof(int),xRawMax);
    EEPROM.get(2*sizeof(int),yRawMin);
    EEPROM.get(3*sizeof(int),yRawMax);
}

void store_calibration_in_mem(){
    EEPROM.put(0,xRawMin);
    EEPROM.put(sizeof(int),xRawMax);
    EEPROM.put(2*sizeof(int),yRawMin);
    EEPROM.put(3*sizeof(int),yRawMax);

}
void print_calibration_vals(){
    Serial.print("Calibration vals: ");
    Serial.print(xRawMin);
    Serial.print(" ");
    Serial.print(xRawMax);
    Serial.print(" ");
    Serial.print(yRawMin);
    Serial.print(" ");
    Serial.print(yRawMax);
    Serial.print(" ");
}

int read_axis(int axisPin){
    long reading = 0;
    analogRead(axisPin);
    delay(10);
    for (int i = 0; i < sampleSize; i++) {
        reading += analogRead(axisPin);
        delay(1);
    }
    return reading / sampleSize;
}


void print_accel_calibration(){
    Serial.print("X min:");
    Serial.print(xRawMin);
    Serial.print(" ");
    Serial.print("X max:");
    Serial.print(xRawMax);
    Serial.print(" ");
    Serial.print("Y min:");
    Serial.print(yRawMin);
    Serial.print(" ");
    Serial.print("Y max:");
    Serial.print(yRawMax);
    Serial.print("\n");
}

void calibrate(){
    update_accel_vals();
    
    Serial.println("Start Calibration | align X-axis up");
    while(!store_calibration_pressed){
        update_bluetooth();
    }
        update_accel_vals();
        auto_calibrate(xRaw,yRaw);
        delay(1000);
        update_bluetooth();
    
    Serial.println("Start Calibration | align X-axis down");
    while(!store_calibration_pressed){
        update_bluetooth();
    }
        update_accel_vals();
        auto_calibrate(xRaw,yRaw);
        delay(1000);
        update_bluetooth();

    Serial.println("Start Calibration | align Y-axis up");
    while(!store_calibration_pressed){
        update_bluetooth();
    }
        update_accel_vals();
        auto_calibrate(xRaw,yRaw);
        delay(1000);
        update_bluetooth();

    Serial.println("Start Calibration | align Y-axis down");
    while(!store_calibration_pressed){
        update_bluetooth();
    }
        update_accel_vals();
        auto_calibrate(xRaw,yRaw);
        delay(1000);
        update_bluetooth();

    Serial.println("Finished Calibration");
    store_calibration_in_mem();
}

void compare_to_stored(float x_accel_in, float y_accel_in){
    if (fabs(x_accel_in) > max_stored_x_accel){
        max_stored_x_accel = fabs(x_accel_in);
    }

    if (fabs(y_accel_in) > max_stored_y_accel){
        max_stored_y_accel = fabs(y_accel_in);
    }
}

void store_accel_to_EEPROM(){
    EEPROM.put(3*sizeof(int)+sizeof(float),max_stored_x_accel);
    EEPROM.put(3*sizeof(int)+2*sizeof(float),max_stored_y_accel);
}

void print_accel_from_EEPROM(){
    float max_x = 0;
    float max_y = 0;
    EEPROM.get(3*sizeof(int)+sizeof(float),max_x);
    EEPROM.get(3*sizeof(int)+2*sizeof(float),max_y);
    Serial.print("Max X : ");
    Serial.print(max_x);
    Serial.print(" | Max Y : ");
    Serial.println(max_y);
}

void update_accel_vals(){
    xRaw = read_axis(X_OUT);
    yRaw = read_axis(Y_OUT);

    long x_scaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
    long y_scaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);    

    x_accel = x_scaled / 1000.0;
    y_accel = y_scaled / 1000.0;

    compare_to_stored(x_accel,y_accel);

    if(store_max_values_pressed){
        store_accel_to_EEPROM();
    }
    
    if(print_accel_values_pressed){
        print_accel_from_EEPROM();
    }

} 

