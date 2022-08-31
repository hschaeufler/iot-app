#include <tojson/toJson.hpp>
#include <ArduinoJson.h>

template <typename _Any>
String toJSON(const char *sensorName, _Any value)
{
    const int capacity = JSON_OBJECT_SIZE(4);
    String jsonString;
    DynamicJsonDocument doc(capacity);
    doc.clear();
    doc["sensor"] = sensorName;
    doc["value"] = value;
    serializeJson(doc, jsonString);
    return jsonString;
}

String toFullJSON(const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water)
{
    const int capacity = JSON_OBJECT_SIZE(10);
    String jsonString;
    DynamicJsonDocument doc(capacity);
    doc.clear();
    doc["sensor"] = "DHT11";
    doc["humidity"] = humidity;
    doc["temperature"] = temp;
    doc["isFire"] = isFire;
    doc["isSmoke"] = isSmoke;
    doc["water"] = water;
    doc["isWater"] = isWater;
    serializeJson(doc, jsonString);
    return jsonString;
}

template String toJSON<float>(const char *sensorName, float value);
template String toJSON<bool>(const char *sensorName, bool value);
template String toJSON<unsigned short>(const char *sensorName, unsigned short value);
