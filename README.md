# ESP32 Signal Interfacing: Comparative Analysis of ADC, PWM, and DAC

---

## 1. Project Overview & Background

Embedded systems usually interact with the physical world via analog signals (such as sensors and continuous physical phenomena) and digital signals (for example, the logic of microcontrollers and communication buses). A common source of confusion in the development of microcontrollers is the basic difference between **digitally simulated analog signals** and **true analog voltage levels**.

This project provides an empirical investigation into signal acquisition, modulation, and true analog synthesis using the **ESP32 microcontroller**:

1. **Analog-to-Digital Conversion (ADC):** Read a changing analog voltage from a potentiometer. Turn it into raw, separate numbers. Find any hardware limits or non-linear behavior.
2. **Pulse-Width Modulation (PWM):** The digital approximation of varying power output is achieved by altering the duty cycle of a square wave with a fixed frequency.
3. **Digital-to-Analog Conversion (DAC):** Creating true, steady direct-current (DC) analog voltages by means of the ESP32's built-in 8-bit DAC channels based on a resistive ladder.

The behaviour, mathematical characteristics and waveform differences between duty-cycle-averaged digital switching and the actual analog voltages can be made clear by carrying out three separate runs of the program (Examples 3, 4 and 5) and examining the outputs on multimeters and an oscilloscope.

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

## 3. Software Implementations (Sketches)

The project is broken into three modular examples:

* **Example 3: Analog Data Acquisition (`Example3_ADC.ino`)**
  * Reads the 12-bit successive-approximation register (SAR) ADC on GPIO34.
  * Maps physical rotation into integer raw values ranging theoretically from $0$ to $4095$.
* **Example 4: PWM Signal Synthesis (`Example4_PWM.ino`)**
  * Configures the ESP32 LED Control (`ledc`) peripheral on GPIO19 at an 8-bit resolution ($0 - 255$).
  * Demonstrates dynamic pulse width modulation governed by input code settings.
* **Example 5: True Analog DAC Output (`Example5_DAC.ino`)**
  * Directs the built-in 8-bit DAC on GPIO25 using `dacWrite(25, value)`.
  * Generates stable DC intermediate voltage levels.

---

## 4. Project Deliverables Checklist

- [x] Three independent sketches: `Example3_ADC`, `Example4_PWM`, and `Example5_DAC`.
- [x] Experimental measurement table comparing raw ADC, calculated PWM duty, and physical DAC voltages.
- [x] Waveform comparison analysis (GPIO19 vs. GPIO25).
- [x] Technical explanation of PWM vs. DAC and ADC endpoint saturation mechanisms.
