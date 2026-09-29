# 💧 TDS Water Quality Monitoring System

An Arduino-based **Total Dissolved Solids (TDS) monitoring system** designed to measure the dissolved solids present in water and display the measured TDS value in **parts per million (ppm)**.

The system uses a TDS sensor connected to an analog input, processes sensor readings using **median filtering and temperature compensation**, and displays the calculated TDS value on a **16×2 I2C LCD**. LEDs provide a visual indication based on the measured TDS range.

---

## 📌 Project Overview

**Total Dissolved Solids (TDS)** represents the concentration of dissolved substances present in water, including minerals, salts, and other conductive materials.

This project continuously reads the analog output of a TDS sensor and converts the sensor voltage into an estimated TDS value in ppm.

### Main Features

* 📡 Analog TDS sensor measurement
* 🔢 TDS measurement in ppm
* 📊 Median filtering for stable sensor readings
* 🌡️ Temperature compensation
* 🖥️ 16×2 I2C LCD display
* 💡 LED-based status indication
* 🔌 Serial Monitor output at 115200 baud
* ⏱️ Periodic sensor sampling
* 🧹 Noise reduction using multiple samples

---

## 🧰 Hardware Requirements

| Component     |    Quantity | Purpose                   |
| ------------- | ----------: | ------------------------- |
| Arduino board |           1 | Main controller           |
| TDS Sensor    |           1 | Measures dissolved solids |
| 16×2 I2C LCD  |           1 | Displays TDS value        |
| LED           |           4 | Status indication         |
| Resistor      |           4 | LED current limiting      |
| Jumper wires  | As required | Connections               |
| Breadboard    |           1 | Prototyping               |
| Water sample  | As required | Testing                   |

---

## 🔌 Pin Configuration

### TDS Sensor

| TDS Sensor    | Arduino |
| ------------- | ------- |
| Analog Output | A1      |
| VCC           | 5V      |
| GND           | GND     |

The sensor analog output is read using:

```cpp
#define TdsSensorPin A1
```

### I2C LCD

The LCD uses I2C address `0x27`.

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

For an Arduino Uno:

| LCD | Arduino UNO |
| --- | ----------- |
| VCC | 5V          |
| GND | GND         |
| SDA | A4          |
| SCL | A5          |

> I2C pins may differ depending on the Arduino board.

### LED Status Pins

| Arduino Pin | Function                  |
| ----------: | ------------------------- |
|          D6 | TDS between 1 and 50 ppm  |
|          D7 | TDS ≥ 50 ppm              |
|          D8 | Other/low-range condition |
|          D9 | TDS = 0                   |

---

## 📦 Required Library

The project requires the:

**LiquidCrystal_I2C** library.

Install it through:

```text
Arduino IDE
→ Sketch
→ Include Library
→ Manage Libraries
→ Search: LiquidCrystal I2C
```

The code uses:

```cpp
#include <LiquidCrystal_I2C.h>
```

---

## ⚙️ Working Principle

The system follows this process:

```text
TDS Sensor
    │
    ▼
Analog Input A1
    │
    ▼
30 Sensor Samples
    │
    ▼
Median Filtering
    │
    ▼
Voltage Conversion
    │
    ▼
Temperature Compensation
    │
    ▼
TDS Calculation
    │
    ├──────────────► 16×2 LCD
    │
    ├──────────────► Serial Monitor
    │
    └──────────────► Status LEDs
```

---

## 🔬 Sensor Sampling

The TDS sensor is sampled every **40 milliseconds**.

```cpp
if (millis() - analogSampleTimepoint > 40U)
```

The system stores **30 samples**:

```cpp
#define SCOUNT 30
```

These samples are subsequently processed using median filtering.

---

## 📊 Median Filtering

Sensor measurements may contain electrical noise or occasional spikes.

The project uses a median-filtering algorithm through:

```cpp
int getMedianNum(int bArray[], int iFilterLen)
```

The 30 samples are sorted and the median value is selected.

This provides a more stable sensor reading before converting the ADC value into voltage.

---

## 🌡️ Temperature Compensation

The code uses:

```cpp
float temperature = 25;
```

Therefore, the default temperature is **25°C**.

The compensation coefficient is calculated as:

```cpp
float compensationCoefficient =
    1.0 + 0.02 * (temperature - 25.0);
```

The compensated voltage is then used for TDS calculation.

> **Note:** The current implementation does not use a physical temperature sensor. The temperature is fixed at 25°C unless manually modified in the code.

---

## 🧮 TDS Calculation

The compensated voltage is converted into TDS using the polynomial equation implemented in the code:

```cpp
tdsValue =
    (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
    - 255.86 * compensationVoltage * compensationVoltage
    + 857.39 * compensationVoltage) * 0.5;
```

The resulting value is represented in:

```text
ppm
```

---

## 🖥️ Serial Monitor

Serial communication is initialized at:

```cpp
Serial.begin(115200);
```

Example output:

```text
TDS Value:25ppm
TDS Value:32ppm
TDS Value:47ppm
TDS Value:65ppm
```

Set the Arduino IDE Serial Monitor to:

```text
115200 baud
```

---

## 📺 LCD Display

The 16×2 I2C LCD displays the calculated TDS value.

Example:

```text
__TDS Value : 35
Water Quality
```

The LCD is initialized using:

```cpp
lcd.init();
lcd.backlight();
```

---

## 💡 LED Indication

The LEDs indicate different TDS conditions.

### 1. TDS between 1 and 50 ppm

```cpp
if (tdsValue > 1 && tdsValue < 50)
```

**D6 is activated.**

### 2. TDS equal to 0 ppm

```cpp
else if (tdsValue == 0)
```

**D9 is activated.**

### 3. TDS greater than or equal to 50 ppm

```cpp
else if (tdsValue >= 50)
```

**D7 is activated.**

### 4. Other values

**D8 is activated.**

---

## 🚀 Installation and Setup

### 1. Install Arduino IDE

Install the Arduino IDE on your computer.

### 2. Install LiquidCrystal_I2C

Open:

```text
Sketch → Include Library → Manage Libraries
```

Search for:

```text
LiquidCrystal I2C
```

and install the required library.

### 3. Connect the Hardware

Connect:

* TDS sensor → Arduino A1
* I2C LCD → Arduino I2C pins
* LEDs → D6, D7, D8 and D9
* Common GND between components

Use appropriate current-limiting resistors for the LEDs.

### 4. Open the Code

Open:

```text
final_code.ino
```

in Arduino IDE.

### 5. Select the Board

Go to:

```text
Tools → Board
```

and select the appropriate Arduino board.

### 6. Select the Port

Connect the Arduino through USB and select:

```text
Tools → Port
```

### 7. Upload

Click **Upload** in Arduino IDE.

After successful uploading, the system will begin monitoring the TDS sensor.

---

## 🧪 Testing Procedure

1. Power the Arduino.
2. Connect the TDS sensor.
3. Open the Serial Monitor.
4. Set the baud rate to **115200**.
5. Place the TDS probe in the water sample.
6. Allow the readings to stabilize.
7. Observe the TDS value on the LCD.
8. Observe the Serial Monitor output.
9. Check the corresponding LED indication.

---

## 📁 Project Structure

```text
TDS-Water-Quality-Monitor/
│
├── final_code.ino
└── README.md
```

---

## ⚙️ Configuration

The following parameters can be modified according to the hardware setup.

### TDS Sensor Pin

```cpp
#define TdsSensorPin A1
```

### ADC Reference Voltage

```cpp
#define VREF 5.0
```

### Number of Samples

```cpp
#define SCOUNT 30
```

### Temperature

```cpp
float temperature = 25;
```

### LCD Address

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

If the LCD does not respond, its I2C address may be different. Common addresses include:

```text
0x27
0x3F
```

---

## ⚠️ Limitations

The current implementation has several limitations:

* Temperature is fixed at 25°C.
* No physical temperature sensor is connected.
* TDS calibration is not implemented.
* LED outputs are not reset before activating another LED.
* `delay(2000)` temporarily blocks the main loop.
* The code assumes a 5 V ADC reference.
* The TDS conversion equation depends on the sensor/module and calibration.
* Accurate quantitative measurements require appropriate sensor calibration.

---

## 🔧 Possible Improvements

Future versions could include:

* 🌡️ DS18B20 temperature sensor integration
* 🎯 TDS sensor calibration
* 📈 Data logging
* 📱 Bluetooth/Wi-Fi monitoring
* ☁️ IoT/cloud dashboard
* 📊 Historical TDS graphs
* 🚨 Buzzer-based warning system
* 💡 Improved LED state management
* ⚡ Non-blocking timing using `millis()`
* 💾 EEPROM storage for calibration parameters
* 📡 ESP32-based wireless monitoring

---

## 📚 Concepts Demonstrated

This project demonstrates:

* Analog sensor interfacing
* ADC data acquisition
* Sensor signal processing
* Median filtering
* Voltage conversion
* Temperature compensation
* I2C LCD interfacing
* Digital GPIO control
* Serial communication
* Embedded timing
* Water-quality monitoring

---

## 👨‍💻 Author

**Arun M**

Robotics & Automation Engineer

Arduino-based TDS Water Quality Monitoring System

---

## ⭐ Project Summary

This project implements a simple embedded system for monitoring **Total Dissolved Solids (TDS)** in water.

An analog TDS sensor is connected to an Arduino. Multiple sensor readings are collected and processed using a median-filtering algorithm. The filtered signal is converted into voltage, temperature compensated, and converted into a TDS value in ppm.

The resulting measurement is displayed on a **16×2 I2C LCD**, transmitted through the **Serial Monitor**, and represented using **LED indicators**.

**Author: Arun M**
