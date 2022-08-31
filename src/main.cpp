#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <secrets.h>
#include <sensors/digitalSensor.hpp>
#include <sensors/fireSensor.hpp>
#include <sensors/smokeSensor.hpp>
#include <sensors/waterSensor.hpp>
#include <tojson/toJson.hpp>
#include <mqtt/MQTTClient.hpp>

#define OFF "offline"
#define ON "online"

/* WiFi-Data */
const char *ssid = WIFI_SSID;
const char *password = WIFI_PW;
WiFiMulti wifiMulti;

MQTTClient mqttClient;

/* DHT11-Data (Connect to GPIO22 on ESP32) */
#define DHTTYPE DHT11     // may be DHT11 or DHT22
uint8_t DHTPin = 23;      // DHT11-Sensor connected to Pin 22
DHT dht(DHTPin, DHTTYPE); // Construct DHT Object for gathering data
float Temperature;
float Humidity;

#define WATERLIMIT 50;
int firePin = 0;
int smokePin = 2;
uint8_t waterPin = 32;
FireSensor fireSensor(firePin);
SmokeSensor smokeSensor(smokePin);
WaterSensor waterSensor(waterPin);

#define MSG_BUFFER_SIZE (256) // Define the message buffer max size

/**
 * This function is called from setup() and establishes a WLAN connection
 */

void initWifi()
{
  Serial.println("Connecting to WiFi ...");
  wifiMulti.addAP(ssid, password);

  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * This function is called when the ESP is powered on.
 */
void setup()
{
  // Set serial port speed to 115200 Baud
  Serial.begin(115200);

  // Connect to WLAN
  initWifi();

  // Start DHT stuff
  pinMode(DHTPin, INPUT); // Set DHT-Pin to INPUT-Mode (so we can read data from it)
  dht.begin();

  // Print to console
  Serial.println("Setup completed.");
  delay(2000);
}

/**
 * This function is the main function and loops forever.
 */
void loop()
{
  Serial.println("Loop started!");
  mqttClient.loop();

  Serial.println("Init Vars!");
  char const *sensorName;
  char const *topicName;
  char const *baseTopic = "homeprotect";

  Serial.println("rEad Temp!");
  // receive measured values from DHT11
  Temperature = dht.readTemperature();
  sensorName = "temp";
  topicName = "homeprotect/temp";
  Serial.println("Publish Temp!");
  mqttClient.publishSensor(sensorName, topicName, Temperature);

  Serial.println("read humidty!");
  Humidity = dht.readHumidity();
  sensorName = "humidty";
  topicName = "homeprotect/humidity";
  Serial.println("Publish humidity!");
  mqttClient.publishSensor(sensorName, baseTopic, Humidity);

  Serial.println("read fire!");
  bool isFire = fireSensor.isFire();
  sensorName = "fire";
  topicName = "homeprotect/fire";
  mqttClient.publishSensor(sensorName, baseTopic, isFire);

  Serial.println("read smoke!");
  bool isSmokey = smokeSensor.isSmokey();
  sensorName = "smoke";
  topicName = "homeprotect/smoke";
  mqttClient.publishSensor(sensorName, baseTopic, isSmokey);

  Serial.println("read water!");
  uint16_t water = waterSensor.detectWater();
  sensorName = "water";
  topicName = "homeprotect/water";
  mqttClient.publishSensor(sensorName, baseTopic, water);

  Serial.println("read isWater!");
  int limt = WATERLIMIT;
  bool isWater = waterSensor.isWater(limt);
  sensorName = "iswater";
  topicName = "homeprotect/iswater";
  mqttClient.publishSensor(sensorName, baseTopic, isWater);

  Serial.println("publish full!");
  sensorName = "homeprotect";
  mqttClient.publishFullSensor(sensorName, sensorName, Humidity, Temperature, isFire, isSmokey, isWater, water);

  Serial.println("loop!!");
  mqttClient.loop();

  // wait a second before the next loop.
  delay(1000);
}