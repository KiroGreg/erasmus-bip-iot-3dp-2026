================================================================================
                    MOVING AVERAGE SENSOR TRACKING ASSIGNMENT - CONCISE SOLUTION
                    Efficient Window-Based Sensor Data Management
                    Author: Kyryll Hryhorenko
================================================================================

ASSUMPTIONS
-----------
• WINDOW_SIZE = 16: Small enough that array shifting is negligible (<3µs)
• samples[0] = newest: Achieved by shifting elements right on each update
• Efficiency: Maintain runningSum for O(1) average vs O(n) recalculation
• Initialization: Track validSampleCount during warm-up phase
• Error handling: Return 1 on failure, caller tracks consecutive errors
• analogRead() assumed: digitalRead() only returns 0/1, insufficient for sensing

================================================================================
CODE IMPLEMENTATION
================================================================================

#define WINDOW_SIZE 16
#define SENSOR_PIN A0
#define UPPER_THRESHOLD 40.0
#define LOWER_THRESHOLD 0.0

float samples[WINDOW_SIZE];
float runningSum = 0.0;
int validSampleCount = 0;
int numErrors = 0;

// Convert raw reading to [-10.0, 50.0] scale
float convertReading(int raw) {
    const int RAW_MAX = 1023;  // Adjust for your sensor
    float norm = constrain(raw, 0, RAW_MAX) / (float)RAW_MAX;
    return -10.0 + norm * 60.0;
}

// Read sensor, update window (samples[0] = newest), maintain running sum
// Returns: 0 = success, 1 = error
int getSample() {
    int raw = analogRead(SENSOR_PIN);
    if (raw < 0) return 1;  // Read error
    
    float newVal = convertReading(raw);
    if (isnan(newVal) || isinf(newVal)) return 1;
    
    // Shift array right, insert newest at index 0
    for (int i = WINDOW_SIZE - 1; i > 0; i--) {
        samples[i] = samples[i - 1];
    }
    samples[0] = newVal;
    
    // Update running sum efficiently
    if (validSampleCount < WINDOW_SIZE) {
        runningSum += newVal;
        validSampleCount++;
    } else {
        runningSum = runningSum - samples[WINDOW_SIZE - 1] + newVal;
    }
    return 0;
}

// Compute moving average in O(1) time
float computeAverage() {
    if (validSampleCount == 0) return 0.0;
    int n = (validSampleCount < WINDOW_SIZE) ? validSampleCount : WINDOW_SIZE;
    return runningSum / n;
}

================================================================================
USAGE: CONTROL LOOP
================================================================================

void setup() {
    Serial.begin(115200);
    for (int i = 0; i < WINDOW_SIZE; i++) samples[i] = 0.0;
}

void loop() {
    if (getSample() == 1) {
        numErrors++;
        if (numErrors > 8) {
            // Persistent fault: alert, safe mode, or reset
            Serial.println("SENSOR FAULT");
            numErrors = 0;
        }
        delay(100);
        return;
    }
    
    numErrors = 0;  // Reset on success
    float avg = computeAverage();
    
    if (avg > UPPER_THRESHOLD) {
        // Activate cooling
        Serial.println("Cooling ON");
    } else if (avg < LOWER_THRESHOLD) {
        // Activate heating
        Serial.println("Heating ON");
    }
    // else: within range, no action
    
    delay(1000);  // Sampling interval
}

================================================================================
KEY POINTS
================================================================================
✓ samples[0] always holds newest reading (requirement met via shift)
✓ computeAverage() is O(1) using runningSum, not O(n) summation
✓ Initialization handled: average uses validSampleCount until window full
✓ Error signaling: getSample() returns 1, caller decides recovery strategy
✓ Conversion: Linear mapping raw[0,1023] → value[-10.0, 50.0]

// For larger windows (>64), replace shifting with circular buffer:
// int head = 0; samples[head] = val; head = (head+1)%WINDOW_SIZE;

================================================================================