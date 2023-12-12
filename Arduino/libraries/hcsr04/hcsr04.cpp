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
	digitalWrite(triggerPin, HIGH);
	delay(.01);
	digitalWrite(triggerPin, LOW);
	triggerTime = millis();
	//delay(60);
	//while(digitalRead(echoPin) == 0){};
	/*if(digitalRead(echoPin) == 1){
		while(digitalRead(echoPin) == 1){
		//waiting for the return pulse (proportional to the distance recorded) to end
		//delay(1);
		}
	}*/
	//echoTime = millis();
	//delay(2000);
}

int hcsr04::currentStatus(){
	return digitalRead(echoPin);
}

float hcsr04::readDistance(){
	//units are inches

	//while(digitalRead(echoPin) == 1){
		//waiting for the return pulse to end
	//}
	echoTime = millis();
	//prevEchoTime = echoTime;
	//echoTime = millis();
	//prevDistance = distance;
  distance = ((echoTime - triggerTime) * 1000) / 148;
  //delay(60);
  //delay(2000);
  return distance;
}

float hcsr04::calculateSpeed(hcsr04&A, hcsr04&B){
	//units are mph
	if(A.distance == 0 || B.distance == 0){
		return 0;
	}else{
		return (abs(((B.distance - A.distance)/63660)/((B.echoTime - A.echoTime)/3600000)));
	}


}
/*
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
*/
