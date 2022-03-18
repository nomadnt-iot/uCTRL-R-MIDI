#ifndef iLED_H
#define iLED_H

#include <Arduino.h>

class iLed{
  public:
    // iLed costructor
    iLed(int pin);

    // Turn on led
    void on();

    // Turn off led
    void off();

    // Program blink led
    void blink(uint8_t times = 3, long on = 70, long off = 70);

    // Update led
    void update();
  private:

    uint8_t _pin;           // the number of the LED pin
    uint8_t _state;         // ledState used to set the LED
    uint8_t _times;         // how many time will blink
    unsigned long _on;      // milliseconds of on-time
    unsigned long _off;     // milliseconds of off-time
    unsigned long _time;  	// will store last time LED was updated
};

#endif
