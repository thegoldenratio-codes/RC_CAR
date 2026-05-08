    #include <Wire.h>
    #include <WiFi.h>

    #define MPU_ADDR 0x68

    const char* ssid = "RC_CAR";
    const char* password = "12345678";
    const char* host = "192.168.4.1";

    WiFiClient client;

    // MPU variables
    int16_t accX, accY, accZ;
    int16_t gyroX, gyroY, gyroZ;

    float roll = 0, pitch = 0;
    float rollAcc, pitchAcc;
    float gyroRollRate, gyroPitchRate;

    unsigned long lastTime = 0;
    float dt;
    float alpha = 0.98;

    String lastCommand = "";

    void connectToServer() {
      while (!client.connect(host, 80)) {
        Serial.println("Retrying connection...");
        delay(500);
      }
      Serial.println("Connected to ESP server");
    }

    void setup() {
      Serial.begin(115200);
      Wire.begin(21, 22);

      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      Serial.println("\nWiFi Connected");
      Serial.println(WiFi.localIP());

      connectToServer();

      // MPU wake
      Wire.beginTransmission(MPU_ADDR);
      Wire.write(0x6B);
      Wire.write(0);
      Wire.endTransmission(true);

      lastTime = micros();
    }

    void loop() {
      // TIME STEP
      unsigned long now = micros();
      dt = (now - lastTime) * 1e-6;
      lastTime = now;

      // READ MPU
      Wire.beginTransmission(MPU_ADDR);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_ADDR, 14, true);

      accX = Wire.read() << 8 | Wire.read();
      accY = Wire.read() << 8 | Wire.read();
      accZ = Wire.read() << 8 | Wire.read();
      Wire.read(); Wire.read();
      gyroX = Wire.read() << 8 | Wire.read();
      gyroY = Wire.read() << 8 | Wire.read();
      gyroZ = Wire.read() << 8 | Wire.read();

      float Ax = accX / 16384.0;
      float Ay = accY / 16384.0;
      float Az = accZ / 16384.0;

      gyroRollRate  = gyroX / 131.0;
      gyroPitchRate = gyroY / 131.0;

      rollAcc  = atan2(Ay, sqrt(Ax*Ax + Az*Az)) * 180 / PI;
      pitchAcc = atan2(-Ax, sqrt(Ay*Ay + Az*Az)) * 180 / PI;

      roll  += gyroRollRate * dt;
      pitch += gyroPitchRate * dt;

      roll  = alpha * roll  + (1 - alpha) * rollAcc;
      pitch = alpha * pitch + (1 - alpha) * pitchAcc;

      // -------- COMMAND LOGIC --------
      String cmd = "S";

      if (roll > 40) cmd = "R";
      else if (roll < -20) cmd = "L";
      else if (pitch > 0) cmd = "F";
      else if (pitch < -40) cmd = "B";

      // -------- SEND ONLY IF CHANGED --------
      if (cmd != lastCommand && client.connected()) {
        client.println(cmd);   // IMPORTANT: newline
        Serial.println(cmd);
        lastCommand = cmd;
      }

      // -------- RECONNECT IF DISCONNECTED --------
      if (!client.connected()) {
        Serial.println("Reconnecting...");
        client.stop();
        connectToServer();
      }

      delay(10);
    }