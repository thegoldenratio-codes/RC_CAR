#include <WiFi.h>

const char* ssid = "RC_CAR";
const char* password = "12345678";

WiFiServer server(80);
String command = "";

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client Connected");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n') {
          Serial.print("Received: ");
          Serial.println(command);
          command = "";
        } else {
          command += c;
        }
      }
    }

    Serial.println("Client Disconnected");
    client.stop();
  }
}