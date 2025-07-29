#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// components
#define MPR_ADDR 0x5A
#define OLED_ADDR 0x3c

// oled screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &&Wire, OLED_RESET);


int touches = 0;

void setup(){
    Serial.begin(9600);
    Wire.begin();

    display.cleayDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    delay(100);

    initMPR();
}

//Initalize Touch Sensor
void initMPR() {
    // soft reset
    Wire.beginTransmission(MPR_ADDR);
    Wire.write(0x80); // sleep mode address
    Wire.write(0x00); // start sensor
    Wire.endTransmission();

    //ElE0 touch
    Wire.beginTransmission(MPR_ADDR);
    Wire.write(0x41); //tou address
    Wire.write(12); // senstivity
    Wire.endTransmission();

    //ELE0 release
    Wire.beginTransmission(MPR_ADDR);
    Wire.write(0x42); // rel address
    Wire.write(6); // sensitivity
    Wire.endTransmission();

    //Only look at ELE0
    Wire.begintransmission(MPR_ADDR);
    Wire.write(0x5E); 
    Wire.write(0x01);
    Wire.endTransmission();
}

//to read target
bool readTouch() {

    // read value
    Wire.beginTransmission(MPR_ADDR);
    Wire.write(0x00);
    Wire.endTransmission(false);

    Wire.requestFrom(MPR_ADDR, 2);

    if (Wire.available() < 2) {
        return false;  // if no 2 bits then I don't want it
    }

    int16_t sensTouch = Wire.read();
    sensTouch |= ((itn16_t)Wire.read() << 8); // turns into 16 bit 

    return sensTouch & 0x01;// true! twas touched
}


void loop() {;
    
    if (readTouch()) {
        touches = touches + 1;
    }
    display.clear();
    display.setCursor(0,0);
    display.print("Times Touched");
    display.setCursor(0,1);
    display.print(touches);
}
