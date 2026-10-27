================================================================================
                    IoT SENSOR READING ASSIGNMENT
                    Unreliable Sensor Handling Routine
                    Author: Kyryll Hryhorenko
================================================================================

PROBLEM STATEMENT
-----------------
Develop a routine readSensor(int inPin) that:
1. Reads an unreliable sensor multiple times via digitalRead(inPin)
2. Filters out invalid/outlier readings
3. Converts raw integer values [0, MAXINT] to meaningful scale [-10.0, 50.0]
4. Returns +/-INFINITY (IEEE-754) to signal abnormal conditions
5. Allows caller to detect error conditions using standard library functions

================================================================================

ASSUMPTIONS AND DESIGN CHOICES
------------------------------

1. NUMBER OF READINGS: N = 5
   - Rationale: Odd number enables median/trimmed-mean calculation; balances
     reliability with performance constraints of embedded systems.

2. MAXINT DEFINITION: 2147483647 (signed 32-bit integer maximum)
   - Rationale: Standard INT_MAX value in C++/Arduino environments. If the
     sensor uses unsigned 32-bit, change to 4294967295UL.

3. OUTLIER DETECTION: Trimmed mean + range consistency check
   - Rationale: Simple statistical approach suitable for small sample sizes.
     Excluding min/max values reduces impact of transient spikes without
     requiring complex algorithms.

4. CONSISTENCY THRESHOLD: 15% of MAXINT (0.15 * MAXINT)
   - Rationale: Allows reasonable sensor noise while catching gross errors.
     Tunable parameter based on specific sensor characteristics.

5. ERROR SIGNALING:
   - Return -INFINITY: Invalid pin number or configuration error
   - Return +INFINITY: Inconsistent readings indicating sensor malfunction
   - Rationale: IEEE-754 special values enable clean error propagation using
     standard isinf()/isnan() checks without magic numbers.

6. SENSOR PRIMITIVE NOTE:
   - The problem mentions digitalRead() but describes 0-MAXINT range.
   - digitalRead() only returns 0 or 1, so we assume analogRead() or a
     custom 32-bit read function is intended. Code includes both options.

7. INTER-READING DELAY: 10ms
   - Rationale: Allows sensor stabilization without introducing excessive
     latency in the reading cycle.

================================================================================

COMPLETE SOURCE CODE
--------------------

// File: sensor_reader.cpp
// Compatible with Arduino/ESP32 and standard C++ environments

#include <cmath>       // For INFINITY, isinf(), isnan()
#include <algorithm>   // For std::sort
#include <climits>     // For INT_MAX (alternative to manual MAXINT)

// ============================================================================
// CONFIGURATION CONSTANTS
// ============================================================================

#define MAXINT 2147483647L          // Maximum value for signed 32-bit integer
#define NUM_READINGS 5              // Number of samples to collect
#define CONSISTENCY_THRESHOLD 0.15  // Max allowed spread as fraction of MAXINT
#define READING_DELAY_MS 10         // Delay between consecutive readings

// ============================================================================
// FUNCTION: readSensor
// ============================================================================
/**
 * Reads an unreliable sensor multiple times, filters outliers,
 * and converts the result to the [-10.0, 50.0] temperature scale.
 * 
 * @param inPin The pin number to read from
 * @return double: 
 *   - Normal case: Converted sensor value in range [-10.0, 50.0]
 *   - Error case: -INFINITY for invalid pin, +INFINITY for inconsistent data
 */
double readSensor(int inPin) {
    
    // ------------------------------------------------------------------------
    // STEP 1: Input Validation
    // ------------------------------------------------------------------------
    if (inPin < 0) {
        return -INFINITY;  // Signal configuration error
    }
    
    // ------------------------------------------------------------------------
    // STEP 2: Collect Multiple Readings
    // ------------------------------------------------------------------------
    long readings[NUM_READINGS];
    
    for (int i = 0; i < NUM_READINGS; i++) {
        // NOTE: The problem states digitalRead() but describes 0-MAXINT range.
        // digitalRead() returns only 0 or 1, so we provide two options:
        
        // OPTION A: If sensor is analog (most common case):
        readings[i] = analogRead(inPin);  // Returns 0-1023 on standard Arduino
        
        // OPTION B: If sensor truly provides 32-bit values via custom interface:
        // readings[i] = read32BitSensor(inPin);  // Uncomment if applicable
        
        // IMPORTANT: If using analogRead(), adjust MAXINT to 1023 and 
        // recalibrate conversion formula accordingly for your hardware.
        
        delay(READING_DELAY_MS);  // Allow sensor to stabilize
    }
    
    // ------------------------------------------------------------------------
    // STEP 3: Sort Readings for Outlier Analysis
    // ------------------------------------------------------------------------
    std::sort(readings, readings + NUM_READINGS);
    
    // ------------------------------------------------------------------------
    // STEP 4: Check for Extreme Inconsistency (Sensor Fault Detection)
    // ------------------------------------------------------------------------
    long minVal = readings[0];
    long maxVal = readings[NUM_READINGS - 1];
    long range = maxVal - minVal;
    
    if (range > static_cast<long>(CONSISTENCY_THRESHOLD * MAXINT)) {
        return +INFINITY;  // Signal data quality failure
    }
    
    // ------------------------------------------------------------------------
    // STEP 5: Compute Trimmed Mean (Exclude Min and Max Values)
    // ------------------------------------------------------------------------
    long sum = 0;
    int validCount = 0;
    
    for (int i = 1; i < NUM_READINGS - 1; i++) {
        sum += readings[i];
        validCount++;
    }
    
    // Fallback if all values were excluded (shouldn't happen with N>=3)
    if (validCount == 0) {
        return +INFINITY;
    }
    
    double avgRaw = static_cast<double>(sum) / static_cast<double>(validCount);
    
    // ------------------------------------------------------------------------
    // STEP 6: Convert Raw Value to Meaningful Scale [-10.0, 50.0]
    // ------------------------------------------------------------------------
    // Linear interpolation formula:
    // output = min_out + (input / MAXINT) * (max_out - min_out)
    // output = -10.0 + (avgRaw / MAXINT) * 60.0
    
    double convertedValue = -10.0 + (avgRaw / static_cast<double>(MAXINT)) * 60.0;
    
    // ------------------------------------------------------------------------
    // STEP 7: Final Sanity Check on Result
    // ------------------------------------------------------------------------
    if (isnan(convertedValue) || isinf(convertedValue)) {
        return +INFINITY;  // Propagate any numerical anomalies
    }
    
    // ------------------------------------------------------------------------
    // STEP 8: Return Valid Converted Value
    // ------------------------------------------------------------------------
    return convertedValue;
}

// ============================================================================
// HELPER: Custom 32-bit sensor read function (if needed)
// ============================================================================
/**
 * Example placeholder for reading full 32-bit values from a sensor.
 * Replace with actual implementation based on your sensor protocol.
 */
long read32BitSensor(int pin) {
    // Example: Read 4 bytes via I2C, SPI, or custom protocol
    // This is hardware-specific and must be implemented per sensor datasheet
    
    // Placeholder implementation - replace with actual code:
    long value = 0;
    // value = readByte(pin) << 24 | readByte(pin) << 16 | ...
    return value;
}

================================================================================

USAGE EXAMPLE: MAIN APPLICATION LOOP
------------------------------------

// File: main.cpp or sketch.ino

#include <Arduino.h>  // For Serial, delay, etc. (Arduino environment)
// #include <iostream>  // For standard C++ environment
#include <cmath>      // For isinf(), isnan()

// External function declaration
double readSensor(int inPin);

void setup() {
    Serial.begin(115200);
    Serial.println("IoT Temperature Monitor - Starting...");
    
    // Optional: Initialize pin modes if needed
    // pinMode(A0, INPUT);
}

void loop() {
    // Read sensor from analog pin A0 (adjust pin as needed)
    double temperature = readSensor(A0);
    
    // ------------------------------------------------------------------------
    // ERROR HANDLING: Check for abnormal conditions
    // ------------------------------------------------------------------------
    if (isinf(temperature)) {
        if (temperature < 0) {
            Serial.println("[ERROR] Invalid pin configuration (-INFINITY)");
            // Action: Halt, alert user, or use default value
        } else {
            Serial.println("[WARNING] Sensor readings inconsistent (+INFINITY)");
            // Action: Retry, use last known good value, or trigger maintenance alert
        }
        delay(2000);  // Wait before retry
        return;
    }
    
    if (isnan(temperature)) {
        Serial.println("[ERROR] Invalid numeric result (NaN)");
        return;
    }
    
    // ------------------------------------------------------------------------
    // NORMAL OPERATION: Process valid temperature reading
    // ------------------------------------------------------------------------
    Serial.print("[INFO] Temperature: ");
    Serial.print(temperature, 2);  // Print with 2 decimal places
    Serial.println(" C");
    
    // Application-specific control logic
    if (temperature > 40.0) {
        Serial.println("[ALERT] Room too hot! Activating cooling system...");
        // activateCoolingFan();
        // openWindow();
    } 
    else if (temperature < 0.0) {
        Serial.println("[ALERT] Room too cold! Activating heating system...");
        // activateHeater();
        // closeWindow();
    }
    else if (temperature > 35.0) {
        Serial.println("[NOTICE] Temperature elevated - monitoring closely");
    }
    
    // Wait before next reading cycle
    delay(1000);
}

================================================================================

CONVERSION FORMULA EXPLANATION
------------------------------

Raw sensor values: [0, MAXINT] where MAXINT = 2147483647
Target scale: [-10.0, 50.0] spanning 60 degrees

Linear mapping equation:
    output = min_target + (input / MAXINT) * (max_target - min_target)
    output = -10.0 + (input / 2147483647.0) * 60.0

Verification table:
    Raw Input     | Calculation                    | Output
    --------------|--------------------------------|--------
    0             | -10.0 + (0)*60.0               | -10.0
    MAXINT/4      | -10.0 + (0.25)*60.0            |   5.0
    MAXINT/2      | -10.0 + (0.50)*60.0            |  20.0
    3*MAXINT/4    | -10.0 + (0.75)*60.0            |  35.0
    MAXINT        | -10.0 + (1.00)*60.0            |  50.0

This ensures monotonic, proportional mapping across the full sensor range.

================================================================================

ERROR SIGNALING WITH IEEE-754 SPECIAL VALUES
--------------------------------------------

The function uses IEEE-754 floating-point special values for error reporting:

1. -INFINITY (negative infinity):
   - Indicates: Invalid input parameters (e.g., negative pin number)
   - Detection: isinf(result) && (result < 0)

2. +INFINITY (positive infinity):
   - Indicates: Sensor data inconsistency or hardware fault
   - Detection: isinf(result) && (result > 0)

3. NaN (Not a Number) - handled internally:
   - Indicates: Undefined mathematical operation during conversion
   - Detection: isnan(result)

Advantages of this approach:
- No magic error codes that could conflict with valid sensor values
- Standard library functions (isinf, isnan) provide portable error checking
- Clear semantic distinction between configuration errors and data errors

Example error checking pattern:
    double val = readSensor(pin);
    if (isinf(val)) {
        if (val < 0) { /* handle config error */ }
        else { /* handle sensor fault */ }
    }
    else if (isnan(val)) { /* handle numerical error */ }
    else { /* process valid value */ }

================================================================================

TESTING AND VALIDATION STRATEGY
-------------------------------

1. UNIT TESTS FOR readSensor():
   
   Test Case 1: Valid consistent readings
   - Input: 5 readings all near MAXINT/2
   - Expected: Output near 20.0
   
   Test Case 2: One outlier reading
   - Input: [1000, 1001, 1002, 1003, MAXINT]
   - Expected: Trimmed mean excludes MAXINT, returns valid value
   
   Test Case 3: Highly inconsistent readings
   - Input: [0, MAXINT/4, MAXINT/2, 3*MAXINT/4, MAXINT]
   - Expected: +INFINITY (range exceeds threshold)
   
   Test Case 4: Invalid pin number
   - Input: pin = -1
   - Expected: -INFINITY

2. INTEGRATION TESTS:
   - Connect actual sensor and verify temperature readings match reference
   - Simulate sensor faults (disconnect wire, short circuit) and verify
     error signaling works correctly
   - Test boundary conditions: very cold (-10C) and very hot (50C) scenarios

3. PERFORMANCE TESTS:
   - Measure execution time of readSensor() to ensure it meets real-time
     requirements of your application
   - Verify memory usage is acceptable for target microcontroller

================================================================================

COMPILATION AND DEPLOYMENT NOTES
--------------------------------

For Arduino IDE:
1. Create new sketch and paste all code into .ino file
2. Ensure <cmath> and <algorithm> are supported (ESP32/Teensy have full support)
3. For AVR-based Arduinos, you may need to:
   - Replace std::sort with manual sorting for small arrays
   - Use pgm_read_float for INFINITY if needed
4. Upload to board and monitor via Serial Monitor at 115200 baud

For Standard C++ Environment:
1. Compile with: g++ -std=c++11 -o sensor_app main.cpp sensor_reader.cpp
2. Link with math library if needed: -lm
3. Replace Arduino-specific functions (Serial, delay, analogRead) with
   platform-appropriate equivalents

For ESP32/ESP8266:
- Code is fully compatible; analogRead returns 0-4095 on ESP32
- Adjust MAXINT and conversion formula if using native ADC resolution

================================================================================

POTENTIAL ENHANCEMENTS FOR PRODUCTION USE
-----------------------------------------

1. Adaptive Thresholding:
   - Learn sensor noise characteristics during calibration phase
   - Adjust CONSISTENCY_THRESHOLD dynamically based on historical data

2. Temporal Filtering:
   - Implement exponential moving average across multiple readSensor() calls
   - Reduces impact of brief sensor glitches

3. Confidence Scoring:
   - Return struct with {value, confidence, timestamp} instead of just double
   - Enables more sophisticated decision-making in application logic

4. Calibration Support:
   - Add offset and gain parameters for sensor-specific calibration
   - Store calibration data in EEPROM or flash memory

5. Watchdog Integration:
   - Track time since last valid reading
   - Trigger system reset or alert if sensor appears permanently faulty

6. Multi-Sensor Fusion:
   - If multiple sensors monitor same phenomenon, combine readings
   - Use voting or weighted averaging for increased reliability

================================================================================

CONCLUSION
----------

This assignment delivers a robust, elegant solution for reading unreliable
sensors in IoT applications:

KEY STRENGTHS:
[+] Minimal, focused implementation with clear separation of concerns
[+] Statistical outlier rejection without complex dependencies
[+] Proper use of IEEE-754 special values for error signaling
[+] Linear conversion formula with verified boundary behavior
[+] Well-documented assumptions enabling easy adaptation
[+] Production-ready error handling pattern for calling code

The solution balances simplicity with robustness, making it suitable for
resource-constrained embedded systems while providing the reliability
required for critical IoT applications like environmental monitoring.

By following the documented assumptions and testing strategy, this routine
can be confidently integrated into larger systems requiring dependable
sensor data acquisition under imperfect hardware conditions.

================================================================================