# ESP32 Signal Interfacing: Comparative Analysis of ADC, PWM, and DAC

---

## 1. Project Overview & Background

Embedded systems routinely interact with the physical world through analog signals (sensors, continuous physical phenomena) and digital signals (microcontroller logic, communication buses). A frequent point of confusion in microcontroller development is the fundamental distinction between **digitally simulated analog signals** and **true analog voltage levels**.

This project provides an empirical investigation into signal acquisition, modulation, and true analog synthesis using the **ESP32 microcontroller**:

1. **Analog-to-Digital Conversion (ADC):** Reading a variable analog potential from a potentiometer, digitizing it into raw discrete numerical codes, and identifying hardware-level saturation/non-linearities.
2. **Pulse-Width Modulation (PWM):** Approximating variable power delivery digitally by modulating the duty cycle of a fixed-frequency square wave.
3. **Digital-to-Analog Conversion (DAC):** Generating genuine, static direct-current (DC) analog voltages using the ESP32's internal resistive-ladder 8-bit DAC channels.

By running three separate program implementations (Examples 3, 4, and 5) and observing the outputs on multimeters and an oscilloscope, this project clarifies the behavioral, mathematical, and waveform differences between duty-cycle-averaged digital switching and actual analog potentials.

---

## 2. Hardware Requirements

### Core Components
* **ESP32 Development Board** (Original ESP32-WROOM-32 / dual-core module; *Note: ESP32-S2/S3 or C-series boards may have different DAC/ADC configurations*).
* **10kΩ Potentiometer** (Linear taper, used as a variable voltage divider).
* **Solderless Breadboard & Male-to-Male Jumper Wires**.
* **Micro-USB / USB-C Cable** (Data and programming interface).

### Test & Measurement Equipment
* **Digital Multimeter (DMM):** For measuring DC rail voltage, potentiometer wiper voltage, and DAC output.
* **Digital Storage Oscilloscope (DSO):** Dual-channel preferred, to directly compare time-domain characteristics, rise/fall transitions, and voltage levels of GPIO19 against GPIO25.

---

## 3. System Architecture & Pin Mapping