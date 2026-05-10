#include <ESP32Servo.h>

Servo steering;

#define SERVO_PIN 13

int angle = 90;

void setup()
{
  Serial.begin(115200);

  steering.setPeriodHertz(50);
  steering.attach(SERVO_PIN, 500, 2400);

  steering.write(angle);

  Serial.print("Servo Angle: ");
  Serial.println(angle);

  delay(1000);
}

void loop()
{
  angle = 65;
  for(angle;angle<=105;angle = angle + 5)
  {
    steering.write(angle);

    Serial.print("Servo Angle: ");
    Serial.println(angle);

    delay(1000);
  }
}