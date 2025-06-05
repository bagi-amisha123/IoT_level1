#define SOIL_MOISTURE_PIN 34  // Analog pin connected to soil moisture sensor
#define MOISTURE_THRESHOLD 500  // Adjust this threshold based on calibration

void setup() {
  Serial.begin(115200);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
}

void loop() {
  // Read soil moisture value
  int moistureLevel = analogRead(SOIL_MOISTURE_PIN);

  // Display the moisture level on the Serial Monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(moistureLevel);

  // Check if moisture level is below the threshold
  if (moistureLevel < MOISTURE_THRESHOLD) {
    Serial.println("Alert: Soil moisture level is low!");
  }

  delay(2000);  // Read every 2 seconds
}