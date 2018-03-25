#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include "Musica.h"

#define pinParlante D5
#define pinBoton D0

const char* ssid = "wifi";
const char* password = "clave";
const char* mqtt_server = "server_broker";
const char* keyDevice = "TIMBRE1CASA";

String linkNotificacion = "link_a_php";

Musica musica(pinParlante);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    
  pinMode(pinParlante,OUTPUT);
  pinMode(pinBoton,INPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);    
  }
  musica.tono();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {

  while (!client.connected()) {

    if (client.connect(keyDevice,"usuario","clave")) {
      Serial.println("connected");
      client.subscribe("TIMBRE");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  
  
  // put your main code here, to run repeatedly:
  if(digitalRead(pinBoton) == HIGH) {
    client.publish(keyDevice,"KNOK");
    enviarDatos(linkNotificacion);
    musica.reproducir();
  } else {
    digitalWrite(pinParlante,LOW);
  }
  //delay(150);
}
HTTPClient http;
void enviarDatos(String alink) { 
  http.begin(alink);
  int httpCode = http.GET();
  if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);     
  }  
  http.end();
}
