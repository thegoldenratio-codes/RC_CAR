#include <ESP32Servo.h>

Servo steering;

#define SERVO_PIN 13

void setup()
{
  steering.setPeriodHertz(50);

  steering.attach(SERVO_PIN, 500, 2400);

  steering.write(90);

  delay(1000);
}

void loop()
{
  steering.write(0);
  delay(1000);

  steering.write(90);
  delay(1000);

  steering.write(180);
  delay(1000);
}