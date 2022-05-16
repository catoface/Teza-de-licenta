/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#ifndef STASSID
#define STASSID "MOLDTELECOM_E20"
#define STAPSK  "3122294100623"
#endif
#define DHTPIN 2
#define DHTTYPE    DHT11

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.6";
const uint16_t port = 5000;

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(115200);
  delay(5000);
  dht.begin();
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000; // Set delay between sensor readings based on sensor details.

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  sensors_event_t event_t;
  sensors_event_t event_h;
  
  dht.temperature().getEvent(&event_t);
  if (isnan(event_t.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event_t.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event_h);
  if (isnan(event_h.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event_h.relative_humidity);
    Serial.println(F("%"));
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.print(String("POST ") + "/?temperature=" + event_t.temperature + "&humidity=" + event_h.relative_humidity +
    " HTTP/1.1\r\n" +
     "Host: " + host + "\r\n" +
     "User-Agent: BuildFailureDetectorESP8266\r\n" +
     "Connection: close\r\n\r\n");
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
//  Serial.println();
//  Serial.println("closing connection");
//  client.stop();

  delay(1000);
}
