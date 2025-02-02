#include <Arduino.h>

// Motor Pins
int EN_A = 11; // Enable pin for left motor
int IN1 = 9;  // Control pin for left motor forward
int IN2 = 8;  // Control pin for left motor backward
int IN3 = 7;  // Control pin for right motor forward
int IN4 = 6;  // Control pin for right motor backward
int EN_B = 10; // Enable pin for right motor
int S0 = 12; //colour detection
int S1 = 2; //colour detection
int S2 = 3; //colour detection
int S3 = 4; //colour detection
int sensorOut = 5; //colour detection
int trigPin = 13;//supersonic sensor
int echoPin = 1;//supersonic sensor

int motor_speed = 255;

// Color Definitions
#define RED 3
#define GREEN 4
#define BLUE 5

// Directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// Tile size estimation
float tileSize = 0.1;  // Default to 10 cm, will adjust dynamically
float lastTileDistance = 0.0;

// Dynamic grid settings
int gridSize = 5;  // Start with a small grid (expandable)
int **grid;

// Robot's position & direction
int posX, posY;
int direction = UP;  // Default facing up

// Color sequence
int colorSequence[] = {RED, GREEN, BLUE, GREEN, BLUE};
int currentColorIndex = 0;


// Setup function
void setup() {
    Serial.begin(9600);

    // Initialize small grid dynamically
    grid = (int **)malloc(gridSize * sizeof(int *));
    for (int i = 0; i < gridSize; i++) {
        grid[i] = (int *)malloc(gridSize * sizeof(int));
        for (int j = 0; j < gridSize; j++) {
            grid[i][j] = 0;  // Mark as unexplored
        }
    }

    // Detect starting position
    detectStartPosition();
}

// Main loop
void loop() {
    goStraight();  // Move to next tile

    // Check for colors
    checkColor();

    // Check for obstacles
    if (detectObstacle()) {
        avoidObstacle();
    }
}

// **Detect the robot's initial position along the edge**
void detectStartPosition() {
    Serial.println("Detecting start position...");

    // Assume bottom-left initially, adjust based on sensors
    posX = 0;
    posY = 0;

    // Measure distances in all directions
    int distFront, distLeft, distRight, distBack;
    
    // Face upward and check forward
    direction = UP;
    distFront = measureDistance();

    // Turn left and check
    turnLeft();
    distLeft = measureDistance();

    // Turn right twice and check right side
    turnRight();
    turnRight();
    distRight = measureDistance();

    // Turn back to original direction and check backward
    turnLeft();
    turnLeft();
    distBack = measureDistance();

    // Decide the most likely start position based on open space
    if (distFront > 15) { posX = gridSize - 1; }
    if (distBack > 15) { posX = 0; }
    if (distLeft > 15) { posY = gridSize - 1; }
    if (distRight > 15) { posY = 0; }

    // Set the robot's position in the grid
    grid[posX][posY] = 5;  // Mark as starting position
}

// **Move to next tile based on estimated tile size**

// **Dynamically update tile size if inconsistencies are found**
void updateTileSize(float newDistance) {
    if (lastTileDistance == 0) {  
        lastTileDistance = newDistance;
        return;  
    }

    // Calculate the difference between estimated and actual tile size
    float error = abs(newDistance - tileSize);
    float errorPercentage = (error / tileSize) * 100;

    // Adjust if the error is greater than 20%
    if (errorPercentage > 20) {
        tileSize = (tileSize + newDistance) / 2;  // Average new and old values
        Serial.print("Adjusted tile size: ");
        Serial.println(tileSize);
    }

    lastTileDistance = newDistance;
}

// **Expand grid when out of bounds**
void expandGrid() {
    Serial.println("Expanding grid...");
    int newSize = gridSize + 5;  // Expand by 5 tiles

    // Allocate new grid
    int **newGrid = (int **)malloc(newSize * sizeof(int *));
    for (int i = 0; i < newSize; i++) {
        newGrid[i] = (int *)malloc(newSize * sizeof(int));
        for (int j = 0; j < newSize; j++) {
            newGrid[i][j] = 0;  // Initialize
        }
    }

    // Copy old grid
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            newGrid[i][j] = grid[i][j];
        }
        free(grid[i]);  // Free old row
    }
    free(grid);
    grid = newGrid;
    gridSize = newSize;
}

// **Detect obstacles using ultrasonic sensor**
bool detectObstacle() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    return distance < 10;
}

// **Mark detected walls**
void markWall() {
    int wallX = posX, wallY = posY;
    if (direction == UP) wallX--;
    if (direction == RIGHT) wallY++;
    if (direction == DOWN) wallX++;
    if (direction == LEFT) wallY--;

    if (wallX >= 0 && wallY >= 0 && wallX < gridSize && wallY < gridSize) {
        grid[wallX][wallY] = 2;
    }
}

void turnLeft(){
     digitalWrite(IN1, LOW); // Left motor backward
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW); // Right motor forward
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(500); // Adjust time for 90-degree turn
}

void turnRight(){
    digitalWrite(IN1, HIGH);  // Left motor forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);  // Right motor backward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(500); // Adjust time for 90-degree turn
}

void stop(){
    digitalWrite(IN1, LOW);  // Left motor forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  // Right motor backward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, 0);
    analogWrite(EN_B, 0);
    delay(1000); // Adjust time for 90-degree turn
}

void goStraight(){
    digitalWrite(IN1, LOW);  // Left motor forward
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);  // Right motor forward
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
}

void avoidObstacle() {
    Serial.println("Obstacle detected! Finding alternative path...");

    // Mark the wall in front of the robot
    markWall();

    // Try turning left first
    turnLeft();
    if (!detectObstacle()) {
        Serial.println("Turned left successfully.");
        goStraight();
        return;
    }

    // If left is blocked, turn right
    turnRight(); // Face original direction
    turnRight(); // Now facing right
    if (!detectObstacle()) {
        Serial.println("Turned right successfully.");
        goStraight();
        return;
    }

    // If still blocked, turn back to original direction and backtrack
    turnLeft(); // Face original direction
    Serial.println("No open space! Backtracking...");
    moveBack();
}

void moveBack(){
    digitalWrite(IN1, HIGH); // Left motor backward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); // Right motor backward
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed);
    delay(100);
}

long measureDistance(){
  long duration;
  long distance;
  
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

  return distance;
}

void checkColor(){
  int red=getRed();
  int blue=getBlue();
  int green=getGreen();
}

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

