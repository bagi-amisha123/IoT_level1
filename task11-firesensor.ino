#define SENSOR_PIN 34  // Select the input pin for the digital flame sensor
#define LED_PIN 9      // Output pin for LED (change to an appropriate GPIO for ESP32)

void setup() {
  // Declare the LED pin as an OUTPUT
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);  // Start serial communication at 115200 baud
}

void loop() {

  // Read the digital value from the flame sensor
  int sensorValue = digitalRead(SENSOR_PIN);

  // Print the sensor value to the Serial Monitor
  Serial.println(sensorValue);

  if (sensorValue == HIGH) {  // If a flame is detected
    Serial.println("Fire Detected");
    Serial.println("LED on");
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
  } else {
    digitalWrite(LED_PIN, LOW);    // Turn off the LED
  }

  delay(1000);  // Delay for a second before the next loop
}