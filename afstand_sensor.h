#ifndef AFSTAND_SENSOR
#define AFSTAND_SENSOR

class afstand_sensor
{
public:
    afstand_sensor(int trig_pin, int echo_pin, int filter_size = 5);
    float filter_afstand();
    float raw_distance();

private:
    int trig_pin;
    int echo_pin;
    int filter_size;
    float *readings; // maakt gebruik van een pointer omdat de size van het filter eventual later te kunne aan pasen dynamische in de code
    int index;
    float totaal;
};
#endif