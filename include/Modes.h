/**
 *  MODE 1
 *
 *  Il MODE 1 invia solo Program Change e si serve del MODE 2 per configurare
 *  i ControlChange di ogni Program Change 8x8
 **/
void MODE1(){
    // SETUP MODE 1
    if(cstate != pstate){
        DEBUG_PRINTLN(F("Switching to MODE(1)..."));

        // DETACH ALL BUTTONS CALLBACK
        UnbindButtons();

        // BUTTONS CLICK RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_CLICK, [](){
            SendProgramChange(cfg.mode1.click[0], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 0); // spengo tutti i led ed accendo solo il numero 1
        });
        BTN[1].on(iBUTTON_EVENT_CLICK, [](){
            SendProgramChange(cfg.mode1.click[1], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 2); // spengo tutti i led ed accendo solo il numero 3
        });
        BTN[2].on(iBUTTON_EVENT_CLICK, [](){
            SendProgramChange(cfg.mode1.click[2], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 4); // spengo tutti i led ed accendo solo il numero 5
        });
        BTN[3].on(iBUTTON_EVENT_CLICK, [](){
            SendProgramChange(cfg.mode1.click[3], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 6); // spengo tutti i led ed accendo solo il numero 7
        });

        // BUTTONS HOLD RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_HOLD, [](){
            SendProgramChange(cfg.mode1.hold[0], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 1); // spengo tutti i led ed accendo solo il numero 1
        });
        BTN[1].on(iBUTTON_EVENT_HOLD, [](){
            SendProgramChange(cfg.mode1.hold[1], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 3); // spengo tutti i led ed accendo solo il numero 3
        });
        BTN[2].on(iBUTTON_EVENT_HOLD, [](){
            SendProgramChange(cfg.mode1.hold[2], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 5); // spengo tutti i led ed accendo solo il numero 5
        });
        BTN[3].on(iBUTTON_EVENT_HOLD, [](){
            SendProgramChange(cfg.mode1.hold[3], cfg.mode1.channel);
            ConfigLeds(0 ^ 1 << 7); // spengo tutti i led ed accendo solo il numero 7
        });

        // ATTACH BUTTON EVENT CALIBRATION
        BTN[0].on(iBUTTON_EVENT_LONG, [](){ cstate = CALIBRATION; });
        // ATTACH BUTTON EVENT SWITCH TO MODE 2
        BTN[1].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE2; });
        // ATTACH BUTTON EVENT SWITCH TO MODE 3
        BTN[2].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE3; });
        // ATTACH BUTTON EVENT SWITCH TO MODE 4
        BTN[3].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE4; });

        // SPENGO TUTTI I LED
        ConfigLeds(0);

        // LAMPEGGIA 3 VOLE IL LED 1
        LED[0].blink();
    }
}

/**
 *  MODE 2
 *  il MODE 2 è una subroutine del MODE 1 in cui vengono scelti i Control Change
 *  da associare al Program Change padre.
 *  Nelle prossime versioni questo verrà rinominato in SUBMODE1
 **/
void MODE2(){
    // SETUP MODE 2
    if(cstate != pstate){
        DEBUG_PRINTLN(F("Switching to MODE(2)..."));

        // DETACH ALL BUTTONS CALLBACK
        UnbindButtons();

        // BUTTONS CLICK RELATED CALLBACKS
        // Invia 0 o 127 in funzione dello stato di ledbar. Se il led relativo
        // al pusante premuto è spento, mando 127 altrimenti 0.
        // Per fare questo giochetto utilizzo la tecnica del bitmask per usare
        // un solo byte in EEPROM
        // LEDBAR ^= 1 << n -- toggle bit at index n
        // LEDBAR & 1 << n -- get bit status at index n
        BTN[0].on(iBUTTON_EVENT_CLICK, [](){
            cfg.mode2.ledbar ^= 1 << 0; // effettuo il toggle del bit in posizione 0
            SendControlChange(cfg.mode2.click[0], (cfg.mode2.ledbar & 1 << 0) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });
        BTN[1].on(iBUTTON_EVENT_CLICK, [](){
            cfg.mode2.ledbar ^= 1 << 2;
            SendControlChange(cfg.mode2.click[1], (cfg.mode2.ledbar & 1 << 2 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });
        BTN[2].on(iBUTTON_EVENT_CLICK, [](){
            cfg.mode2.ledbar ^= 1 << 4;
            SendControlChange(cfg.mode2.click[2], (cfg.mode2.ledbar & 1 << 4 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });
        BTN[3].on(iBUTTON_EVENT_CLICK, [](){
            cfg.mode2.ledbar ^= 1 << 6;
            SendControlChange(cfg.mode2.click[3], (cfg.mode2.ledbar & 1 << 6 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });

        // BUTTONS HOLD RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_HOLD, [](){
            cfg.mode2.ledbar ^= 1 << 1;
            SendControlChange(cfg.mode2.hold[0], (cfg.mode2.ledbar & 1 << 1 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });
        BTN[1].on(iBUTTON_EVENT_HOLD, [](){
            cfg.mode2.ledbar ^= 1 << 3;
            SendControlChange(cfg.mode2.hold[1], (cfg.mode2.ledbar & 1 << 3 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });
        BTN[2].on(iBUTTON_EVENT_HOLD, [](){
            cfg.mode2.ledbar ^= 1 << 5;
            SendControlChange(cfg.mode2.hold[2], (cfg.mode2.ledbar & 1 << 5 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });
        BTN[3].on(iBUTTON_EVENT_HOLD, [](){
            cfg.mode2.ledbar ^= 1 << 7;
            SendControlChange(cfg.mode2.hold[3], (cfg.mode2.ledbar & 1 << 7 ) ? 127 : 0, cfg.mode2.channel);
            ConfigLeds(cfg.mode2.ledbar); // aggiorno lo stato dei led
        });

        // ATTACH BUTTON EVENT SWITCH TO MODE 1
        BTN[0].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE1; });
        // attach store date into eeprom and switch to MODE 1
        BTN[1].on(iBUTTON_EVENT_LONG, [](){
            DEBUG_PRINTLN(F("Storing MODE(2) data into EEPROM..."));
            Config::save();
            cstate = MODE1;
        });
        // ATTACH BUTTON EVENT SWITCH TO MODE 3
        BTN[2].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE3; });
        // ATTACH BUTTON EVENT SWITCH TO MODE 4
        BTN[3].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE4; });

        // SPENGO TUTTI I LED
        ConfigLeds(0);

        // LAMPEGGIA 3 VOLTE IL LED 3
        LED[2].blink();

        // AGGIORNO LO STATO DEI LEDs ALL'ULTIMO CONFIGURAZIONE
        ConfigLeds(cfg.mode2.ledbar);
    }
}

/**
 *  MODE 3
 *  FUNZIONAMENTO:
 *  Quando entro i led sono tutti spenti!
 *  Quando il led è acceso mando 0 come valore;
 *  Quando il led è spento mando 127 come valore;
 **/
void MODE3(){
    // SETUP MODE 3
    if(cstate != pstate){
        DEBUG_PRINTLN(F("Switching to MODE(3)..."));

        // DETACH ALL BUTTONS CALLBACK
        UnbindButtons();

        // BUTTONS CLICK RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_CLICK, [](){
            // PROGRAM CHANGE MODE
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 0); // spengo tutti i led ed accendo solo il numero 1
                SendProgramChange(cfg.mode3.click[0].change, cfg.mode3.channel);
            }
            // CONTROL CHANGE LATCH MODE
            else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 0; // effettuo il toggle del bit in posizione 0
                SendControlChange(cfg.mode3.click[0].change, (cfg.mode3.ledbar & 1 << 0) ? 127 : 0, cfg.mode3.channel);
            }
            // CONTROL CHANGE MOMENTARY MODE
            else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 0);
                SendControlChange(cfg.mode3.click[0].change, cfg.mode3.click[0].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });
        BTN[1].on(iBUTTON_EVENT_CLICK, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 2); // spengo tutti i led ed accendo solo il numero 3
                SendProgramChange(cfg.mode3.click[1].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 2; // effettuo il toggle del bit in posizione 2
                SendControlChange(cfg.mode3.click[1].change, (cfg.mode3.ledbar & 1 << 2) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 2);
                SendControlChange(cfg.mode3.click[1].change, cfg.mode3.click[1].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });
        BTN[2].on(iBUTTON_EVENT_CLICK, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 4); // spengo tutti i led ed accendo solo il numero 5
                SendProgramChange(cfg.mode3.click[2].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 4; // effettuo il toggle del bit in posizione 4
                SendControlChange(cfg.mode3.click[2].change, (cfg.mode3.ledbar & 1 << 4) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 4);
                SendControlChange(cfg.mode3.click[2].change, cfg.mode3.click[2].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });
        BTN[3].on(iBUTTON_EVENT_CLICK, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 6); // spengo tutti i led ed accendo solo il numero 7
                SendProgramChange(cfg.mode3.click[3].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 6; // effettuo il toggle del bit in posizione 6
                SendControlChange(cfg.mode3.click[3].change, (cfg.mode3.ledbar & 1 << 6) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 6);
                SendControlChange(cfg.mode3.click[3].change, cfg.mode3.click[3].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });

        // BUTTONS HOLD RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 1); // spengo tutti i led ed accendo solo il numero 2
                SendProgramChange(cfg.mode3.hold[0].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 1; // effettuo il toggle del bit in posizione 1
                SendControlChange(cfg.mode3.hold[0].change, (cfg.mode3.ledbar & 1 << 1) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 1);
                SendControlChange(cfg.mode3.hold[0].change, cfg.mode3.hold[0].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });
        BTN[1].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 3); // spengo tutti i led ed accendo solo il numero 4
                SendProgramChange(cfg.mode3.hold[1].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 3; // effettuo il toggle del bit in posizione 3
                SendControlChange(cfg.mode3.hold[1].change, (cfg.mode3.ledbar & 1 << 3) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 3);
                SendControlChange(cfg.mode3.hold[1].change, cfg.mode3.hold[1].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });
        BTN[2].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 5); // spengo tutti i led ed accendo solo il numero 6
                SendProgramChange(cfg.mode3.hold[2].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 5; // effettuo il toggle del bit in posizione 5
                SendControlChange(cfg.mode3.hold[2].change, (cfg.mode3.ledbar & 1 << 5) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 5);
                SendControlChange(cfg.mode3.hold[2].change, cfg.mode3.hold[2].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });
        BTN[3].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode3.mode == CONFIG_MODE_PC){
                cfg.mode3.ledbar = (0 ^ 1 << 7); // spengo tutti i led ed accendo solo il numero 8
                SendProgramChange(cfg.mode3.hold[3].change, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCL){
                cfg.mode3.ledbar ^= 1 << 7; // effettuo il toggle del bit in posizione 7
                SendControlChange(cfg.mode3.hold[3].change, (cfg.mode3.ledbar & 1 << 7) ? 127 : 0, cfg.mode3.channel);
            }else if(cfg.mode3.mode == CONFIG_MODE_CCM){
                cfg.mode3.ledbar = (0 ^ 1 << 7);
                SendControlChange(cfg.mode3.hold[3].change, cfg.mode3.hold[3].value, cfg.mode3.channel);
            }
            ConfigLeds(cfg.mode3.ledbar); // aggiorno lo stato dei led
        });

        // ATTACH BUTTON EVENT SWITCH TO MODE 1
        BTN[0].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE1; });
        // ATTACH BUTTON EVENT SWITCH TO MODE 4
        BTN[3].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE4; });

        // SPENGO TUTTI I LED
        ConfigLeds(0);

        // LAMPEGGIA 3 VOLE IL LED 5
        LED[4].blink();
    }
}

/**
 *  MODE 4
 *  FUNZIONAMENTO:
 *  Quando entro i led sono tutti spenti!
 *  Quando il led è acceso mando 0 come valore;
 *  Quando il led è spento mando 127 come valore;
 **/
void MODE4(){
    // SETUP MODE 4
    if(cstate != pstate){
        DEBUG_PRINTLN(F("Switching to MODE(4)..."));

        // DETACH ALL BUTTONS CALLBACK
        UnbindButtons();

        // BUTTONS CLICK RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_CLICK, [](){
            // PROGRAM CHANGE MODE
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 0); // spengo tutti i led ed accendo solo il numero 1
                SendProgramChange(cfg.mode4.click[0].change, cfg.mode4.channel);
            }
            // CONTROL CHANGE LATCH MODE
            else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 0; // effettuo il toggle del bit in posizione 0
                SendControlChange(cfg.mode4.click[0].change, (cfg.mode4.ledbar & 1 << 0) ? 127 : 0, cfg.mode4.channel);
            }
            // CONTROL CHANGE MOMENTARY MODE
            else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 0);
                SendControlChange(cfg.mode4.click[0].change, cfg.mode4.click[0].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });

        BTN[1].on(iBUTTON_EVENT_CLICK, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 2); // spengo tutti i led ed accendo solo il numero 3
                SendProgramChange(cfg.mode4.click[1].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 2; // effettuo il toggle del bit in posizione 2
                SendControlChange(cfg.mode4.click[1].change, (cfg.mode4.ledbar & 1 << 2) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 2);
                SendControlChange(cfg.mode4.click[1].change, cfg.mode4.click[1].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });
        BTN[2].on(iBUTTON_EVENT_CLICK, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 4); // spengo tutti i led ed accendo solo il numero 5
                SendProgramChange(cfg.mode4.click[2].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 4; // effettuo il toggle del bit in posizione 4
                SendControlChange(cfg.mode4.click[2].change, (cfg.mode4.ledbar & 1 << 4) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 4);
                SendControlChange(cfg.mode4.click[2].change, cfg.mode4.click[2].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });
        BTN[3].on(iBUTTON_EVENT_CLICK, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 6); // spengo tutti i led ed accendo solo il numero 7
                SendProgramChange(cfg.mode4.click[3].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 6; // effettuo il toggle del bit in posizione 6
                SendControlChange(cfg.mode4.click[3].change, (cfg.mode4.ledbar & 1 << 6) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 6);
                SendControlChange(cfg.mode4.click[3].change, cfg.mode4.click[3].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });

        // BUTTONS HOLD RELATED CALLBACKS
        BTN[0].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 1); // spengo tutti i led ed accendo solo il numero 2
                SendProgramChange(cfg.mode4.hold[0].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 1; // effettuo il toggle del bit in posizione 1
                SendControlChange(cfg.mode4.hold[0].change, (cfg.mode4.ledbar & 1 << 1) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 1);
                SendControlChange(cfg.mode4.hold[0].change, cfg.mode4.hold[0].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });
        BTN[1].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 3); // spengo tutti i led ed accendo solo il numero 4
                SendProgramChange(cfg.mode4.hold[1].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 3; // effettuo il toggle del bit in posizione 3
                SendControlChange(cfg.mode4.hold[1].change, (cfg.mode4.ledbar & 1 << 3) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 3);
                SendControlChange(cfg.mode4.hold[1].change, cfg.mode4.hold[1].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });
        BTN[2].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 5); // spengo tutti i led ed accendo solo il numero 6
                SendProgramChange(cfg.mode4.hold[2].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 5; // effettuo il toggle del bit in posizione 5
                SendControlChange(cfg.mode4.hold[2].change, (cfg.mode4.ledbar & 1 << 5) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 5);
                SendControlChange(cfg.mode4.hold[2].change, cfg.mode4.hold[2].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });
        BTN[3].on(iBUTTON_EVENT_HOLD, [](){
            if(cfg.mode4.mode == CONFIG_MODE_PC){
                cfg.mode4.ledbar = (0 ^ 1 << 7); // spengo tutti i led ed accendo solo il numero 8
                SendProgramChange(cfg.mode4.hold[3].change, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCL){
                cfg.mode4.ledbar ^= 1 << 7; // effettuo il toggle del bit in posizione 7
                SendControlChange(cfg.mode4.hold[3].change, (cfg.mode4.ledbar & 1 << 7) ? 127 : 0, cfg.mode4.channel);
            }else if(cfg.mode4.mode == CONFIG_MODE_CCM){
                cfg.mode4.ledbar = (0 ^ 1 << 7);
                SendControlChange(cfg.mode4.hold[3].change, cfg.mode4.hold[3].value, cfg.mode4.channel);
            }
            ConfigLeds(cfg.mode4.ledbar); // aggiorno lo stato dei led
        });

        // ATTACH BUTTON EVENT SWITCH TO MODE 1
        BTN[0].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE1; });
        // ATTACH BUTTON EVENT SWITCH TO MODE 3
        BTN[2].on(iBUTTON_EVENT_LONG, [](){ cstate = MODE3; });

        // SPEGNE TUTTI I LED
        ConfigLeds(0);

        // LAMPEGGIA 3 VOLTE IL LED 1
        LED[6].blink();
    }
}

/**
 *  MODE 5 - CALIBRATION
 **/
void CALIBRATION(){
    if(cstate != pstate){
        DEBUG_PRINTLN(F("Switching to CALIBRATION..."));

        // DETACH ALL BUTTONS CALLBACK
        UnbindButtons();

        // Attach button event to exit from CALIBRATION
        for(uint8_t i = 0; i < (sizeof(BTN)/sizeof(BTN[0])); i++){
            BTN[i].on(iBUTTON_EVENT_CLICK, [](){
                DEBUG_PRINTLN(F("Storing CALIBRATION data into EEPROM..."));
                Config::save();
                cstate = MODE1;
            });
        }

        // SPEGNE TUTTI I LEDs LAMPEGGIA 3 VOLTE IL LED 7
        LED[1].blink();

        // imposto i valori di partenza su tutti gli slider
        cfg.sdr.min = 255;
        cfg.sdr.max = 0;
    }

    // Calibro lo slider
    uint16_t avg = 0;
    for(uint8_t j = 0; j < SAMPLES; j++) avg += analogRead(PIN_SDR1);
    avg = map((avg / SAMPLES), 0, 1023, 0 , 255);

    if(avg < cfg.sdr.min){ cfg.sdr.min = avg; }
    if(avg > cfg.sdr.max){ cfg.sdr.max = avg; }

    delay(5);
}
