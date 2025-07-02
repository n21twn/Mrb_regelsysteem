#include <Arduino.h>
#include "afstand_sensor.h"
#include "pid.h"

int test_waarde = 30;
// heb de pid tijdelijk laag gezet om dat de fan gevoelig is. Hij gaf correctie van 20+-
// pid pid1(1.5, 0.2, 0.7);    // oude variant
pid pid1(0.5, 0.1, 0.7); // check goede waarde voor sterke pid

int fan_pin = 3; // nog opzetten

void setup()
{
  Serial.begin(9600);
  pinMode(fan_pin, OUTPUT);
  pid1.set_setpoint(20.0);
}

void loop()
{

  if (Serial.available())
  {
    String data = Serial.readStringUntil('\n');
    float waarde = data.toFloat(); // omzetten naar float

    // stuur waarde terug naar python
    Serial.print("Ontvangen waarde: ");
    Serial.println(waarde);
  }

  float pid_output = pid1.calculate(test_waarde, 0.5);

  // if (pid_output < 0)
  //   pid_output = 0;
  // if (pid_output > 255)
  //   pid_output = 255;

  //  range tussen 76-80  licht aan hoe de buis staat
  analogWrite(fan_pin, 150);

  // Serial.print("Waarde na PID en in PWM signaal: ");
  // Serial.print(pid_output);
  delay(1000);
}
