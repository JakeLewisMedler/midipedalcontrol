#ifndef Pedal_h
#define Pedal_h

#include "Arduino.h"

class Pedal
{
  public:
    Pedal(uint8_t newPinA, uint8_t newPinB, int newPinGND, int newPinSense);

    void setupPins();

    void setCC(int newCC);
    void setMode(int newMode);
    void setType(int newType);
    void setPolarity(int newPolarity);
    void setValue(int newValue);

    bool hasBeenUpdated();
    void checkInput();

    void reset();

    int cc = 1;

    uint8_t pinA;
    uint8_t pinB;
    int pinGND;
    int pinSense;

    int mode = 1;
    int type = 1;
    int value = 0;
    bool enabled = false;
    int polarity = 1;

  private:
    bool _updated = false;

};

#endif