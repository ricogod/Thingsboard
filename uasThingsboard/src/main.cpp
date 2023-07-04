#include <Arduino.h>
#include <dht.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <ThingsBoard.h>

//define pin
#define DhtPin 18
#define DhtType DHT11

//define wifi
#define SSID "Rico"
#define PASS "peterparkour"

//define Tb

#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define THINGSBOARD_ACCESS_TOKEN "mVG1d9m8VI5hw6za6YUs"

DHT dht(DhtPin, DhtType);

BH1750 lightMeter(0x23);
WiFiClient wifiClient;
ThingsBoard tb(wifiClient);


// declare here
float temp;
float hum;
float lux;



void setup() {
 Serial.begin(9600);
  Wire.begin();
  dht.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  Serial.println("Booting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
   if (tb.connect(THINGSBOARD_SERVER, THINGSBOARD_ACCESS_TOKEN))
    Serial.println("Connected to ThingsBoard");
  else
    Serial.println("Error connected to ThingsBoard");

}

void loop() {
  if (tb.connected()) {
    Serial.println("Sending data...");
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    lux = lightMeter.readLightLevel();
    tb.sendTelemetryFloat("temperature", temp);
    tb.sendTelemetryFloat("humidity", hum);
    tb.sendTelemetryFloat("light", lux);
    Serial.println("Data sent");
    Serial.printf("Humidity: %.2f, Temperature: %.2f, Light: %.2f \n",
       hum, temp, lux);
  }
 else if (!tb.connected()) {
    Serial.println("Connecting to ThingsBoard node...");
    tb.connect(THINGSBOARD_SERVER, THINGSBOARD_ACCESS_TOKEN);
    Serial.println("Sending data...");
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    lux = lightMeter.readLightLevel();
    tb.sendTelemetryFloat("temperature", temp);
    tb.sendTelemetryFloat("humidity", hum);
    tb.sendTelemetryFloat("light", lux);
    Serial.println("Data sent");
    Serial.printf("Humidity: %.2f, Temperature: %.2f, Light: %.2f \n",
       hum, temp, lux);
       
  }
    delay(6000);
    tb.loop();
  }