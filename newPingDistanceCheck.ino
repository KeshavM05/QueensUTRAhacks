// include newping library
#include "NewPing.h"

// define trigger and echo pins
int trigPin = 13; //supersonic sensor
int echoPin = 1; //supersonic sensor

// max distance to ping for (cm)
int maxDistance = 30;

//set up newping object
NewPing sonar(trigPin, echoPin, maxDistance);

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.print("Distance = ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
  delay(500);
}