/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "********";
const char* password = "********";
const char* mqtt_server = "183.230.40.39";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {    
    Serial.print((char)payload[i]);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect("587608987","324919","oik2iHkmAztRNCTDU3KTYZAQ")) {
      // Once connected, publish an announcement...
      client.subscribe("go");
      client.subscribe("back");
      client.subscribe("left");
      client.subscribe("right");
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 6002);
  client.connect("587608987","324919","oik2iHkmAztRNCTDU3KTYZAQ");
  client.subscribe("go");
  client.subscribe("back");
  client.subscribe("left");
  client.subscribe("right");
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  }
