#include <WiFi.h>
#include <WebServer.h>

// Replace with your WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Web server on port 80
WebServer server(80);

// Game variables
const int ledPins[4] = {13, 12, 14, 27};
const int buttonPins[4] = {18, 19, 21, 22};
const int maxSequence = 100;
int sequence[maxSequence];
int level = 1;
bool gameOverFlag = false;

void setup() {
  Serial.begin(115200);
  
  // Init LEDs and buttons
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Start WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());

  // Web server routes
  server.on("/", handleRoot);
  server.begin();

  randomSeed(analogRead(0));
  generateSequence();
}

void loop() {
  server.handleClient();

  if (!gameOverFlag) {
    playSequence();
    if (!getUserInput()) {
      gameOverFlag = true;
      gameOverBlink();
    } else {
      level++;
      if (level > maxSequence) level = maxSequence;
      delay(1000);
    }
  }
}

// Generate full random sequence
void generateSequence() {
  for (int i = 0; i < maxSequence; i++) {
    sequence[i] = random(0, 4);
  }
}

// Play current sequence
void playSequence() {
  for (int i = 0; i < level; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(400);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(250);
  }
}

// Handle button inputs
bool getUserInput() {
  for (int i = 0; i < level; i++) {
    int expected = sequence[i];
    int pressed = waitForButtonPress();
    if (pressed != expected) {
      return false;
    }
  }
  return true;
}

// Wait for user to press a button
int waitForButtonPress() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        digitalWrite(ledPins[i], HIGH);
        delay(200);
        digitalWrite(ledPins[i], LOW);
        while (digitalRead(buttonPins[i]) == LOW);
        delay(100); // debounce
        return i;
      }
    }
  }
}

// Flash all LEDs on game over
void gameOverBlink() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) digitalWrite(ledPins[j], HIGH);
    delay(300);
    for (int j = 0; j < 4; j++) digitalWrite(ledPins[j], LOW);
    delay(300);
  }
}

// Web server handler
void handleRoot() {
  String html = "<html><head><title>Simon Says</title></head><body>";
  html += "<h1>Simon Says Game Status</h1>";
  html += "<p><strong>Level:</strong> " + String(level) + "</p>";
  html += "<p><strong>Status:</strong> " + String(gameOverFlag ? "Game Over" : "In Progress") + "</p>";
  html += "<button onclick=\"location.reload()\">Refresh</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}