#include "pirSensor.h"

pirSensor::pirSensor(int pin){
	pirPin = pin;
}

//Sets the pin for the PIR
void pirSensor::initializePir(){
	pinMode(pirPin, INPUT_PULLUP);
}

int pirSensor::currentStatus(){
	return digitalRead(pirPin);
}

//Checks to see if the PIR has detected anything
bool pirSensor::isActive(){
	if(currentStatus() == 1){
		return true;
	}else{
		return false;
	}
}