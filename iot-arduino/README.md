# IoT & Embedded Systems (C++ / Arduino)

This directory contains embedded systems and IoT programming assignments completed as part of the Erasmus+ BIP "Internet of Things and 3D Printing" at the University of Novi Sad.

## Contents

1. **`Exercise1_SensorReading.cpp` & `Exercise2_SensorReadingAdvanced.cpp`**
   - **Topic:** Robust Sensor Reading & Error Handling in Unreliable Environments.
   - **Key Features:**
     - Multi-sample collection ($N=5$) with adjustable inter-reading delay.
     - Statistical outlier rejection (trimmed mean algorithm, excluding min/max).
     - Range consistency check (detects hardware faults when spread exceeds 15% of `MAXINT`).
     - Linear mapping from raw integer range $[0, \text{MAXINT}]$ to the target temperature scale $[-10.0^\circ\text{C}, 50.0^\circ\text{C}]$.
     - Clean error signaling using IEEE-754 special values (`-INFINITY` for configuration errors, `+INFINITY` for sensor data faults).

2. **`MovingAverageTracker.cpp`**
   - **Topic:** Efficient Window-Based Sensor Data Management.
   - **Key Features:**
     - Fixed-size sliding window ($N=16$).
     - $O(1)$ running sum maintenance for high performance on resource-constrained microcontrollers.
     - Warm-up phase tracking via `validSampleCount`.
     - Robust error detection and threshold-based control logic (activating heating/cooling actuators).

## Usage
These routines are written in standard C++ compatible with Arduino (ESP32, Uno, Mega) and embedded environments. They can be compiled directly within the Arduino IDE or tested in standard C++ toolchains.
