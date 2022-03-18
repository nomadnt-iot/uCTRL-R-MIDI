#include <Arduino.h>

/**
 *  Return Used Memory in bytes
 **/
uint16_t UsedMEMORY(){
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
