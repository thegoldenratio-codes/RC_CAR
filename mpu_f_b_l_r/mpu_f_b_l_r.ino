#include <Wire.h>

#define MPU_ADDR 0x68

// Raw sensor values
int16_t accX, accY, accZ;
int16_t gyroX, gyroY, gyroZ;

// Angles
float roll = 0.0;
float pitch = 0.0;

// Accelerometer angles
float rollAcc, pitchAcc;

// Gyro rates
float gyroRollRate, gyroPitchRate;

// Timing
unsigned long lastTime = 0;
float dt;

// Complementary filter constant
float alpha = 0.98;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  lastTime = micros();
}

void loop() {
  // -------- TIME STEP --------
  unsigned long currentTime = micros();
  dt = (currentTime - lastTime) * 1e-6;
  lastTime = currentTime;

  // -------- READ MPU6050 --------
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read(); // temperature (ignore)
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  // -------- CONVERT RAW DATA --------
  float Ax = accX / 16384.0;
  float Ay = accY / 16384.0;
  float Az = accZ / 16384.0;

  gyroRollRate  = gyroX / 131.0;
  gyroPitchRate = gyroY / 131.0;

  // -------- ACCELEROMETER ANGLES --------
  rollAcc  = atan2(Ay, sqrt(Ax * Ax + Az * Az)) * 180 / PI;
  pitchAcc = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * 180 / PI;

  // -------- GYRO INTEGRATION --------
  roll  += gyroRollRate * dt;
  pitch += gyroPitchRate * dt;

  // -------- COMPLEMENTARY FILTER --------
  roll  = alpha * roll  + (1 - alpha) * rollAcc;
  pitch = alpha * pitch + (1 - alpha) * pitchAcc;

  // -------- OUTPUT --------
  // Serial.print("Hl:");
  // Serial.println(90);
  // Serial.print(",");
  // Serial.print("Roll:");
  // Serial.print(roll);
  // Serial.print(",");
  // Serial.print("Pitch:");
  // Serial.println(pitch);
  // Serial.print(",");
  // Serial.print("LL:");
  // Serial.println(-90);
  // Serial.println();

if( roll < 40 && roll > -20 && pitch < 0 && pitch > -40 )
{
  Serial.print("S");
  Serial.println();
}
else if( roll > 40)
{
  Serial.print("R");
  Serial.println();
}
else if(roll < -20)
{
  Serial.print("L");
  Serial.println();
}
else if( pitch > 0)
{
  Serial.print("F");
  Serial.println();
}
else if( pitch < -40)
{
  Serial.print("B");
  Serial.println();
}

  delay(5); // ~200 Hz loop
}