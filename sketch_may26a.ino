#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
static const u1_t PROGMEM APPEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const u1_t PROGMEM DEVEUI[8] = { 0x89, 0x7B, 0x07, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 }; 
static const u1_t PROGMEM APPKEY[16] = { 0xEF, 0x13, 0x12, 0x4A, 0xFF, 0x03, 0xB1, 0x2C, 0xB9, 0xFA, 0x33, 0x6C, 0x9F, 0x61, 0x5A, 0xEE };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16); }
static osjob_t sendjob;
const unsigned TX_INTERVAL = 30;
const lmic_pinmap lmic_pins = { .nss = 10, .rxtx = LMIC_UNUSED_PIN, .rst = 9, .dio = {2, 3, LMIC_UNUSED_PIN}, };
void onEvent (ev_t ev) {
    switch(ev) {
        case EV_JOINING: Serial.println(F("Connexion TTN...")); break;
        case EV_JOINED: Serial.println(F("[SUCCES] Connecte !")); LMIC_setLinkCheckMode(0); break;
        case EV_TXCOMPLETE: Serial.println(F("[SUCCES] Message envoye !")); os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send); break;
        default: break;
    }
}
void do_send(osjob_t* j){
    if (LMIC.opmode & OP_TXRXPEND) { Serial.println(F("Attente...")); } 
    else { uint8_t mydata[] = "Boite Outils Active"; LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0); Serial.println(F("Trame envoyee...")); }
}
void setup() { Serial.begin(115200); delay(1000); Serial.println(F("--- Boite a Outils ---")); os_init(); LMIC_reset(); do_send(&sendjob); }
void loop() { os_runloop_once(); }
