#include <ArduinoJson.h>

#ifndef TOJSON_HPP
#define TOJSON_HPP

template <typename _any>
void toFullJSON(_any buf, const char *sensorName, float humidity, float temp, bool isFire, bool isSmoke, bool isWater, uint16_t water);

#endif