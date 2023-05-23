#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <HTTPClient.h>
#include "FS.h"
#include "WebSocketsServer.h"
#include "WebServer.h"
#include <ArduinoJson.h>

const char* ssid = "LG";
const char* password = "11111111";

const char* serverAddress = "192.168.115.195";
const int serverPort = 8080;

int portNumber = 8811;
WebSocketsClient webSocket;

String dataString;

WebServer server(80);

WebSocketsServer webSocketServer = WebSocketsServer(81);

const int fotoupornikPin = 34;
int fotoupornikValue = 0;
const int BUTTON_PIN = 18;
int trenutnoStanje = 0;
uint8_t LED1pin = 2;
uint8_t LED2pin = 4;
uint8_t LED3pin = 22;
uint8_t LED4pin = 5;
uint8_t LED5pin = 23;
bool LED1status = LOW;
void sequenceOnOff() {
  // Handle sequenceOnOff functionality here
}

void handleRoot() {}


void turnOffAllLEDs() {
  digitalWrite(LED1pin, LOW);
  digitalWrite(LED2pin, LOW);
  digitalWrite(LED3pin, LOW);
  digitalWrite(LED4pin, LOW);
  digitalWrite(LED5pin, LOW);
}

void onWebSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) 
  {
  case WStype_TEXT: 
  {
      StaticJsonDocument<256> doc; // Create a JSON document

      DeserializationError error = deserializeJson(doc, payload, length); // Parse the JSON payload

      if (error) {
        Serial.print(F("deserializeJson() - error: "));
        Serial.println(error.c_str());
        return;
      }
      const char* tip = doc["tipSporočila"];
      int vrednost = doc["vrednost"];
      int pin = doc["pin"];

      if (strcmp(tip, "LED") == 0) {
        if (pin == 2) {
          if (vrednost == 0) {
            digitalWrite(2, LOW);
          } else if (vrednost == 1) {
            digitalWrite(2, HIGH);
          }
        } else if (pin == 4) {
          if (vrednost == 2) {
            digitalWrite(4, HIGH);
          } else if (vrednost == 3) {
            digitalWrite(4, LOW);
          }
        } else if (pin == 5) {
          if (vrednost == 4) {
            digitalWrite(5, HIGH);
          } else if (vrednost == 5) {
            digitalWrite(5, LOW);
          }
        } else if (pin == 22) {
          if (vrednost == 6) {
            digitalWrite(22, HIGH);
          } else if (vrednost == 7) {
            digitalWrite(22, LOW);
          }
        } else if (pin == 23) {
          if (vrednost == 8) {
            digitalWrite(23, HIGH);
          } else if (vrednost == 9) {
            digitalWrite(23, LOW);
          }
        }
         else if (pin == 23||pin==5) {
          if (vrednost == 10) {
            digitalWrite(LED5pin, HIGH);
            digitalWrite(LED4pin, HIGH);
          } else if (vrednost == 11) {
            digitalWrite(LED5pin, LOW);
             digitalWrite(LED4pin, LOW);
          }
      }
    

      } else if (strcmp(tip, "SEQUENCE_ONOFF") == 0) {
        sequenceOnOff();
      } else {
        Serial.println("Neprepoznana vrednost v JSON sporočilu.");
      }
      break;
    }
    
    default:
    break;
  }
}

void setup()
{

  Serial.begin(115200);

  Serial.println("Povezovanje");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z Wifi omrežjem...");
    Serial.print(".");
  }
    
  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov ESP32 modula: ");
  Serial.println(WiFi.localIP());
  Serial.print("Številka port-a: ");
  Serial.println(portNumber);
  Serial.print("WiFi.macAddress: ");
  Serial.println(WiFi.macAddress());

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);

 // del, ki služi kot indikator, da je koda prenešena na modul

  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod

  digitalWrite(2, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(2, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(2, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(2, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne

   pinMode(5, OUTPUT); // nožica št. 2 bo delovala kot izhod

  digitalWrite(5, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(5, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(5, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(5, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  pinMode(23, OUTPUT); // nožica št. 2 bo delovala kot izhod

  digitalWrite(23, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(23, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(23, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(23, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(4, OUTPUT); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(4, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(4, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s
  digitalWrite(4, 1); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750);

  digitalWrite(4, 0); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(22, OUTPUT); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s

  digitalWrite(22, 1); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(750); 
  digitalWrite(22, 0); // na nožici 2 zapišemo visoko vrednost - LED zasveti

  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000ms = 1s
  digitalWrite(22, 1); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(750); 
  digitalWrite(22, 0); // na nožici 2 zapišemo visoko vrednost - LED zasveti



 // zagon WebSocket - klienta; tu vzpostavimo povezavo med esp32 modulom ter node.js strežnikom
  // preko vrat 8811
  webSocket.begin("192.168.115.195", 8811); // priključimo se na node.js strežnik na IP naslovu 192... port 8811
  delay(1000);
  
  webSocket.onEvent(onWebSocketEvent);

  server.on("/", handleRoot);

  server.begin();

}




void loop(){
  webSocket.loop();
  
   int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH) {
    if (trenutnoStanje == 0) {
      digitalWrite(2, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(22, HIGH);
      digitalWrite(23, HIGH);

      trenutnoStanje = 1;
      delay(200);
    } else {
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(22, LOW);
      digitalWrite(23, LOW);
      trenutnoStanje = 0;
      delay(200);
    }
  }
   // spremlja prisotnost klientov in posluša njihove HTML zahteve ("request")

  
  webSocket.sendTXT(dataString);

  delay(50);
}