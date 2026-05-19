# car-telemetry-monitor

A car telemetry monitoring and display system for the STM32F103C8 (Blue Pill) microcontroller.

> This project is part of a bachelor's thesis at **Masaryk University in Brno — Faculty of Informatics**, spring semester 2026.

## Overview

This project captures real-time telemetry from an IMU, a barometer, and a GPS receiver. Raw accelerometer and gyroscope readings are processed through the Madgwick sensor fusion algorithm to produce accurate roll and pitch estimates, while altitude is derived by blending GPS and barometric pressure data. The system also monitors vibration and detects impacts by tracking the magnitude of acceleration over time.

All computed values are presented on a touchscreen display through multiple switchable screens such as speed, orientation, altitude and vibration intensity. At the same time, every sensor sample is written to a timestamped CSV file on an SD card for post-drive analysis. The entire system runs in a non-blocking, interrupt-driven loop to meet real-time constraints on the STM32.

## Board

| | |
|---|---|
| **MCU** | STM32F103C8T6 (Blue Pill) |
| **Platform** | ststm32 (PlatformIO) |
| **Framework** | Arduino |

## Hardware Components & Connections

| Component | Model | Interface | Pins |
|---|---|---|---|
| IMU (accelerometer + gyroscope) | MPU6050 | I2C | SCL → PB6, SDA → PB7 |
| Barometer | BMP280 | I2C | SCL → PB6, SDA → PB7 |
| GPS receiver | u-blox NEO-8M | UART2 | TX → PA2, RX → PA3 |
| Bluetooth | HC-06 | UART1 | TX → PA9, RX → PA10 |
| Display | Nextion NX4024K032 | UART3 | TX → PB10, RX → PB11 |
| SD card reader | — | SPI | MOSI → PA7, MISO → PA6, SCK → PA5, CS → PA4 |

All components are powered from 5 V pin.

## Libraries

| Library | Version | Purpose |
|---|---|---|
| Adafruit MPU6050 | ^2.2.6 | IMU driver |
| Adafruit BMP280 Library | ^3.0.0 | Barometer driver |
| Adafruit RTClib | ^2.1.4 | DateTime class and time utilities |
| SdFat – Adafruit Fork | ^2.3.102 | SD card filesystem |
| TinyGPSPlus | ^1.1.0 | GPS NMEA parsing |
| Madgwick | ^1.2.0 | IMU sensor fusion |
| Easy Nextion Library | ^1.0.6 | Nextion display communication |
| Adafruit Unified Sensor | ^1.1.15 | Sensor abstraction layer |

## Display Interface

The touchscreen UI is built with the [Nextion Editor](https://nextion.tech/nextion-editor/). The `.HMI` project file is included in this repository, allowing the display layout and screens to be modified and re-flashed independently of the firmware.

## Building

Install [PlatformIO](https://platformio.org/), then:

```sh
pio run
pio run --target upload
```
