#include "pid.h"
#include <Arduino.h>

pid::pid(float kp, float ki, float kd) : offset(80), kp(kp), ki(ki), kd(kd), last_error(0), setpoint(0), integral(0), lasttime(0) {};

void pid::set_setpoint(float sp)
{
    setpoint = sp;
}

float pid::calculate(float input, float delta)
{
    float error = input - setpoint;
    integral = integral + error * delta;
    integral = constrain(integral, -500, 500);

    float deri = (error - last_error) / delta;

    float correction = kp * error + ki * integral + kd * deri;
    last_error = error;

    float output = offset + correction;
    output = constrain(output, 0, 255);
    Serial.print(" Error: ");
    Serial.print(error);
    Serial.print(" PWM: ");
    Serial.println(output);
    return output;
}