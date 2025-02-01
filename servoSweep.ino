#include <Servo.h>  // Include the Servo library

Servo myServo;      // Create a servo object to control the SG90

void setup() {
  // Attach the servo to digital pin 3
  myServo.attach(3);
  Serial.begin(9600);
}

void loop() {
  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);  // Set the servo position
    delay(15);             // Wait 15ms for the servo to reach the position
  }
  delay(1000);
  Serial.println("First set done");
  
  // Sweep back from 180° to 0°
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);  // Set the servo position
    delay(15);             // Wait 15ms for the servo to reach the position
  }
  delay(1000);
  Serial.println("Second set done");
}
