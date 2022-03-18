#include "iButton.h"

/**
 *
 **/
iButton::iButton(uint8_t pin, int mode){
  _pin = pin;

  DOUBLE_TS = 250;
  HOLD_TS = 1000;
  LONG_TS = 3000;

  single = false;
  ignoreUp = false;
  trapDoubleClick = false;
  waitDoubleClick = false;
  caughtHold = false;
  caughtLong = false;

  // sets the pin as input
  pinMode(_pin, INPUT);
  // sets the pin mode
  digitalWrite(_pin, mode);

  cb_click = NULL;
  cb_doubleClick = NULL;
  cb_hold = NULL;
  cb_longHold = NULL;
}

/**
 *  set max ms between clicks for DOUBLE CLICK EVENT is catched
 *  @param unsigned int ms
 **/
void iButton::setDoubleTreshold(uint16_t ms){
  DOUBLE_TS = ms;
}

/**
 *  set max ms between clicks for HOLD EVENT is catched
 *  @param unsigned int ms
 **/
void iButton::setHoldTreshold(uint16_t ms){
  HOLD_TS = ms;
}

/**
 *  set max ms between clicks for LONG HOLD EVENT is catched
 *  @param unsigned int ms
 **/
void iButton::setLongTreshold(uint16_t ms){
  LONG_TS = ms;
}

/**
 *  attach callback function for specific event
 *  @param uint8_t event
 *  @param callback cb
 **/
void iButton::on(uint8_t event, void (*cb)()){
  switch(event){
    case iBUTTON_EVENT_CLICK:
      cb_click = cb;
      break;
    case iBUTTON_EVENT_DOUBLE:
      cb_doubleClick = cb;
      break;
    case iBUTTON_EVENT_HOLD:
      cb_hold = cb;
      break;
    case iBUTTON_EVENT_LONG:
      cb_longHold = cb;
      break;
  }
}

/**
 *  detach callback function for all events
 *  @param callback cb
 **/
 void iButton::off(){
   cb_click = NULL;
   cb_doubleClick = NULL;
   cb_hold = NULL;
   cb_longHold = NULL;
 }

/**
 *  detach callback function for specified event
 *  @param uint8_t event
 *  @param callback cb
 **/
void iButton::off(uint8_t event){
  switch(event){
    case iBUTTON_EVENT_CLICK:
      cb_click = NULL;
      break;
    case iBUTTON_EVENT_DOUBLE:
      cb_doubleClick = NULL;
      break;
    case iBUTTON_EVENT_HOLD:
      cb_hold = NULL;
      break;
    case iBUTTON_EVENT_LONG:
      cb_longHold = NULL;
      break;
  }
}

uint8_t iButton::tick(){
  uint8_t event = iBUTTON_EVENT_NONE;

  unsigned long now = millis();

  current = digitalRead(_pin);

  // check if button is pressed down
  if(current == LOW && previous == HIGH && (now - upTime) > iBUTTON_DEBOUNCE){
    dnTime = now;
    single = true;
    ignoreUp = false;
    caughtHold = false;
    caughtLong = false;

    if((now - upTime) < DOUBLE_TS && !trapDoubleClick && waitDoubleClick)
      trapDoubleClick = true;
    else
      trapDoubleClick = false;

    waitDoubleClick = false;
  }

  // check if button is released
  else if(current == HIGH && previous == LOW && (now - dnTime) > iBUTTON_DEBOUNCE){
    if(!ignoreUp){
      upTime = now;

      if(!trapDoubleClick){
        waitDoubleClick = true;
      }else{
        event = iBUTTON_EVENT_DOUBLE;
        trapDoubleClick = false;
        waitDoubleClick = false;
        single = false;
      }
    }
  }

  // Check for normal click
  if(current == HIGH && (now - upTime) > DOUBLE_TS && !trapDoubleClick && waitDoubleClick && single){
    event = iBUTTON_EVENT_CLICK;
    waitDoubleClick = false;
  }

  // Check for hold click
  if(current == HIGH && previous == LOW && (now - dnTime) > HOLD_TS && (now - dnTime) < LONG_TS){
    event = iBUTTON_EVENT_HOLD;
  }

  // Check for normal hold or long
  if(current == LOW && (now - dnTime) > HOLD_TS){
    if(!caughtHold){
      ignoreUp = true;
      caughtHold = true;
    }

    // Check for long
    if((now - dnTime) > LONG_TS){
      if(!caughtLong){
        caughtLong = true;
        event = iBUTTON_EVENT_LONG;
      }
    }
  }

  // calling related callback
  switch(event){
    case iBUTTON_EVENT_CLICK:
      if(cb_click) cb_click();
      break;
    case iBUTTON_EVENT_DOUBLE:
      if(cb_doubleClick) cb_doubleClick();
      break;
    case iBUTTON_EVENT_HOLD:
      if(cb_hold) cb_hold();
      break;
    case iBUTTON_EVENT_LONG:
      if(cb_longHold) cb_longHold();
      break;
  }

  previous = current;

  return event;
}
