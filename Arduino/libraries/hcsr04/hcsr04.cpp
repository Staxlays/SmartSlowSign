#include "hcsr04.h"

hcsr04::hcsr04(int e, int t){
	echoPin = e;
	triggerPin = t;
}

void hcsr04::initializeSonar(){
	pinMode(echoPin, INPUT_PULLUP);
	pinMode(triggerPin, OUTPUT);
	distance = 0;
	prevDistance = 0;
	echoTime = 0;
	prevEchoTime = 0;
	pulseTime = 0;
	deltaDistance = 0;
	deltaTime = 0;
	speed = 0;
}

float hcsr04::triggerSonar(){
	//Serial.println("Now Triggering");

	//clear trigger pin
	digitalWrite(triggerPin, LOW);
	delay(.02);

	//trigger the sonar
	digitalWrite(triggerPin, HIGH);
	delay(.01);
	digitalWrite(triggerPin, LOW);

	//measure the length of the return pulse
	pulseTime = pulseIn(echoPin, HIGH);

	//take note of when the return pulse completed
	echoTime = millis();

	//calculating distance in cm
	distance = pulseTime*0.034/2;

	//delay of 60mS to avoid interference with other recordings
	delay(60);

	return distance;
}

int hcsr04::currentStatus(){
	return digitalRead(echoPin);
}

float hcsr04::readDistance(){
  return distance;
}

float hcsr04::calculateSpeed(hcsr04&A, hcsr04&B){
	//if either distance is 0, there is no speed
	if(A.distance == 0 || B.distance == 0){
		return 0;
	}else{

		//units are cm/ms
		
		//DEBUG STATEMENTS
		//Serial.println("Delta Distance: " + String(abs(B.distance - A.distance)));
		//Serial.println("Delta Time: " + String(abs(B.echoTime - A.echoTime)));
		
		return (abs(((B.distance - A.distance))/float(((B.echoTime - A.echoTime)))));
	}
}
