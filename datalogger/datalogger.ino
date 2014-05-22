#define END 0
#define START 1

#include <Wire.h>
#include "RTClib.h"
#include "SD.h"

const int soundPin = 2;
const int motionPin = 3;
const int ledPin = 13;

volatile int soundFlag = false;
volatile int motionFlag = false;

volatile int soundEventType = END;
volatile int motionEventType = END;

RTC_DS1307 RTC;

void setup() {
    Wire.begin();
    RTC.begin();

    if (!RTC.isrunning()) {
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    attachInterrupt(0, setSoundFlag, CHANGE);
    attachInterrupt(1, setMotionFlag, CHANGE);
    
    // need to add code to create new logfile on SD card at power-up
}

void loop() {
    if(soundFlag | motionFlag) {
        digitalWrite(ledPin, HIGH);
        writeToLog(soundFlag, motionFlag, soundEventType, motionEventType);
        soundFlag = false;
        motionFlag = false;
        delay(500); // to ensure the LED flash is visible and we don't write insanely fast
        digitalWrite(ledPin, LOW);
    }
}

void setSoundFlag() {
    soundFlag = true;
    if(digitalRead(soundPin)) {
        soundEventType = START;
    } else {
        soundEventType = END;
    }
}

void setMotionFlag() {
    motionFlag = true;
    if(digitalRead(motionPin)) {
        motionEventType = START;
    } else {
        motionEventType = END;
    }
}

void writeToLog(int soundFlag, int motionFlag, int soundEventType, int motionEventType) {
    DateTime now = RTC.now();
    // need to add code to actually write to SD card
}
