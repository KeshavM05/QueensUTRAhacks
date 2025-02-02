// Motor Pins
int EN_A = 11; // Enable pin for left motor
int IN1 = 9;  // Control pin for left motor forward
int IN2 = 8;  // Control pin for left motor backward
int IN3 = 7;  // Control pin for right motor forward
int IN4 = 6;  // Control pin for right motor backward
int EN_B = 10; // Enable pin for right motor

// Variables for motor speeds
int motor_speed = 299;

void setup() {
    Serial.begin(9600); // Start serial communication
    
    // Initialize motor pins as output
    pinMode(EN_A, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(EN_B, OUTPUT);
}

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
    digitalWrite(IN3, LOW); // Right motor forward
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(270); // Adjust time for 90-degree turn
}

// Function to rotate right (90 degrees)
void rotateRight() {
    digitalWrite(IN1, LOW);  // Left motor forward
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);  // Right motor backward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(270); // Adjust time for 90-degree turn
}

// Function to stop
void stop() {
    analogWrite(EN_A, 0);
    analogWrite(EN_B, 0);
    delay(1000); // Adjust time for 90-degree turn
}

void loop() {
    rotateLeft();
    stop();
    goStraight();
    stop();
    rotateRight();
    stop();
}
