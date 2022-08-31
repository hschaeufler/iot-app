#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <secrets.h>
#include <sensors/digitalSensor.hpp>
#include <sensors/fireSensor.hpp>
#include <sensors/smokeSensor.hpp>
#include <sensors/waterSensor.hpp>
#include <PubSubClient.h>

#define OFF "offline"
#define ON "online"

/* MQTT-Data */
const char *MQTTSERVER = MQTT_SERVER;
int MQTTPORT = 1883;
const char *mqttuser = MQTT_USER;
const char *mqttpasswd = MQTT_PW;
const char *mqttdevice = "homeprotect"; // Please use a unique name here!
const char *outTopic = "homeprotect";

/* Last will */
const char *willTopic = "homeprotect/status";
const int willQoS = 2;
const boolean willRetain = true;
const char *willMessage = OFF;

const char *statusMessage = ON;

/* WiFi-Data */
const char *ssid = WIFI_SSID;
const char *password = WIFI_PW;
WiFiMulti wifiMulti;

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

WiFiClient wifiClient;
PubSubClient client(wifiClient);

/* JSON-Document-Size for outgoing JSON (object size may be increased for larger JSON files) */
const int capacity = JSON_OBJECT_SIZE(10);
DynamicJsonDocument doc(capacity);

#define MSG_BUFFER_SIZE (256) // Define the message buffer max size
char msg[MSG_BUFFER_SIZE];    // Define the message buffer

/**
 * This function is called from setup() and establishes a WLAN connection
 */

void initWifi()
{

  IPAddress local_IP(141, 72, 16, 245);
  IPAddress gateway(141, 72, 16, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);
  IPAddress secondaryDNS(8, 8, 4, 4);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

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

void initMqtt()
{
  client.setServer(MQTTSERVER, MQTTPORT);

  // connect and last will
  client.connect(mqttdevice, mqttuser, mqttpasswd, willTopic, willQoS, willRetain, willMessage);
  while (!client.connected())
  {
    Serial.print(".");
    delay(500);
  }

  // Publish Status
  client.publish(willTopic, statusMessage, true);
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

  // Init MQTT
  initMqtt();

  // Start DHT stuff
  pinMode(DHTPin, INPUT); // Set DHT-Pin to INPUT-Mode (so we can read data from it)
  dht.begin();

  // Print to console
  Serial.println("Setup completed.");
  delay(2000);
}

void setJSONData(const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water)
{
  doc.clear();
  doc["sensor"] = sensorName;
  doc["humidity"] = humidity;
  doc["temperature"] = temp;
  doc["isFire"] = isFire;
  doc["isSmoke"] = isSmoke;
  doc["water"] = water;
  doc["isWater"] = isWater;
}

/**
 * This function is the main function and loops forever.
 */
void loop()
{
  Serial.println("-----------------------------------------------");
  Serial.println("Loop started");
  // mqttClient.loop();

  Serial.println("Init Vars");
  char const *sensorName;
  char const *topicName;
  char const *baseTopic = "homeprotect";

  Serial.println("Read Sensors");

  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  bool isFire = fireSensor.isFire();
  bool isSmokey = smokeSensor.isSmokey();
  uint16_t water = waterSensor.detectWater();
  int limt = WATERLIMIT;
  bool isWater = waterSensor.isWater(limt);

  Serial.println("Build JSON");
  sensorName = "homeprotect";
  setJSONData(sensorName, Humidity, Temperature, isFire, isSmokey, isWater, water);

  // serialize JSON document to a string representation
  serializeJsonPretty(doc, msg);
  Serial.println(msg);

  Serial.println("Publish Message");
  // publish to MQTT broker
  client.publish(outTopic, msg);
  Serial.println("Going to sleep!");
  delay(2000);
}