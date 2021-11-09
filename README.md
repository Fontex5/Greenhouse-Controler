# Greenhouse Controler
## About the project
This project is a small simulation of a system for controlling  a greenhouse remotely. This means the system will provide the temperature and humidity of the environment and allow turning a fan on or off. The system is consisted of two parts. First part is the code for an Arduino WeMos D1 to sense the temperature and humidity and control the fan, and the second part is an Android application to control the board and show the temperature.

## How the project works
The main part of this project is about the connection between the board and the app which is implemented using MQTT Protocol. Temperature and humidity is sensed by a DHT-11 which is connected to pin 13. When the board starts working, it connects to an MQTT server and subscribes on "fan/status". After sensing the temperature and humidity, the board publishes them with "room/temp" and "room/hum" topics.
The Android app connects to the MQTT server as well and subscribes on "room/temp" and "room/hum". Whenever new data receives, it will be shown on the app. Moreover, there is a switch which is for turning the fan on or off. The app will send the current state of the switch to the board by using "fan/status" as the topic.

### Pay attention
Because the focus of this project was on the connection of the board and the application and not the appareance of the app, only the Java code for the main part of the app is provided.
