#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// MQTT broker information
const char* mqtt_server = "broker.hivemq.com";  // Example MQTT broker
const int mqtt_por
t = 1883;

// Initialize the LED pins
#define LED1_PIN 23
#define LED2_PIN 22
#define LED3_PIN 21

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function to handle incoming messages
void callback(char* topic, byte* message, unsigned int length) {
  message[length] = '\0';  // Null-terminate the message
  String msg = String((char*)message);
  
  Serial.print("Message received: ");
  Serial.println(msg);

  // Control LEDs based on the received message
  if (msg == "LED 1 ON") {
    digitalWrite(LED1_PIN, HIGH);
  } else if (msg == "LED 1 OFF") {
    digitalWrite(LED1_PIN, LOW);
  } else if (msg == "LED 2 ON") {
    digitalWrite(LED2_PIN, HIGH);
  } else if (msg == "LED 2 OFF") {
    digitalWrite(LED2_PIN, LOW);
  } else if (msg == "LED 3 ON") {
    digitalWrite(LED3_PIN, HIGH);
  } else if (msg == "LED 3 OFF") {
    digitalWrite(LED3_PIN, LOW);
  }
}

// Function to reconnect to MQTT broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("led/control");  // Subscribe to the topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Set LED pins as outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Attempt to connect to MQTT
  reconnect();
}

void loop() {
  // Ensure the client stays connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Maintain MQTT connection
}