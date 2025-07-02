#ifndef PID
#define PID

class pid
{
public:
    pid(float kp, float ki, float kd);

    void set_setpoint(float sp);
    float calculate(float input, float delta);

private:
    float offset;
    float kp;
    float ki;
    float kd;
    float last_error;
    float setpoint;
    float integral;
    unsigned long lasttime;
};
#endif