#include <Arduino.h>

// Ultrasonic sensor pins
#define TRIG_PIN 9
#define ECHO_PIN 10
// Color sensor pin
#define COLOR_SENSOR_PIN A0
// Motor control pins
#define MOTOR_LEFT 5
#define MOTOR_RIGHT 6

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

// Function prototypes
void detectStartPosition();
void moveToNextTile();
void turnLeft();
void turnRight();
bool detectObstacle();
void markWall();
void checkColor();
int readColorSensor();
void avoidObstacle();
void stopRobot();
void expandGrid();
void updateTileSize(float newDistance);

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
    moveToNextTile();  // Move to next tile

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
void moveToNextTile() {
    // Clear previous position
    grid[posX][posY] = 1;

    // Move one tile forward
    int newX = posX, newY = posY;
    if (direction == UP) newX--;
    if (direction == RIGHT) newY++;
    if (direction == DOWN) newX++;
    if (direction == LEFT) newY--;

    // Expand grid if needed
    if (newX >= gridSize || newY >= gridSize || newX < 0 || newY < 0) {
        expandGrid();
    }

    // Measure actual movement distance
    float measuredDistance = measureDistance();

    // Update estimated tile size dynamically
    updateTileSize(measuredDistance);

    posX = newX;
    posY = newY;
    grid[posX][posY] = 5;
}

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
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
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
