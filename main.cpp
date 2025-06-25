#include <Arduino.h>
#include "afstand_sensor.h"
#include "pid.h"
#include <boost/asio.hpp>

afstand_sensor sensor(6, 7);
pid pid1(2.0, 0.5, 1.0); // check goede waarde voor sterke pid

int fan_pin = 3; // nog opzetten

void setup()
{
  Serial.begin(9600);
  pinMode(fan_pin, OUTPUT);
  pid1.set_setpoint(20.0);
}

void loop()
{
  // // float raw = sensor.raw_distance();
  // float afstand = sensor.filter_afstand();
  // // (snell heid van geluid(0.000343) * meter(100cm)) / aantall meetingen(2)
  // // afstand = duur * 0.034 / 2.0;
  // Serial.print("Afstand met filter: ");
  // Serial.print(afstand);
  // // Serial.print(raw);
  // Serial.println(" cm_____");

  // float pid_output = pid1.calculate(afstand, 0.5);

  // if (pid_output < 0)
  //   pid_output = 0;
  // if (pid_output > 255)
  //   pid_output = 255;

  //  range tussen 72 en 60
  analogWrite(fan_pin, 70);

  // Serial.print("Waarde na PID en in PWM signaal: ");
  // Serial.print(pid_output);
  delay(1000);
}
