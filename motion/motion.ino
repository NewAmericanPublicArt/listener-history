int motion = 4;
int led = 13;

void setup() {
	pinMode(motion, INPUT);
	pinMode(led, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	if(digitalRead(motion)) {
		Serial.println("Found something!");
		digitalWrite(led, HIGH);
	} else {
		Serial.println("Nothing");
		digitalWrite(led, LOW);
	}
	delay(300);
}