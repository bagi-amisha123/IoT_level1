#include <Wire.h>

#define I2C_ADDRESS 0x04
char receivedMessage[32]; // Buffer to store incoming message
volatile byte index = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.println("Arduino I2C Slave Ready");
}

void loop() {
  // Print message when fully received
  if (index > 0) {
    receivedMessage[index] = '\0';  // Null-terminate string
    Serial.print("Received: ");
    Serial.println(receivedMessage);
    index = 0; // Reset for next message
  }
}

void receiveEvent(int bytes) {
  while (Wire.available() && index < sizeof(receivedMessage) - 1) {
    receivedMessage[index++] = Wire.read();
  }
}