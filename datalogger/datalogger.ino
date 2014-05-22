#define END 0
#define START 1
#define WAIT_TO_START 1 // Wait for serial input for debugging

#include <Wire.h>
#include "RTClib.h"
#include "SD.h"

const int soundPin = 2;
const int motionPin = 3;
const int chipSelect = 10;
const int ledPin = 13;

volatile int soundFlag = false;
volatile int motionFlag = false;

volatile int soundEventType = END;
volatile int motionEventType = END;

RTC_DS1307 RTC;
File logfile;

void setup() {
    Wire.begin();
    RTC.begin();

    if (!RTC.isrunning()) {
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    attachInterrupt(0, setSoundFlag, CHANGE);
    attachInterrupt(1, setMotionFlag, CHANGE);
    pinMode(chipSelect, OUTPUT);

    // Start serial for debugging
    Serial.begin(9600);
    Serial.println();

    #if WAIT_TO_START
    Serial.println("Type any character to start");
    while (!Serial.available());
    #endif //WAIT_TO_START
    
    Serial.println("Initializing SD card...");
    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
    }
    Serial.println("card initialized.");

    char filename[] = "testlog.csv";
    logfile = SD.open(filename, FILE_WRITE);

    if (! logfile) {
        Serial.println("Couldn't create file.");
    }

    Serial.print("Logging to: ");
    Serial.println(filename);

    logfile.println("millis,stamp,datetime,light,temp,vcc");
    Serial.println("millis,stamp,datetime,light,temp,vcc");
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
    // log time
    logfile.print(now.unixtime()); // seconds since 1/1/1970
    logfile.print(", ");
    logfile.print('"');
    logfile.print(now.year(), DEC);
    logfile.print("/");
    logfile.print(now.month(), DEC);
    logfile.print("/");
    logfile.print(now.day(), DEC);
    logfile.print(" ");
    logfile.print(now.hour(), DEC);
    logfile.print(":");
    logfile.print(now.minute(), DEC);
    logfile.print(":");
    logfile.print(now.second(), DEC);
    logfile.print('"');

    Serial.print(now.unixtime()); // seconds since 1/1/1970
    Serial.print(", ");
    Serial.print('"');
    Serial.print(now.year(), DEC);
    Serial.print("/");
    Serial.print(now.month(), DEC);
    Serial.print("/");
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.print('"');
}
