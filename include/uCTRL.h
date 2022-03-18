#include <EEPROM.h>
#include <MIDI.h>
#include <iButton.h>
#include <iSlider.h>
#include <iLed.h>
#include <iCLI.h>

/******************************************************************************/
/* COMMON CONFIGURATIONS                                                      */
/******************************************************************************/
// DEBUG HELPER
#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

// MAX VALUES CONSTANTS AS PLURAL
#define SAMPLES         30                      // Max number of READINGS where to calculate average

// SHARP GP2Y0A51SK0F 2cm to 15cm defines
#define SLIDER_MIN      200                     // This is more or less equals to 10 cm
#define SLIDER_MAX      500                     // This is more or less equals to 4 cm

#define SETUP_DELAY     3000L
#define CONFIG_TIMEOUT  10000L                  // max wait to entering in config mode

/******************************************************************************/
/* TEENSY LC RELATED CONFIGURATION                                            */
/* leds pins, buttons, sliders and others                                     */
/******************************************************************************/
#ifdef ARDUINO_TEENSYLC
    #include <boards/teensylc.h>
#endif

/******************************************************************************/
/* FEATHER32U4 RELATED CONFIGURATION                                          */
/* leds pins, buttons, sliders and others                                     */
/******************************************************************************/
#ifdef ARDUINO_AVR_FEATHER32U4
    #include <boards/feather32u4.h>
#endif

MIDI_CREATE_DEFAULT_INSTANCE();

// VARUABILI GLOBALI SPOSTATE DA uCTRL.CPP
// uCTRL BUTTONs DEFINITION
iButton BTN[]       = { iButton(PIN_BTN1), iButton(PIN_BTN2), iButton(PIN_BTN3), iButton(PIN_BTN4) };
// uCTRL LEDs BAR DEFINITION
iLed LED[]          = { iLed(PIN_LED1), iLed(PIN_LED2), iLed(PIN_LED3), iLed(PIN_LED4), iLed(PIN_LED5), iLed(PIN_LED6), iLed(PIN_LED7), iLed(PIN_LED8) };
// iSLIDERs PIN and LED DEFINITION
iSlider iSDR        = iSlider(PIN_SDR2, 20, 900);
iLed iSDR_LED       = iLed(PIN_SDR2_LED);

uint16_t LAST_SDR   = 0;                      // Last slider sended value trought MIDI
uint16_t LAST_iSDR  = 0;                      // Last iSlider sended value trought MIDI
uint8_t LPC         = 0;                      // Last Program Change index of PROGRAM

// iCLI CLI(Serial, 32);                         // iCLI object instance

// Overload dell'operatore << da utilizzare nelle Serial
template <class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

// Copy string helper
template <typename T, int size> void CopyString(const String& s, T(&arr)[size]) { s.toCharArray(arr, size); }

/**
 *  A very simple error helper
 *  @param const __FlashStringHelper *emsg
 **/
void error(const __FlashStringHelper *emsg);

/**
 *  Send program change helper
 *  @param uint8_t program_change
 *  @param uint8_t channel
 **/
void SendProgramChange(uint8_t program_change, uint8_t channel = 1);

/**
 *  Send control change helper
 *  @param uint8_t control_change
 *  @param uint8_t value
 *  @param uint8_t channel
 **/
void SendControlChange(uint8_t control_change, uint8_t value, uint8_t channel = 1);

/**
 *  Config led with single uint8_t with bitmask tecnique
 *  @param uint8_t ledbar
 **/
void ConfigLeds(uint8_t ledbar = 0);

/**
 *  Unbind all buttons events
 **/
void UnbindButtons();

/**
 *  Mode 1
 **/
void MODE1();

/**
 *  Mode 2
 **/
void MODE2();

/**
 *  Mode 3
 **/
void MODE3();

/**
 *  Mode 4
 **/
void MODE4();

/**
 *  Calibration
 **/
void CALIBRATION();

/**
 *  State machine pointers
 **/
void (*cstate)(), (*pstate)();
