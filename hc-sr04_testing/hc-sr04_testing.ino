//defining the trigger and echo pins for the proximity sensor
#define trig 12
#define echo 14

#define trigdur 10;

long timeTrig = millis();
long timeEcho = millis();
long distance = 0;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT_PULLUP);

  //set interrupt for echo pin
  attachInterrupt(digitalPinToInterrupt(echo), readDistance, FALLING);

}

void loop() {
  Serial.println("Triggering Now");
  //Triggering the sensor
  digitalWrite(trig, HIGH);
  //allowing 10uS for trigger
  delay(.01);
  //disabling the trigger
  digitalWrite(trig, LOW);
  //recording trigger time for distance calculation
  timeTrig = millis();
  //print out distance recorded from readDistance function
  Serial.println(String(distance) + " Inches");
  //allowing some time to check results
  delay(3000);
}

void readDistance() {
  timeEcho = millis();
  Serial.println("Time Triggered:");
  Serial.println(timeTrig);
  Serial.println("Time Echo'd");
  Serial.println(timeEcho);
  //distance = ((timeEcho - timeTrig) * 340) / 2;
  distance = ((timeEcho - timeTrig) * 1000) / 148;
}