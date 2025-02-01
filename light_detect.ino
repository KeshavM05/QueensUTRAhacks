// TCS230 Color Sensor with Arduino
// Light flashes once for red, twice for blue

#include <Arduino.h>

// Pin Definitions
int S0 = 7;
int S1 = 5;
int S2 = 6;
int S3 = 4;
int sensorOut = 8;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);


  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  pinMode(sensorOut,INPUT);

  Serial.begin(9600);
}

void loop() {
  int red=getRed();
  int blue=getBlue();
  int green=getGreen();

  Serial.print(" - Red= ");
	Serial.print(red);
	Serial.print(" - Green= ");
	Serial.print(green);
	Serial.print(" - Blue= ");
	Serial.println(blue);

  if (red <500 && blue <500 && green <500){
    if (red<blue && red<green){
      Serial.print("Red detected");
    }
    else if (blue<red && blue<green){
      Serial.print("Blue detected");
    }
    else if (green<red && green<blue){
      Serial.print("green detected");
    }
    else{
    Serial.print("no colour detected");
    }
  }
  else{
    Serial.print("no colour detected");
  }
  
}

//red
int getRed(){
  digitalWrite(S2,LOW);
	digitalWrite(S3,LOW);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

int getBlue(){
	digitalWrite(S2,LOW);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

int getGreen(){
	digitalWrite(S2,HIGH);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

