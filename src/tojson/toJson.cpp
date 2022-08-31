#include <tojson/toJson.hpp>
#include <ArduinoJson.h>

template <typename _any>
void toFullJSON(_any buf, const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water)
{
    const int capacity = JSON_OBJECT_SIZE(10);
    DynamicJsonDocument doc(capacity);
    doc.clear();
    doc["sensor"] = "DHT11";
    doc["humidity"] = humidity;
    doc["temperature"] = temp;
    doc["isFire"] = isFire;
    doc["isSmoke"] = isSmoke;
    doc["water"] = water;
    doc["isWater"] = isWater;
    serializeJson(doc, buf);
}
