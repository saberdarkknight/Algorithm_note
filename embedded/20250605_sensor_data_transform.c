/*

Data Transformation Function Problem Statement

Overview
Implement a data transformation function that converts sensor data from a defined input format to another format that can be used by another system. Upon successful conversion, outputDataValues is updated and STATUS_SUCCESS is returned.


Background

Input Format
A properly formatted set of input data to transform consists of a set of 9 bytes. 
The first byte is a bit mask (Sensor Value Bit Mask), indicating which sensors in the input have valid data. 
The remaining 8 bytes are the sensor values. For instance, if bit zero of the 'Sensor Value Bit Mask' is one, then 'Sensor Value 0' at byte 1 of the input is valid and contains a sensor reading.

Output Format
A properly formatted set of output data consists of 33 bytes. 
The first byte is a count of the number of sensors that have their corresponding 'Sensor Valid Bit Mask' bit set. 
The sensor values themselves are 32-bit integers in little-endian format. 
The 'Sensor Value' in the output should be set to zero if the corresponding 'Sensor Value Bit Mask' bit is zero. 
Otherwise, the sensor value output is calculated using the following formula:
SensorValue_n(Output) = SensorValue_n(Input) * 2^n
where 'n' is the sensor index from 0 to 7.

Special Cases
If there are no sensors with valid data in the input, the function should return STATUS_NO_SENSOR_DATA and not update the 'outputDataValues'.
Host System Requirements
Note: Assume the host is in little-endian along with the examples listed below.
Data Format Visualization
Input Format (9 bytes total):

Byte 0: Sensor Valid Bit Mask
Byte 1: Sensor Value 0
Byte 2: Sensor Value 1
Byte 3: Sensor Value 2
Byte 4: Sensor Value 3
Byte 5: Sensor Value 4
Byte 6: Sensor Value 5
Byte 7: Sensor Value 6
Byte 8: Sensor Value 7


Transform Process:
The bit mask determines which sensor values are valid:

Bit 7: Sensor Value 7 Valid
Bit 1: Sensor Value 1 Valid, SensorValue 0 Valid
Bit 0: Valid

Output Format (33 bytes total):

Byte 0: Number of Valid Sensors
Bytes 1-4: Sensor Value 0 * 1
Bytes 5-8: Sensor Value 1 * 2
Bytes 9-12: Sensor Value 2 * 4
Bytes 13-16: Sensor Value 3 * 8
Bytes 17-20: Sensor Value 4 * 16
Bytes 21-24: Sensor Value 5 * 32
Bytes 25-28: Sensor Value 6 * 64
Bytes 29-32: Sensor Value 7 * 128

Function Signature
FuncStatus_t TransformData(const size_t inputDataLength, const uint8_t inputDataValues[], 
                          const size_t outputDataLength, uint8_t outputDataValues[])

Input Parameters

inputDataLength: The number of bytes in the input data
inputDataValues[]: Array containing the input data
outputDataLength: The number of bytes available for output data
outputDataValues[]: Array to store the transformed output data

Return Values

STATUS_SUCCESS: Transformation completed successfully
STATUS_NO_SENSOR_DATA: No valid sensor data found in input

Requirements Summary

Parse the 9-byte input format with bit mask and sensor values
Generate a 33-byte output format with count and transformed values
Apply the transformation formula: SensorValue_n(Output) = SensorValue_n(Input) * 2^n
Handle sensors marked as invalid by setting their output values to zero
Count only valid sensors in the first output byte
Use little-endian format for 32-bit integer outputs
Return appropriate status codes based on the presence of valid data



Input Format                Transform                     Output Format
┌──────────────┐              ─────►                  ┌─────────────────────┐
│              │ ┌─  Bit 7 ─ Sensor Value 7 Valid     │                     │
│   Byte 0     │ |                                    │   Byte 0            │
│ Sensor Valid │ │ ┌─ Bit 1 ─ Sensor Value 1 Valid    │ Number of Valid     │
│  Bit Mask    │ │ | ┌─   Bit 0 ─ SensorValue 0 Valid │   Sensors           │
│              │ │ | |                                │                     │  
├──────────────┤ │ │ |                                ├─────────────────────┤
│   Byte 1     │ │ │ |                                │   Bytes 1-4         │  
│Sensor Value 0│ │ │ │                                │ Sensor Value 0 * 1  │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 2     │ │ │ │                                │   Bytes 5-8         │
│Sensor Value 1│ │ │ │                                │ Sensor Value 1 * 2  │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 3     │ │ │ │                                │   Bytes 9-12        │
│Sensor Value 2│ │ │ │                                │ Sensor Value 2 * 4  │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 4     │ │ │ │                                │   Bytes 13-16       │
│Sensor Value 3│ │ │ │                                │ Sensor Value 3 * 8  │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 5     │ │ │ │                                │   Bytes 17-20       │
│Sensor Value 4│ │ │ │                                │ Sensor Value 4 * 16 │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 6     │ │ │ │                                │   Bytes 21-24       │
│Sensor Value 5│ │ │ │                                │ Sensor Value 5 * 32 │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 7     │ │ │ │                                │   Bytes 25-28       │
│Sensor Value 6│ │ │ │                                │ Sensor Value 6 * 64 │
├──────────────┤ │ │ │                                ├─────────────────────┤
│   Byte 8     │ │ │ │                                │   Bytes 29-32       │
│Sensor Value 7│ │ │ │                                │Sensor Value 7 * 128 │
└──────────────┘ │ │ │                                └─────────────────────┘
                 │ │ │
                 └─┼─┼─ Valid bits determine which
                   └─┼─ sensor values are processed
                     └─ and included in output count




*/





// My attempt

// Define status codes
#define STATUS_SUCCESS 0
#define STATUS_NO_SENSOR_DATA 1

typedef uint8_t FuncStatus_t;
typedef size_t size_t;

FuncStatus_t TransformData(const size_t inputDataLength, const uint8_t inputDataValues[],
                          const size_t outputDataLength, uint8_t outputDataValues[])
{
    // Check if input data length is valid (should be 9 bytes)
    if (inputDataLength != 9) {
        return STATUS_NO_SENSOR_DATA;
    }
    
    // Check if output buffer is large enough (should be at least 33 bytes)
    if (outputDataLength < 33) {
        return STATUS_NO_SENSOR_DATA;
    }
    
    // Get the sensor value bit mask from the first byte
    uint8_t sensorValidBitMask = inputDataValues[0];
    
    // Check if there's any valid sensor data
    if (sensorValidBitMask == 0) {
        return STATUS_NO_SENSOR_DATA;
    }
    
    // Count the number of valid sensors
    uint8_t validSensorCount = 0;
    for (int i = 0; i < 8; i++) {
        if (sensorValidBitMask & (1 << i)) {
            validSensorCount++;
        }
    }
    
    // Set the first byte of output to the count of valid sensors
    outputDataValues[0] = validSensorCount;
    
    // Initialize all output values to 0
    for (int i = 1; i < 33; i++) {
        outputDataValues[i] = 0;
    }
    
    // Process each sensor
    for (int sensorIndex = 0; sensorIndex < 8; sensorIndex++) {
        // Check if this sensor has valid data
        if (sensorValidBitMask & (1 << sensorIndex)) {
            // Get the sensor input value from the appropriate byte (input bytes 1-8 correspond to sensor values 0-7)
            uint8_t sensorValue = inputDataValues[sensorIndex + 1];
            
            // Calculate the multiplier (2^n)
            uint32_t multiplier = 1 << sensorIndex; // This is 2^sensorIndex
            
            // Calculate the output value: sensorValue * 2^sensorIndex
            uint32_t outputValue = (uint32_t)sensorValue * multiplier;
            
            // Calculate the starting byte position for this sensor in the output
            int outputStartByte = 1 + (sensorIndex * 4);
            
            // Store the 32-bit value in little-endian format
            outputDataValues[outputStartByte]     = (uint8_t)(outputValue & 0xFF);
            outputDataValues[outputStartByte + 1] = (uint8_t)((outputValue >> 8) & 0xFF);
            outputDataValues[outputStartByte + 2] = (uint8_t)((outputValue >> 16) & 0xFF);
            outputDataValues[outputStartByte + 3] = (uint8_t)((outputValue >> 24) & 0xFF);
        }
        // If sensor is not valid, the corresponding output bytes remain 0
    }
    
    return STATUS_SUCCESS;
}