#include <Servo.h>  // Include the Servo library

// Pin definitions
#define ENA 3    // Speed control for Motor A
#define ENB 9    // Speed control for Motor B
#define IN1 5    // Motor A - Input 1
#define IN2 6    // Motor A - Input 2
#define IN3 7    // Motor B - Input 3
#define IN4 8    // Motor B - Input 4
#define SERVO_PIN 10 // Pin for the Servo

char command; // Variable to store Bluetooth commands
Servo sideViewServo; // Create a Servo object for side view control

void setup() {
    // Initialize motor pins as outputs
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Initialize serial communication for Bluetooth
    Serial.begin(9600);

    // Initialize the servo
    sideViewServo.attach(SERVO_PIN); // Attach the servo to the specified pin

    // Set initial position of the servo to 90 degrees (center position)
    sideViewServo.write(90);  // Servo set to 90 degrees at the start
    delay(500); // Wait for the servo to reach the initial position
}

void loop() {
    // Check for Bluetooth commands
    if (Serial.available()) {
        command = Serial.read(); // Read the command from the Bluetooth module

        // Handle Forward command
        if (command == 'F') {
            forward(); // Start moving forward
        }
        // Handle other commands
        else {
            switch (command) {
                case 'B': // Backward
                    backward();
                    break;
                case 'L': // Left
                    left();
                    break;
                case 'R': // Right
                    right();
                    break;
                case 'S': // Stop
                    stopMotors();
                    break;
                case 'V': // Side view control (move to 180 degrees right)
                    controlSideView(180); // Move the servo to 180 degrees
                    break;
                case 'W': // Side view control (move to 0 degrees left)
                    controlSideView(0); // Move the servo to 0 degrees
                    break;
                default:
                    // Do nothing for unknown commands
                    break;
            }
        }
    }
}

// Function to move forward
void forward() {
    analogWrite(ENA, 50); // Full speed
    analogWrite(ENB, 50); // Full speed
    digitalWrite(IN1, HIGH);   // Motor A forward
    digitalWrite(IN2, LOW);    // Motor A forward
    digitalWrite(IN3, HIGH);   // Motor B forward
    digitalWrite(IN4, LOW);    // Motor B forward
}

// Function to move backward
void backward() {
    analogWrite(ENA, 50); // Increase speed for testing
    analogWrite(ENB, 50); // Increase speed for testing
    digitalWrite(IN1, LOW);    // Motor A backward
    digitalWrite(IN2, HIGH);   // Motor A backward
    digitalWrite(IN3, LOW);    // Motor B backward
    digitalWrite(IN4, HIGH);   // Motor B backward
}

// Function to turn left
void left() {
    analogWrite(ENA, 30); // Adjust speed for turning
    analogWrite(ENB, 30);
    digitalWrite(IN1, LOW);    // Motor A left
    digitalWrite(IN2, HIGH);   // Motor A left
    digitalWrite(IN3, HIGH);   // Motor B left
    digitalWrite(IN4, LOW);    // Motor B left
}

// Function to turn right
void right() {
    analogWrite(ENA, 30);
    analogWrite(ENB, 30);
    digitalWrite(IN1, HIGH);   // Motor A right
    digitalWrite(IN2, LOW);    // Motor A right
    digitalWrite(IN3, LOW);    // Motor B right
    digitalWrite(IN4, HIGH);   // Motor B right
}

// Function to stop the motors
void stopMotors() {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Function to control the side view servo
void controlSideView(int position) {
    sideViewServo.write(position); // Move the servo to the specified position
    delay(500); // Wait for the servo to reach the position
}
