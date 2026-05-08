#include <WiFi.h>

const char* ssid = "RC_CAR";
const char* password = "12345678";

WiFiServer server(80);
WiFiClient client;

char lastCmd = 'd';
String buffer = "";

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  server.begin();
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
    lastCmd = 'd';
  }

  applyMotor(lastCmd);
}

void applyMotor(char cmd) {

  if (cmd == 'F') {
    Serial.println("FORWARD");
  }
  else if (cmd == 'B') {
    Serial.println("BACKWARD");
  }
  else if (cmd == 'L') {
    Serial.println("LEFT");
  }
  else if (cmd == 'R') {
    Serial.println("RIGHT");
  }
  else {
    Serial.println("STOP");
  }
}