#include <mqtt/MQTTClient.hpp>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiMulti.h>
#include <tojson/toJson.hpp>

MQTTClient::MQTTClient()
{
    WiFiClient wifiClient;
    PubSubClient client(wifiClient);

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
    this->pubSubClient = client;
}

MQTTClient::~MQTTClient()
{
}

template <typename _Any>
void MQTTClient::publishSensor(const char *sensorName, const char *topicName, _Any value)
{
    Serial.println("tojson");
    String jsonMessage = toJSON(sensorName, value);
    Serial.println("mqttClient.publish");
    pubSubClient.publish(topicName, jsonMessage.c_str());
    Serial.println("done");
}

void MQTTClient::publishFullSensor(const char *topicName, const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water)
{
    String jsonMessage = toFullJSON(sensorName, humidity, temp, isFire, isSmoke, isWater, water);
    pubSubClient.publish(topicName, jsonMessage.c_str());
}

void MQTTClient::loop()
{
    pubSubClient.loop();
}

template void MQTTClient::publishSensor<float>(const char *sensorName, const char *topicName, float value);
template void MQTTClient::publishSensor<bool>(const char *sensorName, const char *topicName, bool value);
template void MQTTClient::publishSensor<unsigned short>(const char *sensorName, const char *topicName, unsigned short value);
