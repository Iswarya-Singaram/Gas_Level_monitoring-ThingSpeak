# Gas_Level_Monitoring-ThingSpeak
Welcome to the Gas Level Monitoring System project! This system leverages the MQ2 gas sensor and ESP8266 to monitor gas levels, with data visualization available on the ThingSpeak cloud platform. The data from the gas sensor is sent wirelessly to ThingSpeak using the ESP8266, enabling real-time monitoring and analysis of gas levels.
<p align="center">
  <img src="https://github.com/Iswarya-Singaram/Gas_Level_monitoring-ThingSpeak/assets/145309713/f3fea09f-d46a-4870-8017-b703d0407fd5" width="500" height="300">
</p>

## Overview
This project is designed to provide an easy and efficient way to monitor gas levels in various environments. By using the MQ2 gas sensor, we can detect the presence of gases such as LPG, smoke, alcohol, propane, hydrogen, methane, and carbon monoxide. The ESP8266 Wi-Fi module connects the sensor to the internet, allowing the data to be sent to the ThingSpeak cloud platform where it can be observed and analyzed.

<p align="center">
  <img src="https://github.com/Iswarya-Singaram/Gas_Level_monitoring-ThingSpeak/assets/145309713/f158c4ff-e158-43f1-ae23-816d6fd92279" width="500" height="300">
</p>

## Features
<ul>
<li>Real-Time Monitoring: Continuously monitors gas levels and provides real-time data.</li>
<li>Wireless Data Transmission: Sends sensor data to the ThingSpeak cloud platform via Wi-Fi.</li>
<li>Cloud Visualization: Visualize and analyze gas sensor data on ThingSpeak.</li>
</ul>

## Components
<ul>
<li>MQ2 Gas Sensor: Detects various gases and provides an analog signal output.</li>
<li>ESP8266 Wi-Fi Module: Connects to Wi-Fi and sends data to the cloud.</li>
<li>ThingSpeak Cloud Platform: Receives and visualizes sensor data.</li>
</ul>

## Getting Started
To get started with this project, follow the instructions below:

## Hardware Setup
### Connect the MQ2 gas sensor to the ESP8266:
<li>VCC to 3.3V</li>
<li>GND to GND</li>
<li>A0 to A0 (analog pin on ESP8266)</li>

## Software Setup
#### 1.Download the latest Arduino IDE from the official website
~~~
https://www.arduino.cc/en/software
~~~
After installing arduino IDE
#### 2.Configure the ESP8266 Board in Arduino IDE:
<ol>
<li>Open Arduino IDE</li>
<li>Go to File > Preferences.</li>
  <p align="center">
  <img src="https://github.com/Iswarya-Singaram/Gas_Level_monitoring-ThingSpeak/assets/145309713/cb45563f-407c-423e-8b28-cb29bd277469" width="500" height="300">
</p>
<li>In the Additional Board Manager URLs field, add the following URL:</li>
  
~~~
http://arduino.esp8266.com/stable/package_esp8266com_index.json
~~~
<li>Go to Tools > Board > Boards Manager.</li>
<li>Search for esp8266 and install the ESP8266 package.</li>
</ol>
Note: In the Arduino IDE, Board Manager URLs are used to specify the location of additional hardware definitions for boards that are not included by default in the IDE. These URLs point to JSON files that contain metadata and links to the necessary files for supporting additional microcontroller boards. 

#### 3.Setting up ThingSpeak 
1.Create a ThingSpeak account at 
~~~
https://thingspeak.com
~~~
2. Create a new channel and add a field for the gas sensor data.

<p align="center">
  <img src="https://github.com/Iswarya-Singaram/Gas_Level_monitoring-ThingSpeak/assets/145309713/b327a286-c1c1-4798-b845-310f3df9cc93" width="500" height="300">
</p>
3. Note the Write API Key from the API Keys tab.
<p align="center">
  <img src="https://github.com/Iswarya-Singaram/Gas_Level_monitoring-ThingSpeak/assets/145309713/05864e8a-ed51-4873-8bb4-289e9dff7ca2" width="500" height="300">
</p>
4. Update your Arduino sketch with the API Key in the following source code.

~~~
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
~~~
#### 4.Upload the code to your ESP8266:
1.Select Tools > Board > NodeMCU 1.0 (ESP-12E Module).</br>
<p align="center">
  <img src="https://github.com/Iswarya-Singaram/Gas_Level_monitoring-ThingSpeak/assets/145309713/faa7ea3b-9ae2-4aae-8805-013fbb4d285e" width="500" height="300">
</p>
2.Select the appropriate Port.</br>
3.Click the Upload button.</br>

Once the setup is complete, the ESP8266 will connect to your Wi-Fi network and start sending gas sensor data to ThingSpeak. You can visualize the data on your ThingSpeak channel.
## Contributing
Contributions are welcome! If you have any ideas for improvements or encounter any issues, feel free to open an issue or submit a pull request.


