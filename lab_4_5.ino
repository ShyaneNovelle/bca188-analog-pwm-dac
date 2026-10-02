#include <Arduino.h>

const int DAC_PIN = 25;

void setup() {
  Serial.begin(115200);
  dacWrite(DAC_PIN, 0);
}

void loop() {
  dacWrite(DAC_PIN, 0);
  Serial.println("DAC code: 0");
  delay(10000);

  dacWrite(DAC_PIN, 64);
  Serial.println("DAC code: 64");
  delay(10000);

  dacWrite(DAC_PIN, 128);
  Serial.println("DAC code: 128");
  delay(10000);

  dacWrite(DAC_PIN, 192);
  Serial.println("DAC code: 192");
  delay(10000);

  dacWrite(DAC_PIN, 255);
  Serial.println("DAC code: 255");
  delay(10000);
}
