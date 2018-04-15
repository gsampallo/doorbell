#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include "Musica.h"
//#include "Configuracion.h"

#define pinParlante D5
#define pinBoton D0

const char* ssid = "wifi";
const char* password = "clave";
const char* mqtt_server = "mqtt_server";
const char* keyDevice = "TIMBRE";

String linkNotificacion = "update.php?key=";

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
  if (((char)payload[0] == '1')) {
    musica.reproducir();
  }
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

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  
  
  int reading = digitalRead(pinBoton);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        
        client.publish(keyDevice,"KNOK");
        enviarDatos(linkNotificacion);
        musica.reproducir();
        //tiempoTranscurrido = 0;        
      } else {
        digitalWrite(pinParlante,LOW);
      }


    }    
  }
  lastButtonState = reading;

  //delay(10);
  
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
