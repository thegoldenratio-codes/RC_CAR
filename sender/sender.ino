#include <WiFi.h>

const char* ssid = "RC_CAR";
const char* password = "12345678";
const char* host = "192.168.4.1";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  if (client.connect(host, 80)) {
    Serial.println("Connected to ESP server");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  if (client.connected()) {
    client.println("TEST");
    Serial.println("Sent: TEST");
  } else {
    Serial.println("Disconnected, retrying...");
    client.connect("192.168.4.1", 80);
  }

  delay(500);
}