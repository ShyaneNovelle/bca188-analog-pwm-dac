# Laboratory Report: Comparative Analysis of ADC, PWM, and DAC Signals on the ESP32

---

## 1. Project Background & Overview

Interfacing embedded systems with physical environments requires translating between continuous physical phenomena and discrete digital processing. Microcontrollers handle this balance using conversion and modulation peripherals. However, a common source of confusion in embedded hardware design is distinguishing between **simulated analog levels via high-speed switching** and **true, steady-state analog direct-current (DC) voltages**.

This laboratory experiment investigates the operation, mathematical relationships, and behavioral nuances of three core signal subsystems on the **ESP32 microcontroller**:
1. **Analog-to-Digital Conversion (ADC):** Sampling a variable voltage divider (potentiometer) and digitizing it into raw 12-bit integer values ($0\text{–}4095$) with 11 dB attenuation.
2. **Pulse-Width Modulation (PWM):** Converting raw digitized inputs into an 8-bit duty cycle ($0\text{–}255$) using the ESP32 Core v3 `ledc` peripheral to modulate effective average power on **GPIO 19** at 5 kHz[cite: 3, 9].
3. **Digital-to-Analog Conversion (DAC):** Driving the internal 8-bit R-2R resistive ladder on **GPIO 25** to synthesize genuine, continuous DC output levels across five distinct code stages ($0, 64, 128, 192, 255$).

---

## 2. Hardware Requirements & Pinout

### Equipment & Components
* **Microcontroller:** ESP32 Development Board (ESP32-WROOM-DA Module / Dual-Core with DAC support)[cite: 5, 9]
* **Passive Input:** $10\text{ k}\Omega$ linear potentiometer
* **Output Actuator:** LED with current-limiting resistor (or direct probe line on GPIO 19)
* **Test & Measurement:**
  * Solderless Breadboard & Dupont Jumper Wires
  * Digital Multimeter (DMM) for DC voltage verification
  * Digital Storage Oscilloscope (DSO) for waveform inspection on GPIO 19 vs. GPIO 25

### Pin Mapping

| Pin | Peripheral Role | Operating Range / Format | Description |
| :--- | :--- | :--- | :--- |
| **GPIO 34** | ADC1 Channel 6 | $0\text{ V} - 3.3\text{ V}$ (12-bit: $0\text{–}4095$)[cite: 2] | Potentiometer wiper analog input[cite: 2] |
| **GPIO 19** | LEDC PWM Channel | $5000\text{ Hz}$, 8-bit ($0\text{–}255$) | High-frequency switching digital output[cite: 3, 9] |
| **GPIO 25** | DAC Channel 1 | $0\text{ V} - 3.3\text{ V}$ (8-bit: $0\text{–}255$) | True analog DC voltage output |

---

## 3. Implementation Sketches

### Example 3: Analog-to-Digital Conversion (`lab_4_3.ino`)
Reads raw discrete counts and calibrated millivolts from the potentiometer wiper on GPIO 34.

```cpp
#include <Arduino.h>

const uint8_t POT_PIN = 34;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(POT_PIN, ADC_11db);
}

void loop() {
  const int raw = analogRead(POT_PIN);
  const uint32_t millivolts = analogReadMilliVolts(POT_PIN);

  Serial.print("Raw:");
  Serial.print(raw);

  Serial.print("\tMillivolts:");
  Serial.println(millivolts);

  delay(1000);
}