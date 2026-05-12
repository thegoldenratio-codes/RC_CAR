#include <WiFi.h>

const char* ssid = "RC_CAR";
const char* password = "12345678";

WiFiServer server(80);
WiFiClient client;

char lastCmd = 'd';
String buffer = "";

// Motor pins
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22
#define ENA 5
#define ENB 23

char command;

void setup() {
  Serial.begin(115100);

  WiFi.softAP(ssid, password);

  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  server.begin();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // NEW ESP32 PWM API (Core v3+)
  ledcAttach(ENA, 1000, 8);  // pin, freq, resolution
  ledcAttach(ENB, 1000, 8);

  startCar();

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

      // command is single char OR newline
      if (c == '\n' || c == '\r') {
        if (buffer.length() > 0) {
          lastCmd = buffer[0];   // take first character only
          buffer = "";
        }
      } else {
        buffer += c;
      }
    }
  } 
  else {
    // only STOP when disconnected
    lastCmd = 'S';
  }

  applyMotor(lastCmd);
}

void applyMotor(char cmd) {
  Serial.println(cmd);
  switch (cmd) {
    case 'F': forward(); break;
    case 'B': backward(); break;
    case 'L': left(); break;
    case 'R': right(); break;
    case 'S': stopCar(); break;
  }
}

// ---------------- MOTION FUNCTIONS ----------------
void startCar() {
  ledcWrite(ENA, 100);
  ledcWrite(ENB, 100);

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