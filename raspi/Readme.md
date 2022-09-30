# Step 1: Install Raspberry Pi OS
- Download [Rasperry Pi Imager](https://www.raspberrypi.com/software/) for your OS
- insert SD card into Computer
- Start Raspberry Pi Imager
- Select Raspberry Pi OS Lite as operating system
- go to settings and enable SSH, provide username and password if desired
- write OS to SD card

# Step 2: Install docker
- follow installation steps from [Docker documentation to install Docker on Debian](https://docs.docker.com/engine/install/debian/)
- execute [Post-installation steps for Linux](https://docs.docker.com/engine/install/linux-postinstall/)

# Step 3: Start portainer docker container
- start portainer docker container as decribed in official [documentation](https://docs.portainer.io/v/ce-2.9/start/install/server/docker/linux)
  - use a file system mapping instead of a docker volume if desired

# Step 4: Deploy Stack
- create folders
  - /home/pi/nodered-data
  - /home/pi/mosquitto-data/config
- place [mosquitto_conf.txt](https://github.com/hschaeufler/iot-app/blob/master/raspi/mosquitto_conf.txt) in folder /home/pi/mosquitto-data/config
- open portainer webinterface (accessible under <your-raspberry-pi-ip>:8000)
- open stack overview in portainer webinterface
- create new stack
- copy file contents from [docker-compose.yml](https://github.com/hschaeufler/iot-app/blob/master/raspi/docker-compose.yml) or import file directly
- start stack

# Step 5: Import Node-Red flow
- open Node-Red webinterface (accessible under <your-raspberry-pi-ip>:1880)
- import [flow.json](https://github.com/hschaeufler/iot-app/blob/master/raspi/flow.json) (copy file content or upload file)
  - telegram Bot should be preconfigured and working

# Step 6: use Telegram Bot
- install Telegram on your Smartphone/Tablet/Computer/...
- search for bot IoT_123456_bot
- use commands to operate bot
  - /status - to get all current sensor values
  - /start - to subscribe to warnings
  - /stop - to unsubscribe from warning