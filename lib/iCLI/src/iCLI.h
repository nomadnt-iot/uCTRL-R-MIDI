#ifndef _iCLI_H_
#define _iCLI_H_

#include <Arduino.h>

class iCLI{
	public:
    /**
     *  class constructor
     **/
	iCLI(Stream &serial, unsigned int size);

    /**
     *  class destructor
     **/
    ~iCLI();

    /**
     *  check if serial buffer have something and parse it
     *  @return unsigned integer
     **/
    void run();

    /**
     *  set callback to call on string receive complete
     *  @param const char *line
     **/
    void on(void(*cb)(char *line));

  private:
    Stream &_serial;

    uint16_t _cursor;
    uint16_t _size;

    char *_buffer;

    void (*_cb_on)(char *line);
};

#endif
