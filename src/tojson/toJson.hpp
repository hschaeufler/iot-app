#include <ArduinoJson.h>

#ifndef TOJSON_HPP
#define TOJSON_HPP

template <typename _Any>
String toJSON(const char *sensorName, _Any value);
String toFullJSON(const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water);

#endif