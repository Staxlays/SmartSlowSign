#include "hcsr04.h"

hcsr04::hcsr04(int e, int t){
	echoPin = e;
	triggerPin = t;
}

void hcsr04::initializeSonar(){
	pinMode(echoPin, INPUT_PULLUP);
	pinMode(triggerPin, OUTPUT);
	distance = 0;
	echoTime = 0;
	pulseTime = 0;
	speed = 0;
}

//Sends the trigger pulse to the sonar then calculates the distance found
float hcsr04::triggerSonar(){
	//Serial.println("Now Triggering");

	//clear trigger pin
	digitalWrite(triggerPin, LOW);
	delay(.02);

	//trigger the sonar
	digitalWrite(triggerPin, HIGH);
	delay(.01);
	digitalWrite(triggerPin, LOW);

	//measure the length of the return pulse, used for distance calculation
	pulseTime = pulseIn(echoPin, HIGH);

	//take note of when the return pulse completed, used for speed calculation
	echoTime = millis();


	//calculating distance in cm
	distance = float(pulseTime/58);

	//calculating distance in M
	//distance = float(pulseTime/5800);

	//calculating distance in KM
	//distance = float(pulseTime/58000);


	//pulseTime multiplied by the speed of sound in M/S divided by 2
	//distance = float(pulseTime*340/2);


	//calculating distance in inches
	//distance = float(pulseTime/148);

	//delay of 60mS to avoid interference with other recordings
	delay(60);

	return distance;
}

float hcsr04::calculateSpeed(hcsr04&A, hcsr04&B){
	
	//DEBUG STATEMENTS
	//Serial.println("Delta Distance: " + String(abs(B.distance - A.distance)));
	//Serial.println("Delta Time: " + String(abs(B.echoTime - A.echoTime)));

	//if either distance is 0, there is no speed
	if(A.distance == 0 || B.distance == 0){
		speed = 0;
	}else{
		//units are cm/ms
		//speed = (abs((float((B.distance - A.distance)))/float(((B.echoTime - A.echoTime)))));

		//units are ms
		//speed = abs((float((B.distance - A.distance)))/float((B.echoTime - A.echoTime)));

		//units are hours
		//speed = (float(B.distance - A.distance))/(float(B.echoTime / 1000 / 3600 - A.echoTime / 1000 / 3600));

		// cm/ms * 36 = kmh
		//units are kmh, use cm formula for calulating distance then plug in below
		speed = abs((float((B.distance - A.distance)))/float((B.echoTime - A.echoTime))) * 36;

	}

	return speed;
}
