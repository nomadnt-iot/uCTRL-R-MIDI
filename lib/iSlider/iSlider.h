#ifndef iSLIDER_H
#define iSLIDER_H

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define iSLIDER_UNLOCKED  0x00
#define iSLIDER_LOCKED    0x01

#define iSLIDER_ARES      10
#define iSLIDER_AREF      3.3
#define iSLIDER_MIN       0.4

class iSlider {
  public:
    iSlider(uint8_t pin, uint8_t rate = 20, uint16_t lock = 1000L, uint16_t debounce = 2000L);

    /**
     * Class destructor
     **/
    ~iSlider();

    /**
     *  Set the motion tolerance (ie hand vibration) rate
     *  @param uint8_t rate
     **/
    void setTolerance(uint8_t rate);

    /**
     *  Set the amount of time needed to detect slider lock in ms
     *  @param uint16_t ms
     **/
    void setLockTimeout(uint16_t ms);

    /**
     *  Set the amount of time needed to avoid false UNLOCKED request STATE detection
     *  @param uint16_t ms
     **/
    void setDebounceTimeout(uint16_t ms);

    /**
     *  Attach the function that will be called when event occurred
     *  @param uint8_t event
     *  @param void cb
     **/
    void on(uint8_t event, void (*cb)());

    /**
     *  Return the current or stored value of virtual slider
     *  @return uint16_t value
     **/
    uint16_t read();

    /**
     *  Get the current STATE of slider
     **/
    uint8_t state();

    /**
     *  Set the default STATE of slider and value
     *  @param uint8_t state
     **/
    uint8_t state(uint8_t state);

  private:
    // Time tracking variable
    unsigned long *__time;

    // PIN used from the sensor
    uint8_t __pin;

    // Tolerance rate used from the sensor
    uint8_t __rate;

    // STATE of sensor
    uint8_t __state;

    // Lock timeout value
    uint16_t __lock;

    // Debounce timeout value
    uint16_t __debounce;

    // Sensor current value
    uint16_t __current;

    // Sensor stored value
    uint16_t __stored;

    // Lock callback
    void (*cb_lock)();

    // Unlock callback
    void (*cb_unlock)();
};

#endif
