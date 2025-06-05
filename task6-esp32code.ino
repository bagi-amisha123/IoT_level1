#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// ThingSpeak API configuration
const char* server = "http://api.thingspeak.com/update";
String apiKey = "your_thingspeak_api_key";

// DHT sensor configuration
#define DHTPIN 4          // GPIO pin where DHT sensor is connected
#define DHTTYPE DHT11     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi connection
    float temperature = dht.readTemperature();

    if (isnan(temperature)) {  // Check if the reading is successful
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Create HTTP request to post data
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(temperature);
    http.begin(url);

    int httpResponseCode = http.GET();  // Send HTTP GET request
    if (httpResponseCode > 0) {
      Serial.print("Temperature posted successfully: ");
      Serial.println(temperature);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(httpResponseCode);
    }
    http.end();  // Free resources
  } else {
    Serial.println("Wi-Fi not connected");
  }

  delay(15000);  // Send data every 15 seconds
}