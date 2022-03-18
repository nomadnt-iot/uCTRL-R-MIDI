#include <Arduino.h>
#include <EEPROM.h>

#define CONFIG_EEADDR   0

#define CONFIG_MODE_PC  0   // Used by mode3 and mode4 mean MODE PROGRAM CHANGE
#define CONFIG_MODE_CCL 1   // Used by mode3 and mode4 mean MODE CONTROL CHANGE LATCH
#define CONFIG_MODE_CCM 2   // Used by mode3 and mode4 mean MODE CONTROL CHANGE MOMENTARY

/**
 *  generic event structure used by MODE3 and MODE4
 **/
struct config_event_t{
    uint8_t change;
    uint8_t value;
};

/**
 *  configuration structure
 **/
struct config_t{
    uint32_t magic;

    // button config struct
    struct btn_t{
        unsigned int click;         // click duration
        unsigned int shold;         // short hold duration
        unsigned int lhold;         // long hold duration
    } btn;

    // slider config struct
    struct sdr_t {
        uint8_t control_change;     // control change to send
        uint8_t channel;            // channel used from vslider
        uint8_t min;                // minimum stored value
        uint8_t max;                // minimum stored value
    } sdr;

    // vslider config struct
    struct vsdr_t{
        uint8_t lock:1;             // enable or disable vslider lock
        uint8_t control_change;     // control change to send
        uint8_t channel;            // channel used from vslider
    } vsdr;

    // mode1 config struct
    struct config_mode1_t{
        uint8_t channel;            // channel to be used
        uint8_t click[4];           // value to send for each button click event
        uint8_t hold[4];            // value to send for each button hold event
    } mode1;

    // mode2 config struct
    struct config_mode2_t{
        uint8_t channel;            // channel to be used
        uint8_t ledbar;             // ledbar configuration
        uint8_t click[4];           // value to send for each button click event
        uint8_t hold[4];            // value to send for each button hold event
    } mode2;

    // NEL MODE3 E NEL MODE4 LEDBAR POTREBBE ANDARE VIA IN QUANTO RIUSCIAMO
    // A RECUPERARE LO STATO DEI LED DAI DATI DA CONFIG_EVENT_T.
    // IN QUESTO MODO RISPARMIEREMMO 2 BYTES MA LA MODE3 E MODE4 FUNZIONEREBBERO
    // IN MANIERA DIFFERENTE RISPETTO A MODE2 IL CHE CREEREBBE PIÙ ECCEZIONI
    // DA GESTIRE. SICURAMENTE QUANDO MODE2 SARÀ GESTITO COME SUBMODE1 LO ADEGUEREMO

    // mode3 config struct
    struct config_mode3_t{
        uint8_t channel;            // channel to be used
        uint8_t ledbar;             // ledbar configuration only used when mode > 0
        uint8_t mode:2;             // 0 ~ Program Change; 1 ~ Control Change (Latch); 2 ~ Control Change (Momentary)
        config_event_t click[4];    // event structure for each button click
        config_event_t hold[4];     // event structure for each button hold
    } mode3;

    // mode4 config struct
    struct config_mode4_t{
        uint8_t channel;            // channel to be used
        uint8_t ledbar;             // ledbar configuration only used when mode > 0
        uint8_t mode:2;             // 0 ~ Program Change; 1 ~ Control Change (Latch); 2 ~ Control Change (Momentary)
        config_event_t click[4];    // event structure for each button click
        config_event_t hold[4];     // event structure for each button hold
    } mode4;

} __attribute__((packed));

// Configuration
config_t cfg;

// Default configuration
const config_t cfg_default = {
    // MAGIC
    0x6C61706F,
    // BUTTONS
    { 0, 200, 1000L },
    // SLIDER
    { 10, 1, 0, 0 },
    // VSLIDER
    { 1, 127, 0 },
    // MODE1
    { 1, { 0, 1, 2, 3 }, { 4, 5, 6, 7 } },
    // MODE 2
    { 1, 0, { 1, 2, 3, 4 }, { 5, 6, 7, 8 } },
    // MODE 3
    { 1, 0, CONFIG_MODE_PC, { { 10, 0 }, { 11, 0 }, { 12, 0 }, { 13, 0 } }, { { 14, 0 }, { 15, 0 }, { 16, 0 }, { 17, 0 } } },
    // MODE 4
    { 1, 0, CONFIG_MODE_PC, { { 20, 0 }, { 21, 0 }, { 22, 0 }, { 23, 0 } }, { { 24, 0 }, { 25, 0 }, { 26, 0 }, { 27, 0 } } },
};

/**
 *  Config helper
 **/
namespace Config{

    /**
     *  Load configuration from EEPROM to cfg
     **/
    void load(){
        EEPROM.get(CONFIG_EEADDR, cfg);
        if(cfg.magic != cfg_default.magic) {
            cfg = cfg_default;
            return;
        }
    }

    /**
     *  Initialize configuration
     **/
    void init(){
        Config::load();
    }

    /**
     *  Store configuration to EEPROM
     **/
    void save(){
        EEPROM.put(CONFIG_EEADDR, cfg);
    }

    /**
     *  Restore configuration to default value
     **/
    void drop(){
        cfg = cfg_default;
        Config::save();
    }

    /**
     *  Return size of configuration in bytes
     *  @return unsigned int bytes
     **/
     unsigned int size(){
         return sizeof(config_t);
     }
}
