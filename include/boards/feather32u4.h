#include <MIDIUSB.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include <Adafruit_BLEMIDI.h>

// COMMON SETTINGS
#define BUFSIZE                       128     // Size of the read buffer for incoming data
#define VERBOSE_MODE                  false   // If set to 'true' enables debug output
#define BLUEFRUIT_SWUART_RXD_PIN      9       // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN      10      // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN        11      // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN        -1      // Optional, set to -1 if unused
#define BLUEFRUIT_UART_MODE_PIN       -1      // Set to -1 if unused
#define BLUEFRUIT_SPI_CS              8
#define BLUEFRUIT_SPI_IRQ             7
#define BLUEFRUIT_SPI_RST             4

#ifdef Serial1    // this makes it not complain on compilation if there's no Serial1
  #define BLUEFRUIT_HWSERIAL_NAME     Serial1
#endif

#define FACTORYRESET_ENABLE           1
#define MINIMUM_FIRMWARE_VERSION      "0.7.0"
#define USE_RUNNING_STATUS            0

#define PIN_LED1                      12      // Pin number of led 1
#define PIN_LED2                      11      // Pin number of led 2
#define PIN_LED3                      18      // Pin number of led 3
#define PIN_LED4                      19      // Pin number of led 4
#define PIN_LED5                      2       // Pin number of led 5
#define PIN_LED6                      3       // Pin number of led 6
#define PIN_LED7                      22      // Pin number of led 7
#define PIN_LED8                      23      // Pin number of led 8

#define PIN_BTN1                      13      // Pin number of button 1
#define PIN_BTN2                      20      // Pin number of button 2
#define PIN_BTN3                      5       // Pin number of button 3
#define PIN_BTN4                      21      // Pin number of button 4

#define PIN_SDR1                      A10     // Pin number of slider
#define PIN_SDR2                      A7      // Pin number of vSlider
#define PIN_SDR2_LED                  9       // Pin number of vSlider

#define PIN_VBAT                      A9      // Pin number of battery check

#define VBAT_CHECK                    30      // Battery check interval in minutes

// BLE initializing
Adafruit_BluefruitLE_SPI BLE(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
// BLE MIDI initializing
Adafruit_BLEMIDI MidiBLE(BLE);
// LAST BATTERY CHECK TIME
unsigned long LAST_VBAT_CHECK = 0;

/**
 *  Check battery level
 **/
void CheckBattery(){
  float level = (((float)analogRead(PIN_VBAT) * 2) * 3.3) / 1024;
  DEBUG_PRINT(F("VBAT: "));
  DEBUG_PRINTLN(level);

  if(level > 3.6){
    DEBUG_PRINTLN(F("VBAT IS GREATHER THAN 3.6 VOLTS!"));
  }else if(level > 3.5 && level < 3.7){
    DEBUG_PRINTLN(F("VBAT IS MORE OR LESS 3.6 VOLTS!"));
  }else{
    DEBUG_PRINTLN(F("VBAT IS LESS OR EQUAL THAN 3.5 VOLTS!"));
  }
}
