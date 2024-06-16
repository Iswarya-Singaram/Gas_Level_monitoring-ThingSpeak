#include <ESP8266WiFi.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* thingspeakServer = "api.thingspeak.com";
const int thingspeakAPIKey = YOUR_THINGSPEAK_API_KEY;
const int thingspeakChannelID = YOUR_THINGSPEAK_CHANNEL_ID;

const int gasSensorPin = A0; // Adjust based on your sensor's analog pin connection

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read analog value from gas sensor
  int sensorValue = analogRead(gasSensorPin);

  // Convert sensor value to a meaningful unit (adjust based on sensor datasheet)
  // This example assumes a linear relationship between sensor value and gas concentration (replace with your sensor's conversion method)
  float gasConcentration = sensorValue * 5.0 / 1023.0; // Assuming 0-5V sensor output

  // Prepare HTTP request to Thingspeak
  String dataString = "api_key=" + String(thingspeakAPIKey) + "&field1=" + String(gasConcentration);

  Serial.print("Connecting to Thingspeak...");
  if (connectToThingspeak()) {
    Serial.println("Connected!");
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: "  thingspeakServer "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Connection: close\n");
    client.print("Content-Length: ");
    client.print(dataString.length());
    client.println("\n");
    client.println(dataString);
    delay(1000); // Wait for Thingspeak to process data
  } else {
    Serial.println("Connection failed!");
  }
  client.stop();

  delay(10000); // Adjust delay between readings as needed
}

boolean connectToThingspeak() {
  if (!client.connected()) {
    Serial.print("Connecting to server: ");
    Serial.println(thingspeakServer);
    if (client.connect(thingspeakServer, 80)) {
      return true;
    } else {
      return false;
    }
  }
  return true;
}


