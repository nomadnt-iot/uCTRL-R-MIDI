#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "iSlider.h"

/**
 * Class costructor
 *
 * @param uint8_t pin to be used form sensor
 * @param uint8_t tolerance rate
 * @param uint16_t lock timeout
 * @param uint16_t debounce timeout
 **/
iSlider::iSlider(uint8_t pin, uint8_t rate, uint16_t lock, uint16_t debounce){
	__pin = pin;
	__rate = rate;
	__lock = lock;
	__debounce = debounce;

	__time = new unsigned long[2];

	__current = 0;
	__stored = 0;

	cb_lock = NULL;
	cb_unlock = NULL;
}

/**
 * Class destructor
 **/
iSlider::~iSlider(){
	delete [] __time;
}

/**
 *  Set the motion tolerance (ie hand vibration) rate
 *  @param uint8_t rate
 **/
void iSlider::setTolerance(uint8_t rate){
	__rate = rate;
}

/**
 *  Set the amount of time needed to detect slider lock in ms
 *  @param uint16_t ms
 **/
void iSlider::setLockTimeout(uint16_t ms){
	__lock = ms;
}

/**
 *  Set the amount of time needed to avoid false UNLOCKED request STATE detection
 *  @param uint16_t ms
 **/
void iSlider::setDebounceTimeout(uint16_t ms){
	__debounce = ms;
}

/**
 *  Attach the function that will be called when event occurred
 *  @param uint8_t event
 *  @param callback cb
 **/
void iSlider::on(uint8_t event, void (*cb)()){
  switch(event){
    case iSLIDER_LOCKED:
      cb_lock = cb;
      break;
    case iSLIDER_UNLOCKED:
      cb_unlock = cb;
      break;
  }

}

/**
 *  Return the current or stored value of virtual slider
 *  @return uint16_t value
 **/
uint16_t iSlider::read(){
	unsigned long now = millis();

	__current = analogRead(__pin);

	switch(__state){
		case iSLIDER_UNLOCKED:

			//iSLIDER_MIN = (iSLIDER_ARES * 0.4) / iSLIDER_AREF
			// ((pow(2, iSLIDER_ARES) - 1) * 0.4) / iSLIDER_AREF
			if(__current > ((pow(2, iSLIDER_ARES) - 1) * 0.4) / iSLIDER_AREF){
				if(abs(__current - __stored) > __rate){
					__stored = __current;
					__time[iSLIDER_UNLOCKED] = now;
				}

				if((now - __time[iSLIDER_UNLOCKED]) > __lock && abs(__current - __stored) < __rate){
					__state = iSLIDER_LOCKED;
					__time[iSLIDER_LOCKED] = now;
					if(cb_lock) cb_lock();
				}
			}else{
				__time[iSLIDER_UNLOCKED] = now;
			}

			break;
		case iSLIDER_LOCKED:
			if(now - __time[iSLIDER_LOCKED] > __debounce){
				if(abs(__current - __stored) < __rate){
					__state = iSLIDER_UNLOCKED;
					__time[iSLIDER_UNLOCKED] = now;
					if(cb_unlock) cb_unlock();
				}
			}
			break;
	}

	if(__state == iSLIDER_LOCKED) return __stored;

	return __current;
}

/**
 *  Get the current STATE of slider
 **/
uint8_t iSlider::state(){
	return __state;
}

/**
 *  Set the default STATE of slider and value
 *  @param uint8_t state
 **/
uint8_t iSlider::state(uint8_t state){
	return __state = state;
}
