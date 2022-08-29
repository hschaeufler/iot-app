#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define OFF "offline"
#define ON "online"

/* WiFi-Data */
const char *ssid = "";
const char *password = "";
WiFiMulti wifiMulti;

/* MQTT-Data */
const char *MQTTSERVER = "";
int MQTTPORT = 1883;
const char *mqttuser = "";
const char *mqttpasswd = "";
const char *mqttdevice = "MyNameIsUnqie12223333334"; // Please use a unique name here!
const char *outTopic = "dasIsMeinTestTopic";

/* Last will */
const char *willTopic = "firealarm/status";
const int willQoS = 2;
const boolean willRetain = true;
const char *willMessage = OFF;

const char *statusMessage = ON;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

/* DHT11-Data (Connect to GPIO22 on ESP32) */
#define DHTTYPE DHT11     // may be DHT11 or DHT22
uint8_t DHTPin = 22;      // DHT11-Sensor connected to Pin 22
DHT dht(DHTPin, DHTTYPE); // Construct DHT Object for gathering data
float Temperature;
float Humidity;

/* JSON-Document-Size for incoming JSON (object size may be increased for larger JSON files) */
const int capacity = JSON_OBJECT_SIZE(6);

/* Outgoing JSON Documents */
DynamicJsonDocument doc(capacity);

#define MSG_BUFFER_SIZE (256) // Define the message buffer max size
char msg[MSG_BUFFER_SIZE];    // Define the message buffer

/**
 * This function is called when a MQTT-Message arrives.
 */
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{ // callback includes topic and payload ( from which (topic) the payload is comming)
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");

  // Create a JSON document on call stack
  StaticJsonDocument<capacity> doc;
  String jsonInput = String((char *)payload);

  // try to deserialize JSON
  DeserializationError err = deserializeJson(doc, jsonInput);

  // if an error occurs, print it out
  if (err)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
    return;
  }

  // Read out a name from JSON content (assuming JSON doc: {"SensorType" : "SomeSensorType", "value" : 42})
  const char *name = doc["SensorType"];
  long value = doc["value"];
  Serial.println(name);
  Serial.println(value);
}

/**
 * This function is called from setup() and establishes a MQTT connection.
 */
void initMqtt()
{
  client.setServer(MQTTSERVER, MQTTPORT);

  // Set the callback-Function when new messages are received.
  client.setCallback(mqtt_callback);

  // connect and last will
  client.connect(mqttdevice, mqttuser, mqttpasswd, willTopic, willQoS, willRetain, willMessage);
  while (!client.connected())
  {
    Serial.print(".");
    delay(500);
  }

  // Publish Status
  client.publish(willTopic, statusMessage, true);

  // subscribe to a certain topic
  client.subscribe("myExampleTopicToSubscribeTo");
}

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

  // Connect to MQTT server
  initMqtt();

  // Start DHT stuff
  pinMode(DHTPin, INPUT); // Set DHT-Pin to INPUT-Mode (so we can read data from it)
  dht.begin();

  // Print to console
  Serial.println("Setup completed.");
  delay(2000);
}

/**
 * This function is called prior to sending data to mqtt.
 * The JSON document gets cleared first (to free memory and
 * avoid memory leaks), then sensor name, timestamp and
 * measured values (humidity and temperature) are set to
 * the JSON document.
 */
void setJSONData(float humidity, float temp)
{
  doc.clear();
  doc["sensor"] = "DHT11";
  doc["humidity"] = humidity;
  doc["temperature"] = temp;
}

/**
 * This function is the main function and loops forever.
 */
void loop()
{
  // loop the mqtt client so it can maintain its connection and send and receive messages
  client.loop();

  // receive measured values from DHT11
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity();       // Gets the values of the humidity

  // set measured data to preprared JSON document
  setJSONData(Humidity, Temperature);

  // serialize JSON document to a string representation
  serializeJsonPretty(doc, msg);

  // publish to MQTT broker
  client.publish(outTopic, msg);
  client.loop();

  // wait a second before the next loop.
  delay(1000);
}