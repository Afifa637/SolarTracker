# 🌞 Smart 1-Axis Solar Tracker — ESP32 + INA219 + Blynk

## Overview

This project demonstrates a **1-axis solar tracker** that automatically orients a small solar panel toward the brightest light source. It uses:

- **ESP32** microcontroller (Wi-Fi & Bluetooth)
- **Two LDRs** for light detection
- **SG90 servo** for panel movement
- **INA219 sensor** for voltage/current monitoring
- **2-channel relay module** for controlling loads
- **Blynk IoT platform** for remote telemetry and control

The system continuously tracks sunlight, reports real-time telemetry, and allows remote load control via Blynk.

---

## Objectives

1. Build a functioning 1-axis solar tracker using LDRs and servo.
2. Monitor solar panel voltage, current, and compute power using INA219.
3. Remotely control two loads (LEDs or small devices) via Blynk.
4. Ensure reliable operation with dedicated 5V supply for actuators and common ground.
5. Provide clear documentation, wiring diagrams, and reproducible code for educational use.

---

## Components

**Hardware**

- ESP32 development board
- 2 × LDRs + 2 × 10 kΩ resistors
- SG90 servo (5 V)
- INA219 I²C voltage/current sensor
- 2-channel 5 V relay module (optocoupled)
- Solar panel(s) (107 × 62 mm)
- Li-ion battery or regulated 5 V supply (≥2 A)
- Breadboard, wires, connectors, mechanical frame
- Capacitors: 1000 μF electrolytic, 0.1 μF ceramic
- LED loads + series resistors
- USB cable

**Software / Libraries**

- Arduino IDE with ESP32 board support
- Blynk library (`BlynkSimpleEsp32`)
- Adafruit INA219 library
- ESP32Servo library

---

## Wiring & Connections

### LDR Voltage Dividers

- Left: 3.3 V → LDR → ADC GPIO 34 → 10 kΩ → GND
- Right: 3.3 V → LDR → ADC GPIO 35 → 10 kΩ → GND

### Servo

- Signal: GPIO 18
- Power: 5 V
- GND: common ground
- Add 1000 μF capacitor across servo 5V and GND

### Relay Module

- JD-VCC: 5 V
- VCC (logic): 3.3 V or 5 V (check module)
- GND: common ground
- IN1 → GPIO 25, IN2 → GPIO 26 (active LOW)

### INA219

- VCC → ESP32 3.3 V
- GND → common ground
- SDA → GPIO 21, SCL → GPIO 22
- VIN+ → solar panel positive
- VIN− → system positive input (load side)

### Solar Panels

- For parallel: + terminals together, − terminals together
- Use blocking diodes to prevent reverse current

### LED Loads (Test)

- COM → 5 V
- NO → LED + resistor → LED − → GND

## Expected Results

- Servo moves toward brighter LDR side

- INA219 reports voltage, current, and power to Blynk

- Relays respond to Blynk toggles (active LOW)

- Proper decoupling prevents ESP32 resets

## References

1. [Espressif ESP32 Datasheet](https://www.espressif.com/en/products/socs/esp32/resources)  
2. [Adafruit INA219 Guide](https://learn.adafruit.com/adafruit-ina219-current-sensor)  
3. [Blynk Documentation](https://docs.blynk.io/)  
4. [ESP32Servo Library](https://github.com/madhephaestus/ESP32Servo)  

