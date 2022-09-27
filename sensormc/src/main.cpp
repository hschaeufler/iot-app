#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <config/secrets.h>
#include <config/wlan.h>
#include <sensors/digitalSensor.hpp>
#include <sensors/fireSensor.hpp>
#include <sensors/smokeSensor.hpp>
#include <sensors/waterSensor.hpp>
#include <actuators/Buzzer.hpp>
#include <actuators/TrafficLight.hpp>
#include <PubSubClient.h>

#define REDPIN 16
#define YELLOWPIN 17
#define GREENPIN 18
#define DHTPIN 23
#define FIREPIN 0
#define SMOKEPIN 2
#define WATERPIN 32
#define BUZZERPIN 12

#define OFF "offline"
#define ON "online"

#define SENSORNAME "homeprotect"

#define DHTTYPE DHT11

#define WATERLIMIT 50

#define MSG_BUFFER_SIZE (256) // Define the message buffer max size

// Initialize Sensors
FireSensor fireSensor(FIREPIN);
SmokeSensor smokeSensor(SMOKEPIN);
WaterSensor waterSensor(WATERPIN);
DHT dht(DHTPIN, DHTTYPE);

// Initalize Actuators
Buzzer buzzer(BUZZERPIN);
TrafficLight trafficLight(REDPIN, YELLOWPIN, GREENPIN);

/* MQTT-Data */
const char *mqttdevice = SENSORNAME;
const char *outTopic = SENSORNAME;

/* MQTT-Last will */
const char *willTopic = "homeprotect/status";
const int willQoS = 2;
const boolean willRetain = true;
const char *willMessage = OFF;

const char *statusMessage = ON;

/* WiFi-Data */
const char *ssid = WIFI_SSID;
const char *password = WIFI_PW;

WiFiMulti wifiMulti;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

/* JSON-Document-Size for outgoing JSON (object size may be increased for larger JSON files) */
const int capacity = JSON_OBJECT_SIZE(10);
DynamicJsonDocument doc(capacity);
char msg[MSG_BUFFER_SIZE]; // Define the message buffer

/**
 * This function is called from setup() and establishes a WLAN connection
 */

void initWifi()
{

  if (!USE_DHCP)
  {
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress primaryDNS;
    IPAddress secondaryDNS;

    local_IP.fromString(LOCAL_IP);
    gateway.fromString(GATEWAY_IP);
    subnet.fromString(SUBNET_ADDR);
    primaryDNS.fromString(PRIMARY_DNS);
    secondaryDNS.fromString(SECONDARY_DNS);

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
      Serial.println("STA Failed to configure");
    }
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
  client.setServer(MQTT_SERVER, MQTT_PORT);

  // connect and last will
  client.connect(mqttdevice, MQTT_USER, MQTT_PW, willTopic, willQoS, willRetain, willMessage);
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

  // Init TrafficLight
  trafficLight.setYellow();

  // Connect to WLAN
  initWifi();

  // Init MQTT
  initMqtt();

  // Start DHT stuff
  pinMode(DHTPIN, INPUT);
  dht.begin();

  // set TraffigLight to Green
  trafficLight.setGreen();

  // Print to console
  Serial.println("Setup completed.");
  delay(1000);
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

  Serial.println("Read Sensors");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  bool isFire = fireSensor.isFire();
  bool isSmokey = smokeSensor.isSmokey();
  uint16_t water = waterSensor.detectWater();
  int limt = WATERLIMIT;
  bool isWater = waterSensor.isWater(limt);

  bool isAlarm = isFire || isWater || isSmokey;

  if (isAlarm)
  {
    trafficLight.setRed();
    buzzer.startBeep();
  }
  else
  {
    trafficLight.setGreen();
    buzzer.stopBeep();
  }

  Serial.println("Build JSON");
  setJSONData(SENSORNAME, humidity, temperature, isFire, isSmokey, isWater, water);

  // serialize JSON document to a string representation
  serializeJsonPretty(doc, msg);
  Serial.println(msg);

  Serial.println("Publish Message");
  // publish to MQTT broker
  client.publish(outTopic, msg);
  Serial.println("Going to sleep!");

  delay(1000);
}