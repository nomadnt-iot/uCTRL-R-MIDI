#ifndef iBUTTON_H
#define iBUTTON_H

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define iBUTTON_MODE_PULLUP     HIGH
#define iBUTTON_MODE_PULLDOWN   LOW

#define iBUTTON_DEBOUNCE        20

#define iBUTTON_EVENT_NONE      0x00
#define iBUTTON_EVENT_CLICK     0x01
#define iBUTTON_EVENT_DOUBLE    0x02
#define iBUTTON_EVENT_HOLD      0x03
#define iBUTTON_EVENT_LONG      0x04

class iButton{
  public:
    iButton(uint8_t pin, int mode = iBUTTON_MODE_PULLUP);

    // set max ms between clicks for DOUBLE CLICK EVENT is catched
    void setDoubleTreshold(uint16_t ms);
    // set max ms between clicks for HOLD EVENT is catched
    void setHoldTreshold(uint16_t ms);
    // set max ms between clicks for LONG HOLD EVENT is catched
    void setLongTreshold(uint16_t ms);

    // attach functions that will be called when button was pressed in the specified way.
    void on(uint8_t event, void (*cb)());

    void off();
    void off(uint8_t event);

    uint8_t tick(void);
  private:
    uint8_t _pin;                         // pin number used by button

    unsigned long dnTime;                 // when the button was pressed
    unsigned long upTime;                 // when the button was released

    bool current = HIGH;                  // current button state value
    bool previous = HIGH;                 // previous button state value

    // Non si è capito se è necessario... ad ogni modo funziona così
    bool single;                          // whether it's OK to do a single click
    bool ignoreUp;                        // whether to ignore the button release due to CLICK + HOLD EVENT was triggered
    bool trapDoubleClick;                 // whether to register DOUBLE CLICK EVENT on next release, or whether to wait and CLICK
    bool waitDoubleClick;                 // whether we're waiting for a DOUBLE CLICK EVENT (DOWN)
    bool caughtHold;                      // whether or not the HOLD EVENT happened already
    bool caughtLong;                      // whether or not the LONG EVENT happened already

    uint16_t DOUBLE_TS;                   // DOUBLE CLICK GAP timeout
    uint16_t HOLD_TS;                     // HOLD TRESHOLD timeout
    uint16_t LONG_TS;                     // LONG HOLD TRESHOLD timeout

    // callback function related section
    void (*cb_click)();
    void (*cb_doubleClick)();
    void (*cb_hold)();
    void (*cb_longHold)();
};

#endif
