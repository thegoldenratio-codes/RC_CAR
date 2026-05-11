#include "BluetoothSerial.h"
#include "ESP32Servo.h"

BluetoothSerial SerialBT;

// Motor pins
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22
// #define ENA 5
// #define ENB 17

char lastCmd = 'S';
String buffer = "";

char command;

// ---------------- SERVO ----------------
#define SERVO_PIN 13

Servo steering;

int servoAngle = 90;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_RC");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // NEW ESP32 PWM API (Core v3+)
  // ledcAttach(ENA, 1000, 8);  // pin, freq, resolution
  // ledcAttach(ENB, 1000, 8);

  startCar();

  Serial.println("ESP32 RC Ready");
}

void loop() {

  // Read incoming Bluetooth data
  while (SerialBT.available()) {

    char c = SerialBT.read();

    if (c == '\n' || c == '\r') {

      if (buffer.length() > 0) {

        lastCmd = buffer[0];
        buffer = "";
      }

    } else {

      buffer += c;
    }
  }

  applyControl(lastCmd);
}

// ---------------- CONTROL ----------------

void applyControl(char cmd) {

  Serial.print("Command: ");
  Serial.println(cmd);

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

// ---------------- MOTION FUNCTIONS ----------------

void startCar() {
  // ledcWrite(ENA, 200);
  // ledcWrite(ENB, 200);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  // ledcWrite(ENA, 250);
  // ledcWrite(ENB, 250);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  // ledcWrite(ENA, 250);
  // ledcWrite(ENB, 250);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}



void stopCar() {
  // ledcWrite(ENA, 0);
  // ledcWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void steerLeft() {

  while(servoAngle < 65) {
    servoAngle = servoAngle + 5;
    steering.write(servoAngle);
    delay(20);
  }
  while(servoAngle > 65) {
    servoAngle = servoAngle - 5;
    steering.write(servoAngle);
    delay(20);
  }
}

void steerRight() {
    while(servoAngle < 115) {
      servoAngle = servoAngle + 5;
      steering.write(servoAngle);
      delay(20);
    }
    while(servoAngle > 115) {
      servoAngle = servoAngle - 5;
      steering.write(servoAngle);
      delay(20);
    }
  }

void centerSteering() {

  while(servoAngle < 90) {
    servoAngle = servoAngle + 5;
    steering.write(servoAngle);
    delay(20);
  }
  while(servoAngle > 90) {
    servoAngle = servoAngle - 5;
    steering.write(servoAngle);
    delay(20);
  }
}