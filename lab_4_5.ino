#include <Arduino.h>

const uint8_t DAC_PIN = 25;
const uint8_t ADC_PIN = 34;

void setup() {
  Serial.begin(115200);
  
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

void loop() {
  const uint8_t dacSteps[] = {0, 64, 128, 192, 255};

  for (int i = 0; i < 5; i++) {
    uint8_t val = dacSteps[i];
    
    dacWrite(DAC_PIN, val);
    delay(100);

    uint32_t mv = analogReadMilliVolts(ADC_PIN);
    float volts = mv / 1000.0;

    Serial.print("DAC Code: ");
    Serial.print(val);
    Serial.print("\tMeasured Voltage: ");
    Serial.print(volts, 2);
    Serial.println(" V");

    delay(2000);
  }
}
