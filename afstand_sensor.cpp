#include "afstand_sensor.h"
#include <Arduino.h>

afstand_sensor::afstand_sensor(int trig, int echo, int size) : trig_pin(trig), echo_pin(echo), filter_size(size), index(0), totaal(0)
{
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    readings = new float[filter_size];    // maakt een new aan omdat pointer is gebruikt in de class en hij nog geen size dus heeft.
    for (int i = 0; i < filter_size; i++) // full lijst met 0
    {
        readings[i] = 0;
    }
};

float afstand_sensor::raw_distance()
{
    // zet de HC-SR04 aan (req 10 mili second impulse)
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // leest the pin zo lang de hij high is.
    long duur = pulseIn(echo_pin, HIGH);
    // berekend afstand door snelheid van geluid * lengthe van echo te doen. Deel daarna naar 2 want echo moet heen en terug komen.
    float afstand = duur * 0.034 / 2.0;
    if (afstand == 0 || afstand > 400)
        return afstand;

    return afstand;
};

float afstand_sensor::filter_afstand()
{
    float new_afstanden = raw_distance();
    if (new_afstanden < 0)
        return -1.0;

    totaal -= readings[index];         // verwijder laatste waarde uit totaal
    readings[index] = new_afstanden;   // Voeg nieuwe waarde toe aan lijst
    totaal += readings[index];         // Voeg nieuwe waarde toe aan totaal
    index = (index + 1) % filter_size; // schuift index 1 op en gebruikt modulo om binn filter size te blijven

    int average = totaal / filter_size;
    return average;
}