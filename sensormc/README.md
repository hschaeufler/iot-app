# Sensor Subsystem
The subsystem for recording the sensor values consists of a ESP32 microcontroller, a corresponding development board (ESP32-DevKit C V2) and  and a flame sensor, a gas, a smoke sensor and a water level sensor. In addition, a DHT-11 sensor records the room temperature and humidity. A traffic light module and an active buzzer are used as actuators.
## Getting Started
To test and develop the source code, it is recommended to use the IDE [PlatformIO](https://platformio.org/). This also takes care of the dependency management. Once the IDE is installed, simply clone the repository and import the project.
## Wiring
Several sensors and actuators were used.  These are from the 16 in 1 sensor kit from AZ-Delivery and the ELEGOO Upgraded 37 in 1 Sensor Modules Kit. The following figure shows the wiring.

![alt text](https://github.com/hschaeufler/iot-app/blob/master/report/SensorErfassung_SP.PNG?raw=true)

The following place in the main.cpp can be used to adjust the used ports.
```
#define REDPIN 16
#define YELLOWPIN 17
#define GREENPIN 18
#define DHTPIN 23
#define FIREPIN 0
#define SMOKEPIN 2
#define WATERPIN 32
#define BUZZERPIN 12
```
