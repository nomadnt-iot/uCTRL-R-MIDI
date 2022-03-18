#include <Arduino.h>
#include <uCTRL.h>
#include <Config.h>
#include <Modes.h>

// A small helper
void error(const __FlashStringHelper *err){
    DEBUG_PRINTLN(err);
    while(true);
}

void setup(void){
    delay(SETUP_DELAY);

    Serial.begin(9600);

    char BUFFER[24] = { 0 }; // Buffer used by strings
    sprintf_P(BUFFER, PSTR("%s v%s\r\n"), NAME, VERSION);

    DEBUG_PRINTLN(BUFFER);

    // Configuro l'analogReference su EXTERNAL (usato per risoluzione iSlider)
    analogReference(EXTERNAL);

    // Inizializzo la configurazione del controller
    DEBUG_PRINT(F(" > Initializing Config:      "));
    Config::init();
    DEBUG_PRINTLN(F("[ OK ]"));

    // Inizializzo i led del controller
    DEBUG_PRINT(F(" > Initializing LEDs:        "));
    // accendo e spengo i led del controller
    for(uint8_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); i++){
        LED[i].on(); delay(300); LED[i].off();
    }
    // accendo e spengo i led del virtual slider
    iSDR_LED.on(); delay(300); iSDR_LED.off();
    DEBUG_PRINTLN(F("[ OK ]"));

    // Inizializzo gli slider virtuali
    DEBUG_PRINT(F(" > Initializing Sliders:     "));
    // Configuro la callback chiamata quando iSlider viene bloccato
    iSDR.on(iSLIDER_LOCKED, [](){ iSDR_LED.on(); });
    // Configuro la callback chiamata quando iSlider viene sbloccato
    iSDR.on(iSLIDER_UNLOCKED, [](){ iSDR_LED.off(); });
    DEBUG_PRINTLN(F("[ OK ]"));

    // Inizializzo gli slider virtuali
    DEBUG_PRINT(F(" > Initializing Buttons:     "));
    // Setting up buttons times
    for(uint8_t i = 0; i < (sizeof(BTN)/sizeof(BTN[0])); i++){
        BTN[i].setDoubleTreshold(cfg.btn.click);
        BTN[i].setHoldTreshold(cfg.btn.shold);
        BTN[i].setLongTreshold(cfg.btn.lhold);
    }
    DEBUG_PRINTLN(F("[ OK ]"));

    // Inizializzo la libreria MIDI
    DEBUG_PRINT(F(" > Initializing MIDI:        "));
    MIDI.begin();
    DEBUG_PRINTLN(F("[ OK ]"));

#ifdef ARDUINO_AVR_FEATHER32U4
    // Inizializzo il modulo bluetooth
    DEBUG_PRINT(F(" > Initializing BLE:         "));
    if(!BLE.begin(VERBOSE_MODE)){
        error(F("Couldn't find BLE, make sure it's in CoMmanD mode & check wiring?"));
    }
    DEBUG_PRINTLN(F("[ OK ]"));

    if(FACTORYRESET_ENABLE){
        /* Perform a factory reset to make sure everything is in a known state */
        DEBUG_PRINT(F(" >> BLE factory reset:       "));
        if(!BLE.factoryReset()){
            error(F("Couldn't factory reset"));
        }
        DEBUG_PRINTLN(F("[ OK ]"));
    }

    BLE.echo(false);

    /* Change the device name to make it easier to find */
    DEBUG_PRINT(F(" >> Setting name to 'uCTRL': "));
    sprintf_P(BUFFER, PSTR("AT+GAPDEVNAME=%s"), NAME);
    if(!BLE.sendCommandCheckOK(BUFFER)){
        error(F("Could not set device name"));
    }
    DEBUG_PRINTLN(F("[ OK ]"));

    DEBUG_PRINT(F(" >> Enable BLE MIDI:         "));
    if(!MidiBLE.begin(true)){
        error(F("Could not enable MIDI"));
    }
    DEBUG_PRINTLN(F("[ OK ]"));

    BLE.verbose(false);

#endif

    cstate = MODE1;
    pstate = NULL;

    // tolgo il bind configuration dal bottone
    BTN[3].off();

    // invio l'ultimo program change memorizzato
    SendProgramChange(LPC);
}

void loop(){
    // reading for commands
    while(Serial.available() > 0){
        char c = (char)Serial.read();

        if(c == 'G'){
            // Serial.println("Get configuration");
            Serial.write((uint8_t*) &cfg, sizeof(cfg));
        }else if(c == 'S'){
            Serial.println("Set configuration");
            // Serial.readBytes(buffer, sizeof(buffer));
        }else if(c == 'R'){
            cfg = cfg_default;
            EEPROM.put(CONFIG_EEADDR, cfg);
            Serial.println("Reset configuration");
        }else{
            Serial.println("Unknown command");
        }
    }

#ifdef ARDUINO_AVR_FEATHER32U4
    // interval for each scanning ~ 500ms (non blocking)
    BLE.update(500);
#endif

    if(cstate != CALIBRATION){
        uint16_t reading = 0;

        // SLIDER RELATED CODE
        for(uint8_t j = 0; j < SAMPLES; j++) reading += analogRead(PIN_SDR1);
        // converto la media delle letture in valori compresi tra 0 e 255
        reading = map((reading / SAMPLES), 0, 1023, 0, 255);
        // rendo il valore coerente con la scala accettta da MIDI
        reading = constrain(map(reading, cfg.sdr.min, cfg.sdr.max, 0 , 127), 0, 127);

        // questa roba mi serve per evitare di inviare continuamente ControlChange anche
        // quando non ci sono variazioni di valori... rinominare con nuovo valore opportuno
        if(LAST_SDR != reading){
            LAST_SDR = reading;
            SendControlChange(cfg.sdr.control_change, LAST_SDR, cfg.sdr.channel);
        }

        // VSLIDER RELATED CODE
        reading = map(constrain(iSDR.read(), SLIDER_MIN, SLIDER_MAX), SLIDER_MIN, SLIDER_MAX, 0, 127);

        // questa roba mi serve per evitare di inviare continuamente ControlChange anche
        // quando non ci sono variazioni di valori... rinominare con nuovo valore opportuno
        // sarebbe anche il caso di provare questa funzione su slider normale
        if(LAST_iSDR != reading){
            LAST_iSDR = reading;
            SendControlChange(cfg.vsdr.control_change, LAST_iSDR, cfg.vsdr.channel);
        }

        // delay just some millis
        delay(5);
    }

    // running current state function
    cstate();

    // set the previous state as current state
    pstate = cstate;

    // keep watching the push button
    for(uint8_t i = 0; i < (sizeof(BTN)/sizeof(BTN[0])); i++) BTN[i].tick();
    // updating leds of controller
    for(uint8_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); i++) LED[i].update();
    // updating vslider led
    iSDR_LED.update();
}

/**
 *  Send program change helper
 *  @param uint8_t program_change
 *  @param uint8_t channel
 **/
void SendProgramChange(uint8_t program_change, uint8_t channel){
    // Send via standard Midi
    MIDI.sendProgramChange(program_change, channel);

#ifdef TEENSYLC
    usbMIDI.sendProgramChange(program_change, channel);
#endif

#ifdef ARDUINO_AVR_FEATHER32U4
    // Send via MidiUSB
    MidiUSB.sendMIDI({ 0x0C, (uint8_t)(0xC0 | channel), program_change });
    MidiUSB.flush();
    // Send via standard MidiBLE
    MidiBLE.send(0xC0, channel, program_change);
#endif
}

/**
 *  Send control change helper
 *  @param uint8_t control_change
 *  @param uint8_t value
 *  @param uint8_t channel
 **/
void SendControlChange(uint8_t control_change, uint8_t value, uint8_t channel){
    // Send via standard Midi
    MIDI.sendControlChange(control_change, value, channel);

#ifdef TEENSYLC
    usbMIDI.sendControlChange(control_change, value, channel);
#endif

#ifdef ARDUINO_AVR_FEATHER32U4
    // Send via MidiUSB
    MidiUSB.sendMIDI({ 0x0B, (uint8_t)(0xB0 | channel), control_change, value });
    MidiUSB.flush();
    // Send via standard MidiBLE
    MidiBLE.send(0xB0, control_change, value);
#endif
}

/**
 *  Config led with single uint8_t with bitmask tecnique
 *  @param uint8_t ledbar
 **/
void ConfigLeds(uint8_t ledbar){
    for(uint8_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); i++){
        (ledbar & 1 << i) ? LED[i].on() : LED[i].off();
    }
}

/**
 *  Detach all Buttons callbacks
 **/
void UnbindButtons(){
    for(uint8_t i = 0; i < (sizeof(BTN)/sizeof(BTN[0])); i++){ BTN[i].off(); }
}
