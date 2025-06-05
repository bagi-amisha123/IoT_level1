#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

WebServer server(80);

#define LED_PIN 2  // GPIO for LED
const int dotDuration = 200;  // Duration of a dot in milliseconds

// Morse code dictionary
const char* morseAlphabet[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
  ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

// HTML page to enter a message
const char* htmlPage = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Morse Code Sender</title>
  <style>
    .form-container {
      display: flex;
      align-items: center;
    }
    .form-container input[type="text"] {
      margin-right: 10px;
    }
  </style>
</head>
<body>
  <h1>Morse Code Server</h1>
  <form action="/send" method="POST" class="form-container">
    <label for="message">Message:</label>
    <input type="text" id="message" name="message" placeholder="Type your message here">
    <input type="submit" value="Send">
  </form>
</body>
</html>
)=====";

// Convert a character to Morse code
String charToMorse(char c) {
  if (c >= 'a' && c <= 'z') {
    return morseAlphabet[c - 'a'];
  } else if (c >= 'A' && c <= 'Z') {
    return morseAlphabet[c - 'A'];
  } else if (c == ' ') {
    return " ";
  }
  return "";
}

// Flash Morse code for a single character
void flashMorseChar(char c) {
  String morse = charToMorse(c);

  for (int i = 0; i < morse.length(); i++) {
    if (morse[i] == '.') {
      digitalWrite(LED_PIN, HIGH);
      delay(dotDuration);  // Dot duration
    } else if (morse[i] == '-') {
      digitalWrite(LED_PIN, HIGH);
      delay(dotDuration * 3);  // Dash duration
    }
    digitalWrite(LED_PIN, LOW);
    delay(dotDuration);  // Space between dots and dashes
  }
  delay(dotDuration * 3);  // Space between characters
}

// Flash Morse code for a message
void flashMorseMessage(String message) {
  for (int i = 0; i < message.length(); i++) {
    flashMorseChar(message[i]);
    if (message[i] == ' ') {
      delay(dotDuration * 7);  // Space between words
    }
  }
}

// Handle root webpage
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Handle message sending
void handleSend() {
  String message = server.arg("message");
  flashMorseMessage(message);
  server.send(200, "text/html", "<h2>Message Sent in Morse Code</h2><a href='/'>Go Back</a>");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  server.on("/", handleRoot);
  server.on("/send", HTTP_POST, handleSend);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}