#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Motor pins
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22
#define ENA 5
#define ENB 17

char command;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_RC");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // NEW ESP32 PWM API (Core v3+)
  ledcAttach(ENA, 1000, 8);  // pin, freq, resolution
  ledcAttach(ENB, 1000, 8);

  startCar();

  Serial.println("ESP32 RC Ready");
}

void loop() {
  if (SerialBT.available()) {
    command = SerialBT.read();
    Serial.println(command);

    switch (command) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': stopCar(); break;
    }
  }
}

// ---------------- MOTION FUNCTIONS ----------------

void startCar() {
  ledcWrite(ENA, 200);
  ledcWrite(ENB, 200);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  ledcWrite(ENA, 250);
  ledcWrite(ENB, 250);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  ledcWrite(ENA, 250);
  ledcWrite(ENB, 250);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  ledcWrite(ENA, 200);
  ledcWrite(ENB, 250);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  ledcWrite(ENA, 250);
  ledcWrite(ENB, 200);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}