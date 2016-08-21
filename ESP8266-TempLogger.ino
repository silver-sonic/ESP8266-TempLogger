/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#include "ESP8266-TempLogger_Config.h"   // See ESP8266-TempLogger_Config.h.sample

// Define WIFI SSID and Password
const char*     ssid = WIFI_SSID;
const char* password = WIFI_SSID_PASSWORD;

// Define Webpage which has the receiver
String url=STORE_URL;

// How long to sleep after sending a set of values
const unsigned long deepSleepSeconds = 1800;

// Define Pin for Sensor - I use GPIO2
#define DHTPIN 2

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// init DHT; 3rd parameter = 16 works for ESP8266@80MHz
// FixMe: What is the third parameter??
DHT dht(DHTPIN, DHTTYPE, 16); 

// Enable ADC to measure Vcc
ADC_MODE(ADC_VCC);

// Some global variables

// Generally, you should use "unsigned long" for variables that hold time
const long interval = 2000;              // interval at which to read sensor
unsigned long previousMillis = 0;        // will store last temp was read

float hum, temp;  // Values read from sensor
unsigned int counter;

int vcc;    // Vcc measured by internal ADC; unit should be 1/1024 Volt


void setup() {

    counter = 0;

    // Short delay after boot... seems needed sometimes
    for(uint8_t t = 4; t > 0; t--) {
        delay(1000);
    }

    Serial.begin(115200);   Serial.println("");
    WiFi.begin(ssid, password);
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
 
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    hum  = dht.readHumidity();          // Read humidity (percent)
    temp = dht.readTemperature();     // Read temperature as Celsius ...(true)=Fahrenheit
    // Check if any reads failed and exit early (to try again).
    if (isnan(hum) || isnan(temp)) {
      return;
    }
  }
}

void loop() {

    String url_act;

    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED)) {

        gettemperature();
        vcc=ESP.getVcc();
        
        HTTPClient http;
        Serial.println("HTTP Start");

        url_act  = url;
        url_act += "?temp=";    url_act += temp;
        url_act += "&hum=";     url_act += hum;
        url_act += "&counter="; url_act += counter;
        url_act += "&vcc=";     url_act += vcc;
        
        Serial.print("URL="); Serial.println(url_act);
        http.begin(url_act); //HTTP-Client

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            // file found at server
            Serial.println("HTTP Code OK");
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
            }
        } else {
            // We do nothing if something did not work...
            Serial.println("HTTP Not OK");
        }
        http.end();
        Serial.println("HTTP End");
    }
  counter++;
  Serial.println("----- Going to deep sleep for some time -----"); Serial.println("");
  delay(250); // to make sure any messages from above are really transmitted before going to sleep

  ESP.deepSleep(deepSleepSeconds * 1000 * 1000);
  delay(100);  //needed to really go into sleep
}


