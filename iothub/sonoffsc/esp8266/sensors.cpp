#include "SerialLink.h"
#include "sensors.h"

SerialLink link(Serial, false);

const PROGMEM char at_hello[] = "AT+HELLO";
const PROGMEM char at_push[] = "AT+PUSH";
const PROGMEM char at_every[] = "AT+EVERY";
const PROGMEM char at_temp[] = "AT+TEMP";
const PROGMEM char at_hum[] = "AT+HUM";
const PROGMEM char at_dust[] = "AT+DUST";
const PROGMEM char at_noise[] = "AT+NOISE";
const PROGMEM char at_light[] = "AT+LIGHT";

// -----------------------------------------------------------------------------
// VALUES
// -----------------------------------------------------------------------------

static float temperature;
static int humidity;
static int light;
static float dust;
static int noise;

bool gotResponse = false;
long response;

float getTemperature() { return temperature; }
float getHumidity() { return humidity; }
float getLight() { return light; }
float getDust() { return dust; }
float getNoise() { return noise; }

// -----------------------------------------------------------------------------
// COMMUNICATIONS
// -----------------------------------------------------------------------------

bool sensorsGet(char * key) {
    return false;
}

bool sensorsSet(char * key, long value) {

    char buffer[50];

    if (strcmp_P(key, at_temp) == 0) {
        temperature = (float) value;
        return true;
    }

    if (strcmp_P(key, at_hum) == 0) {
        humidity = value;
        return true;
    }

    if (strcmp_P(key, at_light) == 0) {
        light = value;
        return true;
    }

    if (strcmp_P(key, at_dust) == 0) {
        dust = (float) value;
        return true;
    }

    if (strcmp_P(key, at_noise) == 0) {
        noise = value;
        return true;
    }

    gotResponse = true;
    response = value;
    return true;
}

void sensorsSetup(void) {

    link.onGet(sensorsGet);
    link.onSet(sensorsSet);
    link.clear();
    delay(200);
  
	link.send_P(at_every, 60);	
	link.send_P(at_push, 1);
}

void sensorsLoop(void) {
    link.handle();
}
