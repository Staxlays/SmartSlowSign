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
	triggerTime = 0;
	deltaDistance = 0;
	deltaTime = 0;
	speed = 0;
}

void hcsr04::triggerSonar(){
	triggerTime = millis();
	digitalWrite(triggerPin, HIGH);
	delay(.01);
	digitalWrite(triggerPin, LOW);
}

int hcsr04::currentStatus(){
	return digitalRead(echoPin);
}

float hcsr04::readDistance(){
	prevEchoTime = echoTime;
	echoTime = millis();
	prevDistance = distance;
  	distance = ((echoTime - triggerTime) * 1000) / 148;
  	delay(2000);
  	return distance;
}

float hcsr04::calculateSpeed(float distance, float prevDistance, long echoTime, long prevEchoTime){
  	if((prevEchoTime >= echoTime - 7000) && (prevDistance != distance) && (prevDistance != 0)){
        if(prevDistance >= distance){
          deltaDistance = prevDistance - distance;
        }else{
          deltaDistance = distance - prevDistance;
        }
        deltaTime = (echoTime - prevEchoTime) / 1000;
        //Serial.println("deltaDistance: " + String(deltaDistance) + " deltaTime: " + String(deltaTime));
        speed = deltaDistance / float(deltaTime);
        //Serial.println("distancePrev : " + String(prevDistance) + " distance: " + String(distance) + " timeEchoPrev: " + String(prevEchoTime) + " timeEcho: " + String(echoTime));
        //Serial.println("Speed is: " + String(speed) + "in/S");
        //prevEchoTime = echoTime;
        //prevDistance = distance;
        //delay(2000);
        return speed;
    }else{
       //Serial.println("Distance is: " + String(distance));
       //prevEchoTime = echoTime;
       //prevDistance = distance;
       //delay(2000);
       return 0;
    }
}
