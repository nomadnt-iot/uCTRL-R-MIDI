#include <iCLI.h>

iCLI::iCLI(Stream &serial, unsigned int size) : _serial(serial){
    _size = size;

    _cursor = 0;

    _buffer = new char[_size];

    _cb_on = NULL;
}

iCLI::~iCLI(){
    delete [] _buffer;
}

void iCLI::on(void (*cb)(char *line)){
    _cb_on = cb;
}

void iCLI::run(){
    if(_serial.available()){
        uint8_t cr = 0;

        while(_serial.available() > 0){
            char c = (char)_serial.read();

            (c == '\r' || c == '\n') ? cr++ : cr = 0;

            _serial.print(c);

            if(c == '\b'){
                _cursor--;
                _buffer[_cursor] = '\0';
            }else{
                if(cr < 2 && _cursor < _size - 1){
                    // filling the buffer
                    if(c != '\b'){
                        _buffer[_cursor++] = c;
                        _buffer[_cursor] = '\0';
                    }
                }else{
                    _serial.flush();
                    // resetting cursor
                    _cursor = 0;
                    // call callback if exists
                    if(_cb_on) _cb_on(_buffer);
                }
            }

            delay(1);
        }
    }
}
