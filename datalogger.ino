#define END 0
#define START 1

int soundPin = 2;
int motionPin = 3;

volatile int soundFlag = false;
volatile int motionFlag = false;

volatile int soundEventType = END;
volatile int motionEventType = END;

void setup() {
    attachInterrupt(0, setSoundFlag, CHANGE);
    attachInterrupt(1, setMotionFlag, CHANGE);
}

void loop() {
    if(soundFlag | motionFlag) {
        writeToLog(soundFlag, motionFlag, soundEventType, motionEventType);
        soundFlag = false;
        motionFlag = false;
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
