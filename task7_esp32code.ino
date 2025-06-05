#include <WiFi.h>
#include <Wire.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = " ";
const char* password = " ";

// I2C configuration
#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_SLAVE_ADDRESS 0x04

WebServer server(80);

// HTML form
const char* htmlPage = R"=====( 
<!DOCTYPE html>
<html>
<head><title>ESP32 Message Sender</title></head>
<body>
  <h1>Send Message to Arduino</h1>
  <form action="/send" method="POST">
    <input type="text" name="message" placeholder="Type your message here">
    <input type="submit" value="Send">
  </form>
</body>
</html>
)=====";

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSend() {
  String message = server.arg("message");

  // Limit to 30 characters to avoid I2C buffer overflow
  if (message.length() > 30) {
    message = message.substring(0, 30);
  }

  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write((const uint8_t*)message.c_str(), message.length());  // 👈 Fix here
  Wire.endTransmission();

  Serial.println("Message sent via I2C: " + message);
  server.send(200, "text/html", "<h2>Message Sent: " + message + "</h2><a href='/'>Go Back</a>");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL); // SDA = GPIO 21, SCL = GPIO 22
  connectWiFi();

  server.on("/", handleRoot);
  server.on("/send", HTTP_POST, handleSend);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();  // Handle web requests
}