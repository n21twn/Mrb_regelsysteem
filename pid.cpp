#include "pid.h"
#include <Arduino.h>

pid::pid(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd), last_error(0), setpoint(0), intergral(0), lasttime(0) {};

void pid::set_setpoint(float sp)
{
    setpoint = sp;
}

float pid::calculate(float input, float delta)
{
    float error = abs(setpoint - input);
    intergral = intergral + error * delta;
    float deri = (error - last_error) / 2;

    float output = kp * error + ki * intergral + kd * deri;

    last_error = error;
    return output;
}