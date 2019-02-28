#include <DHT.h>
#include <SerialLink.h>
#include <math.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE         9600
#define LDR_PIN                 A3

#define SHARP_LED_PIN           9
#define SHARP_READ_PIN          A1
#define SHARP_SAMPLING_TIME	    280
#define SHARP_DELTA_TIME		40
#define SHARP_SLEEP_TIME		9680

#define DHT_PIN                 6
#ifndef DHT_TYPE
// Uncomment the sensor type that you have (comment the other if applicable)
#define DHT_TYPE                DHT11
//#define DHT_TYPE                DHT22
#endif
#define DHT_EXPIRES             60000

#define ADC_COUNTS              1024
#define MICROPHONE_PIN          A2
#define NUM_SAMPLES             10

#define MAX_SERIAL_BUFFER       20

#define DEFAULT_EVERY           60
#define DEFAULT_PUSH            0

#define NULL_VALUE              -999

// -----------------------------------------------------------------------------
// Keywords
// -----------------------------------------------------------------------------

const PROGMEM char at_hello[] = "AT+HELLO";
const PROGMEM char at_push[] = "AT+PUSH";
const PROGMEM char at_every[] = "AT+EVERY";
const PROGMEM char at_temp[] = "AT+TEMP";
const PROGMEM char at_hum[] = "AT+HUM";
const PROGMEM char at_dust[] = "AT+DUST";
const PROGMEM char at_noise[] = "AT+NOISE";
const PROGMEM char at_light[] = "AT+LIGHT";

// -----------------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------------

SerialLink link(Serial);
DHT dht(DHT_PIN, DHT_TYPE);

bool push = DEFAULT_PUSH;
unsigned long every = 1000L * DEFAULT_EVERY;

float temperature = NULL_VALUE;
int humidity = NULL_VALUE;
float dust = NULL_VALUE;
int light = NULL_VALUE;
int noise = NULL_VALUE;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

// -----------------------------------------------------------------------------
// SENSORS
// -----------------------------------------------------------------------------

int getLight() {
    return map(analogRead(LDR_PIN), 0, ADC_COUNTS, 100, 0);
}

// 0.5V ==> 100ug/m3
float getDust() {

    digitalWrite(SHARP_LED_PIN, LOW);
	delayMicroseconds(SHARP_SAMPLING_TIME);

	float reading = analogRead(SHARP_READ_PIN);

	delayMicroseconds(SHARP_DELTA_TIME);
	digitalWrite(SHARP_LED_PIN, HIGH);

    // mg/m3
	float dust = 170.0 * reading * (5.0 / 1024.0) - 100.0;
    if (dust < 0) dust = 0;
    return dust;

}

void loadTempAndHum() {

    // Check at most once every minute
    static unsigned long last = 0;
    if (millis() - last < DHT_EXPIRES) return;

    // Retrieve data
    double h = dht.readHumidity();
    double t = dht.readTemperature();

    // Check values
    if (isnan(h) || isnan(t)) return;
    temperature = t;
    humidity = h;

    // Only set new expiration time if good reading
    last = millis();

}

float getTemperature() {
    loadTempAndHum();
    return temperature;
}

int getHumidity() {
    loadTempAndHum();
    return humidity;
}

int getNoise() {
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MICROPHONE_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double db = 20 * log10(peakToPeak);  // convert to volts
   return (int)db;
}

// -----------------------------------------------------------------------------
// COMMUNICATION
// -----------------------------------------------------------------------------

// How to respond to AT+...=? requests
bool linkGet(char * key) {

    if (strcmp_P(key, at_push) == 0) {
        link.send(key, push ? 1 : 0, false);
        return true;
    }

    if (strcmp_P(key, at_every) == 0) {
        link.send(key, every / 1000, false);
        return true;
    }

    if (strcmp_P(key, at_temp) == 0) {
        if (every == 0) temperature = getTemperature();
        if (temperature == NULL_VALUE) return false;
        link.send(key, temperature, false);
        return true;
    }

    if (strcmp_P(key, at_hum) == 0) {
        if (every == 0) humidity = getHumidity();
        if (humidity == NULL_VALUE) return false;
        link.send(key, humidity, false);
        return true;
    }

    if (strcmp_P(key, at_noise) == 0) {
        if (every == 0) noise = getNoise();
        if (noise == NULL_VALUE) return false;
        link.send(key, noise, false);
        return true;
    }

    if (strcmp_P(key, at_dust) == 0) {
        if (every == 0) dust = getDust();
        if (dust == NULL_VALUE) return false;
        link.send(key, dust, false);
        return true;
    }

    if (strcmp_P(key, at_light) == 0) {
        if (every == 0) light = getLight();
        if (light == NULL_VALUE) return false;
        link.send(key, light, false);
        return true;
    }

    return false;

}

// Functions for responding to AT+...=<long> commands that set values and functions
bool linkSet(char * key, long value) {

    if (strcmp_P(key, at_push) == 0) {
        if (0 <= value && value <= 1) {
            push = value == 1;
            return true;
        }
    }

    if (strcmp_P(key, at_every) == 0) {
        if (5 <= value && value <= 300) {
            every = 1000L * value;
            return true;
        }
    }

    return false;
}

//Setup callbacks when AT commands are recieved
void linkSetup() {
    link.onGet(linkGet);
    link.onSet(linkSet);
}

// Check for incoming AT+ commands
void linkLoop() {
    link.handle();
}

// -----------------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------------

void setup() {

  	// Setup Serial port
    Serial.begin(SERIAL_BAUDRATE);
    link.send_P(at_hello, 1);

    linkSetup();

	  // Setup physical pins on the ATMega328
	  pinMode(LDR_PIN, INPUT);
    pinMode(DHT_PIN, INPUT);
    pinMode(SHARP_LED_PIN, OUTPUT);
    pinMode(SHARP_READ_PIN, INPUT);
    pinMode(MICROPHONE_PIN, INPUT_PULLUP);

	// Setup the DHT Thermometer/Humidity Sensor
    dht.begin();
}

void loop() {

    static unsigned long last = 0;

    linkLoop();

	// If AT+EVERY>0 then we are sending a signal every so many seconds
    if ((every > 0) && ((millis() - last > every) || (last == 0))) {

        last = millis();

        temperature = getTemperature();
        if (push) link.send_P(at_temp, temperature, false);

        humidity = getHumidity();
        if (push) link.send_P(at_hum, humidity, false);

        light = getLight();
        if (push) link.send_P(at_light, light, false);

        noise = getNoise();
        if (push) link.send_P(at_noise, noise, false);
		
		    dust = getDust();
        if (push) link.send_P(at_dust, dust, false);

    }
}
