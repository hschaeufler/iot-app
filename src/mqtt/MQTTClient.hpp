#include <PubSubClient.h>
#include <secrets.h>

#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP
#define OFF "offline"
#define ON "online"

class MQTTClient
{
private:
    PubSubClient mqttClient;
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

public:
    MQTTClient();
    ~MQTTClient();
    template <typename _Any>
    void publishSensor(const char *sensorName, const char *topicName, _Any value);
    void publishFullSensor(const char *topicName, const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water);
    void loop();
};

#endif