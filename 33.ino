// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
#include "Arduino.h"
#include "LTimer.h"
#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"

// Assign AP ssid / password here
#define _SSID "iPhone"
#define _KEY  "1234567891"
MCSDevice mcs("D5cizQ7N", "k1k0Obj5YwFbjsh1");
MCSDisplayFloat s("33");
 MCSDisplayFloat x("44");
float f,h,hic,hif,t;
int led1 = 6;
int val1 = 0; 
LTimer timer1(LTIMER_1);
void setup() {
  Serial.begin(9600);
  
  while(WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  mcs.addChannel(s);
  mcs.addChannel(x);
  while(!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if(mcs.connected())
      Serial.println("MCS connected !!");
  }
  Serial.println(F("DHTxx test!"));
  dht.begin();
  timer1.begin();
  timer1.start(10000, LTIMER_REPEAT_MODE, _callback1, NULL); 
}
void _callback1(void *usr_data)
{ 
   h = dht.readHumidity();
  // Read temperature as Celsius (the default)
   t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
   f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
   hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
   hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("'C "));
  Serial.print(f);
  Serial.print(F("'F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("'C "));
  Serial.print(hif);
  Serial.println(F("'F"));
  
}
void loop() {
  dht.readTemperature();
  dht.readHumidity();
  mcs.process(100);
  x.set(t);
  s.set(h);
}
