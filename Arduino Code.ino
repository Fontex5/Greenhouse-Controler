#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"


#define DHTPIN 13
#define DHTTYPE DHT11

//Enter the credentials of your WiFi
const char* ssid = "";     
const char* password = "";

//Enter the address for the MQTT Server
//You can create a local server by using Mosquitto 
const char* mqtt_server = "";

WiFiClient esp8266device;
PubSubClient client(esp8266device);
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

//When new data is received from MQTT 
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messagein;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messagein += (char)message[i];
  }

  if(topic=="fan/status"){
    if (messagein == "ON"){ 
      Serial.println("\nFan ON");
      digitalWrite(4, LOW);          
    }
   if(messagein == "OFF"){
      Serial.println("\nFan OFF");
      digitalWrite(4, HIGH);
   }
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("device1")) {

      Serial.println("connected");  
      client.subscribe("fan/status");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
   randomSeed(analogRead(0));
  pinMode(4 , OUTPUT);
  digitalWrite(4,HIGH);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("device1");
   // int temp=random(0, 100);
       char temp [8];
       char hum[8];
 
  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C\n ");
  dtostrf(t,6,2,temp);
  dtostrf(h , 6 , 2 ,hum);         
   client.publish("room/temp",temp);
   client.publish("room/hum" ,hum );
} 
