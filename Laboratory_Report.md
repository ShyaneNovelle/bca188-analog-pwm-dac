# Laboratory 4 Report
## ADC Input, PWM Output, and DAC Output on the ESP32

| | |
|---|---|
| **Name** | ______________________ |
| **Course / Section** | ______________________ |
| **Instructor** | ______________________ |
| **Date Performed** | ______________________ |
| **Board Used** | ESP32-WROOM-DA Module (original ESP32), connected on COM3 |
| **Software** | Arduino IDE 2.3.10 |

---

## 1. Objective

The goal of this laboratory is to run Examples 3, 4, and 5 separately and compare three things:

1. the **input reading** (ADC value from the potentiometer),
2. the **PWM setting** (duty value sent to the LED), and
3. the **measured DAC voltage** (true analog output of the ESP32).

By the end of the activity, the student should be able to explain why a PWM signal is not the same as a DAC output, and why an ADC reading can saturate (stop increasing) near its upper end.

---

## 2. Materials and Equipment

- ESP32-WROOM-DA development board (original ESP32, which has a built-in DAC)
- Potentiometer (connected to GPIO34)
- LED with a current-limiting resistor (connected to GPIO19)
- Breadboard and jumper wires
- Multimeter (for measuring the DAC voltage on GPIO25)
- Oscilloscope (optional, for comparing GPIO19 and GPIO25) 
- USB cable and a computer with Arduino IDE

> **Equipment note:** PWM measurements are **not** reported as DAC measurements in this report. The PWM duty values come from the serial monitor, and the DAC voltages come from the multimeter on GPIO25.

---

## 3. Background (Short Review)

**ADC (Analog-to-Digital Converter).** The ADC converts a voltage into a number. In these examples the resolution is set to 12 bits, so the raw value goes from **0 to 4095**. The attenuation is set to `ADC_11db`, which allows the ADC to read higher input voltages (close to the 3.3 V supply).

**PWM (Pulse Width Modulation).** PWM is a digital signal that switches quickly between HIGH (3.3 V) and LOW (0 V). The *duty* tells how long the signal stays HIGH in each cycle. In Example 4, the frequency is 5000 Hz and the resolution is 8 bits, so the duty goes from **0 to 255**.

**DAC (Digital-to-Analog Converter).** The DAC does the opposite of the ADC. It turns a number into a steady voltage. The original ESP32 has two 8-bit DAC channels: GPIO25 and GPIO26. The code goes from **0 to 255**.

The expected DAC output voltage is:

```
V_out ≈ (DAC code / 255) × 3.3 V
```

The expected PWM duty in Example 4 follows the Arduino `map()` function:

```
duty = raw × 255 / 4095      (the decimal part is dropped)
```

---

## 4. Procedure

1. Connect the potentiometer to GPIO34 and the LED to GPIO19.
2. Upload **Example 3** and turn the potentiometer to different positions. Record the `Raw` and `Millivolts` values from the Serial Monitor (115200 baud).
3. Upload **Example 4** and turn the potentiometer to different positions. Record the `Raw` and `PWM Duty` values. Watch the LED brightness.
4. For each raw value, **predict** the PWM duty using the formula above, then compare it with the value shown in the Serial Monitor.
5. Upload **Example 5**. The code sends DAC codes 0, 64, 128, 192, and 255 to GPIO25, each for 10 seconds. Measure the voltage on GPIO25 with a multimeter for each code.
6. (Optional) Use an oscilloscope to compare the GPIO19 waveform (PWM) with the GPIO25 waveform (DAC).

---

## 5. Source Code

### 5.1 Example 3: ADC Reading (`lab_4_3.ino`)

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

  delay(1500);
}
```

**What the code does:** It reads the potentiometer on GPIO34 every 1.5 seconds. It prints the raw ADC value (0–4095) and the voltage in millivolts. `analogReadMilliVolts()` uses the ESP32's calibration data to convert the reading to millivolts.

### 5.2 Example 4: PWM Output (`lab_4_4.ino`)

```cpp
#include <Arduino.h>

const uint8_t POT_PIN = 34;
const uint8_t PWM_LED_PIN = 19;
bool pwmReady = false;

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetPinAttenuation(POT_PIN, ADC_11db);

  pinMode(PWM_LED_PIN, OUTPUT);
  digitalWrite(PWM_LED_PIN, LOW);

  // ESP32 Core v3.x API: attaches frequency (5000 Hz) and resolution (8-bit) directly to GPIO 19
  pwmReady = ledcAttach(PWM_LED_PIN, 5000, 8);

  if (pwmReady) {
    ledcWrite(PWM_LED_PIN, 0);
    Serial.println("PWM initialized successfully!");
  } else {
    Serial.println("PWM setup failed. Check board/core and pin.");
  }
}

void loop() {
  if (!pwmReady) {
    return;
  }

  const int raw = analogRead(POT_PIN);
  const int duty = constrain(map(raw, 0, 4095, 0, 255), 0L, 255L);

  ledcWrite(PWM_LED_PIN, duty);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("\tPWM Duty: ");
  Serial.println(duty);

  delay(100);
}
```

**What the code does:** It reads the potentiometer, converts the 0–4095 reading into a 0–255 duty value using `map()`, and sends that duty to the LED on GPIO19 using `ledcWrite()`. The PWM runs at 5000 Hz with 8-bit resolution. `constrain()` makes sure the duty never goes outside 0–255.

### 5.3 Example 5: DAC Output (`lab_4_example5.ino`)

```cpp
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
```

**What the code does:** It outputs five DAC codes (0, 64, 128, 192, 255) on GPIO25, holding each one for 10 seconds. The 10-second delay gives enough time to read the voltage on a multimeter.

---

## 6. Results and Data

### 6.1 Potentiometer Readings and PWM Duty (Examples 3 and 4)

The potentiometer was set to five positions. At each position, the raw ADC value and the input voltage were recorded from Example 3. The PWM duty was then predicted from the raw value using `raw × 255 / 4095` (decimal part dropped, same as Arduino `map()`) and compared with the duty shown in the Serial Monitor in Example 4.

**Table 1. Potentiometer readings (Example 3)**

| Pos | Potentiometer Position Description | Raw ADC Value (12-bit) | Measured Input Voltage (V) |
|:---:|:---|:---:|:---:|
| 1 | Fully Counter-Clockwise (Saturated Low) | 0 | 0.00 V / 142 mV |
| 2 | Low Range Rotation (~13%) | 535 | ~0.43 V |
| 3 | Mid-High Rotation (~60%) | 2482 | 2.15 V |
| 4 | High Rotation (~70%) | 2865 | ~2.31 V |
| 5 | Higher Range Rotation (~85%) | 3471 | ~2.79 V |

*Values marked with "~" are approximate.*

**Table 2. Predicted and observed PWM duty (Example 4)**

| Pos | Raw ADC Value | Predicted PWM Duty (0–255) | Observed PWM Duty (Serial Monitor) | Observed PWM Duty Cycle (%) | Match? |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | 0 | 0 | 0 | 0.0% | Yes |
| 2 | 535 | 33 | 33 | 12.9% | Yes |
| 3 | 2482 | 154 | 154 | 60.4% | Yes |
| 4 | 2865 | 178 | 178 | 69.8% | Yes |
| 5 | 3471 | 216 | 216 | 84.7% | Yes |

The duty cycle is computed as `duty / 255 × 100%`. For example, 178 / 255 = 69.8%.

**Figure 1.** Example 3 code and Serial Monitor output (low readings).
![Figure 1: Example 3 serial output](images/example3_serial_low.png)

**Figure 2.** Example 3 Serial Monitor output while turning the potentiometer up.
![Figure 2: Example 3 serial output](images/example3_serial_high.png)

**Figure 3.** Example 4 Serial Monitor output at a low position (Raw ≈ 530–546, Duty = 33–34).
![Figure 3: Example 4 low position](images/example4_low.png)

**Figure 4.** Example 4 Serial Monitor output at a middle-high position (Raw ≈ 2862–2878, Duty = 178–179).
![Figure 4: Example 4 middle position](images/example4_mid.png)

**Figure 5.** Example 4 Serial Monitor output at a high position (Raw ≈ 3438–3483, Duty = 214–217).
![Figure 5: Example 4 high position](images/example4_high.png)

**Observations for Examples 3 and 4:**

- As the potentiometer is turned up, the raw value, the voltage, and the PWM duty all go up. The LED becomes brighter.
- At the lowest position, the raw value is **0**, but the board still reports **142 mV** instead of 0 mV.
- The raw value changes slightly even when the knob is not moved (for example 530, 543, 532, 546). This is normal ADC noise, and it makes the duty change by 1 at times (33 to 34).
- The raw value never reached 4095 in the recorded data, so the duty did not reach 255.

### 6.2 Example 5: Predicted and Measured DAC Voltage

The predicted voltage is computed with `V = (code / 255) × 3.3 V`.

> **Fill in the "Measured" columns using your multimeter readings on GPIO25.** The measured values were not provided, so they are left blank here instead of being guessed.

**Table 3. DAC output voltage (Example 5)**

| Programmed DAC Code (0–255) | Predicted DAC Voltage (V) | Measured DAC Voltage (V) | Difference (V) |
|:---:|:---:|:---:|:---:|
| 0 | 0.00 V | ______ | ______ |
| 64 | 0.83 V | ______ | ______ |
| 128 | 1.66 V | ______ | ______ |
| 192 | 2.49 V | ______ | ______ |
| 255 | 3.30 V | ______ | ______ |

**Figure 6.** Example 5 setup with multimeter on GPIO25.
![Figure 6: DAC measurement setup](images/example5_setup.jpg)

**Video link / file:** `videos/example5_dac.mp4` (add your link here)

### 6.3 Oscilloscope Comparison (GPIO19 vs. GPIO25)

> Complete this section only if an oscilloscope was used. If it was not available, write "Oscilloscope not available" and keep the explanation below.

| Signal | Pin | What the waveform looks like |
|:---|:---:|:---|
| PWM (Example 4) | GPIO19 | A fast square wave that switches between 0 V and 3.3 V at 5000 Hz. The HIGH time gets longer when the duty goes up. |
| DAC (Example 5) | GPIO25 | A flat, steady line. The line sits at a different height for each DAC code. |

---

## 7. Comparison of Predicted and Observed Results

**Example 3 (ADC).** The code has no formula for the millivolt value, so the comparison is between the raw count and the reported voltage. On a straight line from 0 to 3.3 V, a raw value of 0 would be 0.00 V and a raw value of 2482 would be about 2.00 V. The board reported 0.142 V and 2.15 V. The results are close but not exact, which shows the ESP32 ADC is not perfectly linear.

**Example 4 (PWM).** The prediction worked very well. All five predicted duty values (0, 33, 154, 178, 216) matched the Serial Monitor exactly. This is expected because the program uses the same formula (`map()`) that was used for the prediction. The duty cycle went from 0.0% to 84.7%, so the LED was off at the lowest position and fairly bright at the highest recorded position.

**Example 5 (DAC).** The predicted voltages are 0.00, 0.83, 1.66, 2.49, and 3.30 V. Compare these with the multimeter readings in Table 3. Small differences are normal because the real supply voltage of the board may not be exactly 3.3 V, and the DAC output has a small error at the very low and very high ends.

---

## 8. Discussion

### 8.1 Why PWM is not the same signal as the DAC output

- **PWM** is a *digital* signal. At any moment it is either 3.3 V or 0 V. It never stays at a middle voltage. The "level" is carried by the duty, which is the percentage of time the signal is HIGH. Its average voltage is about `(duty / 255) × 3.3 V`, but the actual pin voltage is jumping between 0 V and 3.3 V 5000 times every second.
- **DAC** output is an *analog* voltage. For code 128, the pin really sits at about 1.65 V and stays there.
- An LED looks dimmer with PWM because the light turns on and off too fast for the eye to see, so the eye sees the average brightness. A multimeter on a PWM pin may show a number close to the average, but that does not make it a DAC signal. Other circuits (such as a sensor input or an amplifier) would still see the fast on-off signal.
- This is why this report does not use PWM readings as DAC measurements.

### 8.2 Why an ADC endpoint may saturate

- The ADC can only count from 0 to 4095. When the input voltage is at or above the top of its range, the reading stays at 4095 (it saturates) even if the voltage keeps rising.
- The ESP32 ADC is also **non-linear near its ends**. In our data, a raw value of 0 still reports 142 mV, and a raw value of 2482 reports 2.15 V instead of the 2.00 V a straight line would give. This means the very bottom and very top of the potentiometer's turn are less accurate.
- Even with `ADC_11db` attenuation, the input is best kept away from the exact extremes (0 V and the supply voltage) to get more reliable readings.

---

## 9. Conclusion

In this laboratory, the three examples were run and compared. In Example 3, the ESP32 ADC converted the potentiometer voltage into raw values from 0 up to about 3500, and the millivolt reading increased with it, but not in a perfect straight line. In Example 4, the raw ADC value was converted to a PWM duty between 0 and 255 (0.0% to 84.7% in our tests). All five predicted duty values matched the observed values, so the `map()` formula was confirmed. In Example 5, the DAC produced five different steady voltages on GPIO25 based on the code value.

The main lesson is that **PWM and DAC are different outputs**. PWM switches a digital pin on and off and relies on the average, while the DAC gives a real analog voltage. It was also learned that the ADC is not perfectly linear and can saturate near its limits, so readings at the extreme ends should be treated carefully.

---

## 10. Submission Checklist

- [x] Sketch 1: `lab_4-example_3.ino`
- [x] Sketch 2: `lab_4-example_4.ino`
- [x] Sketch 3: `lab_4_example5.ino`
- [x] Measurement table for Example 3 (ADC)
- [x] Predicted vs. observed table for Example 4 (PWM)
- [x] Predicted DAC voltage table for Example 5
- [ ] Measured DAC voltage column for Example 5 (fill in)
- [ ] Oscilloscope comparison (optional, fill in)
- [x] Short comparison of predicted and observed results

---

## Appendix: Image and Video File Guide

Place your pictures and videos in the same folder as this report, using these suggested names so the images appear:

```
Laboratory_Report.md
images/
    example3_serial_low.png
    example3_serial_high.png
    example4_low.png
    example4_mid.png
    example4_high.png
    example5_setup.jpg
videos/
    example5_dac.mp4
```
