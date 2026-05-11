#include "BluetoothSerial.h"
#include "ESP32Servo.h"

BluetoothSerial SerialBT;

// ---------------- MOTOR PINS ----------------
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

// ---------------- SERVO ----------------
#define SERVO_PIN 14

Servo steering;

int servoAngle = 90;

char lastCmd = 'S';

// =====================================================

void setup() {

  Serial.begin(115200);

  // Bluetooth name
  SerialBT.begin("ESP32_RC");

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Attach servo
  steering.attach(SERVO_PIN);

  // Center steering
  steering.write(90);

  startCar();

  Serial.println("ESP32 RC Ready");
}

// =====================================================

void loop() {

  // Read Bluetooth command
  if (SerialBT.available()) {

    lastCmd = SerialBT.read();

    Serial.print("Received: ");
    Serial.println(lastCmd);
  }

  applyControl(lastCmd);
}

// =====================================================
// CONTROL
// =====================================================

void applyControl(char cmd) {

  switch (cmd) {

    case 'F':
      forward();
      break;

    case 'B':
      backward();
      break;

    case 'L':
      steerLeft();
      break;

    case 'R':
      steerRight();
      break;

    case 'S':
      stopCar();
      centerSteering();
      break;
  }
}

// =====================================================
// MOTOR FUNCTIONS
// =====================================================

void startCar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =====================================================
// SERVO FUNCTIONS
// =====================================================

void steerLeft() {

  while (servoAngle > 65) {

    servoAngle -= 5;

    steering.write(servoAngle);

    delay(20);
  }
}

void steerRight() {

  while (servoAngle < 115) {

    servoAngle += 5;

    steering.write(servoAngle);

    delay(20);
  }
}

void centerSteering() {

  while (servoAngle < 90) {

    servoAngle += 5;

    steering.write(servoAngle);

    delay(20);
  }

  while (servoAngle > 90) {

    servoAngle -= 5;

    steering.write(servoAngle);

    delay(20);
  }
}