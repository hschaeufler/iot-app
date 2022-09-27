# HomeProtect
The source code published here is part of the IOT system HomeProtect. 
## Use-Case Description
The source code published here is part of the IOT system HomeProtect. HomeProtect is designed to detect environmental hazards and protect life and property. HomeProtect can detect fires, smoke, gas leaks, floods and extreme temperatures. The sensor data is shown to the user via a mobile display. In case of imminent danger, such as a fire, the user receives a warning via a Telegram chatbot. 
## Architektur
The system is made up of three subsystems:
- **Sensor subsystem**: Consists of an ESP32 and several sensors (smoke, flame, water level and DHT11 sensor) and actuators (buzzer and traffic light module). Detects and reports hazardous situations via an MQTT topic.
- **Display subsystem**: Consists of an ESP32 and an I2C LCD display. Reads the values from the MQTT topic and shows them on the display.
- **Application Layer**: The Eclipse Mosquitto MQTT broker and Node-RED are run in Docker containers on a Raspberry PI. The MQTT topic is read out via a Node-RED flow and a Telegrambot is controlled.

![Aufbau des Prototyps](https://raw.githubusercontent.com/hschaeufler/iot-app/master/report/Uebersicht_1.jpg)