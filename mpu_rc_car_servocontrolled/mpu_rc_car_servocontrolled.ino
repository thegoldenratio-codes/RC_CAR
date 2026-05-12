#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid = "RC_CAR";
const char* password = "12345678";

WiFiServer server(80);
WiFiClient client;

char lastCmd = 'S';
char prevCmd = 'X';

String buffer = "";

// ---------------- MOTOR PINS ----------------
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

// ---------------- SERVO ----------------
#define SERVO_PIN 14

Servo steering;

int servoAngle = 90;

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  // KEEP WIFI FULLY ACTIVE
  WiFi.setSleep(false);

  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  server.begin();

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Servo setup
  steering.setPeriodHertz(50);
  steering.attach(SERVO_PIN, 500, 2400);

  centerSteering();

  stopCar();
}

void loop() {

  // Accept new client
  if (!client || !client.connected()) {

    client = server.available();

    if (client) {
      Serial.println("Client Connected");
    }
  }

  // Read incoming data
  if (client && client.connected()) {

    while (client.available()) {

      char c = client.read();

      if (c == '\n' || c == '\r') {

        if (buffer.length() > 0) {

          lastCmd = buffer[0];
          buffer = "";
        }

      } else {

        buffer += c;
      }
    }

  } else {

    lastCmd = 'S';
  }

  // ONLY APPLY IF COMMAND CHANGES
  if(lastCmd != prevCmd) {

    applyControl(lastCmd);

    prevCmd = lastCmd;
  }

  delay(10);
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

// ---------------- MOTOR FUNCTIONS ----------------

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

// ---------------- SERVO FUNCTIONS ----------------

void steerLeft() {

  while(servoAngle < 65) {

    servoAngle += 5;

    steering.write(servoAngle);

    delay(20);
  }

  while(servoAngle > 65) {

    servoAngle -= 5;

    steering.write(servoAngle);

    delay(20);
  }
}

void steerRight() {

  while(servoAngle < 115) {

    servoAngle += 5;

    steering.write(servoAngle);

    delay(20);
  }

  while(servoAngle > 115) {

    servoAngle -= 5;

    steering.write(servoAngle);

    delay(20);
  }
}

void centerSteering() {

  while(servoAngle < 90) {

    servoAngle += 5;

    steering.write(servoAngle);

    delay(20);
  }

  while(servoAngle > 90) {

    servoAngle -= 5;

    steering.write(servoAngle);

    delay(20);
  }
}