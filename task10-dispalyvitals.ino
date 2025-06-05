#include <Wire.h>
#include <WiFi.h>
#include <MAX30100_PulseOximeter.h>

#define REPORTING_PERIOD_MS 1000  // Reporting period

// Wi-Fi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Web server
WiFiServer server(80);

// MAX30100 sensor
PulseOximeter pox;
uint32_t tsLastReport = 0;

// Normal range thresholds
#define NORMAL_HEART_RATE_MIN 60
#define NORMAL_HEART_RATE_MAX 100
#define NORMAL_SPO2_MIN 95

// Variables to hold sensor data
float heartRate = 0;
float spo2 = 0;

// Function to handle web server client requests
void handleClient(WiFiClient client) {
  // Prepare the HTML response
  String htmlResponse = "<html><body><h1>Heart Rate & SpO2</h1>";
  htmlResponse += "<p>Heart Rate: " + String(heartRate) + " bpm</p>";
  htmlResponse += "<p>Oxygen Level: " + String(spo2) + " %</p>";

  if (heartRate < NORMAL_HEART_RATE_MIN || heartRate > NORMAL_HEART_RATE_MAX) {
    htmlResponse += "<p style='color:red;'>Alert: Abnormal heart rate detected!</p>";
  }

  if (spo2 < NORMAL_SPO2_MIN) {
    htmlResponse += "<p style='color:red;'>Alert: Low oxygen level detected!</p>";
  }

  htmlResponse += "</body></html>";

  // Send the HTML response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(htmlResponse);
}

// Callback function for pulse oximeter
void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);
  
  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();

  // Initialize the MAX30100 sensor
  if (!pox.begin()) {
    Serial.println("Failed to initialize MAX30100 sensor");
    while (1);
  }
  
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        client.readStringUntil('\r');  // Clear incoming request
        handleClient(client);
        break;
      }
    }
    client.stop();
  }

  // Measure heart rate and SpO2
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    heartRate = pox.getHeartRate();
    spo2 = pox.getSpO2();

    // Display on Serial Monitor
    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.print(" bpm - SpO2: ");
    Serial.print(spo2);
    Serial.println(" %");

    // Check for abnormal values and notify
    if (heartRate < NORMAL_HEART_RATE_MIN || heartRate > NORMAL_HEART_RATE_MAX) {
      Serial.println("Alert: Abnormal heart rate detected!");
    }
    if (spo2 < NORMAL_SPO2_MIN) {
      Serial.println("Alert: Low oxygen level detected!");
    }

    tsLastReport = millis();
  }
}