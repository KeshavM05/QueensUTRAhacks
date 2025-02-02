// Motor Pins
int EN_A = 11; // Enable pin for left motor
int IN1 = 9;  // Control pin for left motor forward
int IN2 = 8;  // Control pin for left motor backward
int IN3 = 7;  // Control pin for right motor forward
int IN4 = 6;  // Control pin for right motor backward
int EN_B = 10; // Enable pin for right motor
int S0 = 12; 
int S1 = 2;
int S2 = 3;//colour detection
int S3 = 4;//colour detection
int sensorOut = 5;
int trigPin = 13;//supersonic sensor
int echoPin = 1;//supersonic sensor

// Variables for motor speeds
int motor_speed = 255;

void setup() {
    Serial.begin(9600); // Start serial communication
    
    // Initialize motor pins as output
    pinMode(EN_A, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(EN_B, OUTPUT);
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

  //set frequency to 20% for colour detection
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    pinMode(sensorOut,INPUT);
    Serial.begin(9600);
}

void loop() {

  //------Colour detector values corresponding to the function------
  int red=getRed();
  int blue=getBlue();
  int green=getGreen();

  Serial.print(" - Red= ");
	Serial.print(red);
	Serial.print(" - Green= ");
	Serial.print(green);
	Serial.print(" - Blue= ");
	Serial.println(blue);

  //-----------------------Supersonic setup-----------------------
  long duration;
  int distance;
  
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  distance = (duration / 2) / 29.1;

  //------------------------------COMMANDS------------------------------
    
    if (red <500 && blue <500 && green <500){//if box is red, spin 180
      goStraight();
      long unsigned time_y = millis();
      // if (red<blue && red<green){
      //   Serial.print("Red detected");
      // }
      // else if (blue<red && blue<green){//if box is blue, turn left
      //   Serial.print("Blue detected");
      // }
      // else if (green<red && green<blue){//if box is green, turn right
      //   Serial.print("green detected");
      // }
    }
    else{//if box is black, go straight
      stop();
      yPos = time_y/2;
      turnLeft();
      if (red <500 && blue <500 && green <500){
        goStraight();
        long unsigned time_x = millis();
      }
      else{
        rotateLeft();
        rotateLeft();
      
      }

    }
  }
  
}

//---------------------FUNCTIONS-------------
// Function to move forward
void goStraight() {
    digitalWrite(IN1, LOW);  // Left motor forward
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  // Right motor forward
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
}

// Function to move backward
void goBack() {
    digitalWrite(IN1, HIGH); // Left motor backward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); // Right motor backward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
}

// Function to rotate left (90 degrees)
void rotateLeft() {
    digitalWrite(IN1, HIGH); // Left motor backward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); // Right motor forward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(500); // Adjust time for 90-degree turn
}

// Function to rotate right (90 degrees)
void rotateRight() {
    digitalWrite(IN1, LOW);  // Left motor forward
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  // Right motor backward
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(500); // Adjust time for 90-degree turn
}

void stop() {
    digitalWrite(IN1, LOW);  // Left motor forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  // Right motor backward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, 0);
    analogWrite(EN_B, 0);
    delay(500); // Adjust time for 90-degree turn
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

