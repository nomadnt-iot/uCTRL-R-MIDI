#include "iLed.h"

iLed::iLed(int pin){
  _pin = pin;
  _state = LOW;
  _time = 0;
  _times = 0;

  pinMode(_pin, OUTPUT);
}

/**
 *  turn led on
 **/
void iLed::on(){
  _state = HIGH;
  digitalWrite(_pin, _state);
}

/**
 *  turn led off
 **/
void iLed::off(){
  _state = LOW;
  digitalWrite(_pin, _state);
}

/**
 *  set up a blink program
 *  @param uint8_t times, how many times you want to blink led
 *  @param long on, how many time the led will be on
 *  @param long off, how many time the led will be off
 **/
void iLed::blink(uint8_t times, long on, long off){
  _times = times * 2;
  _on = on;
  _off = off;
}

/**
 *  update the led program
 **/
void iLed::update(){
    // check to see if it's time to change the state of the LED
    if(_times > 0){
      unsigned long now = millis();
      if((_state == HIGH) && (now - _time >= _on)){
        _time = now;
        _times--;
        off();
      }else if((_state == LOW) && (now - _time >= _off)){
        _time = now;
        _times--;
        on();
      }
    }
}
