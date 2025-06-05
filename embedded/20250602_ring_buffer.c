/*

Question : Memory-Constrained Circular Buffer for IoT Sensor Hub
Scenario :
You're designing firmware for a Google Nest environmental sensor hub that monitors temperature, humidity, air quality, and light levels in a smart home. The device uses a low-power ARM Cortex-M0 + microcontroller with severe memory constraints:

Total RAM: 2KB (2048 bytes)
Stack + OS overhead: ~800 bytes
Available for application: ~1200 bytes
Sensor reading frequency: Every 500ms
Data transmission to cloud: Every 30 seconds (when WiFi is available)

Each sensor reading contains:
typedef struct {
    uint32_t timestamp;     // 4 bytes - Unix timestamp
    uint16_t temperature;   // 2 bytes - Temperature in 0.1°C units
    uint16_t humidity;      // 2 bytes - Humidity in 0.1% units  
    uint16_t air_quality;   // 2 bytes - Air quality index
    uint16_t light_level;   // 2 bytes - Light level in lux
    uint8_t  battery_level; // 1 byte  - Battery percentage
    uint8_t  sensor_status; // 1 byte  - Status flags
} sensor_reading_t;         // Total: 14 bytes per reading (size : 16 bytes with alignment)


The Problem
The device needs to buffer sensor readings when WiFi is temporarily unavailable. In worst-case scenarios, WiFi might be down for up to 5 minutes. During this time, the device must:

Continue collecting sensor data every 500ms
Store readings in a circular buffer without losing critical data
Handle memory exhaustion gracefully
Maintain system stability (no crashes or memory corruption)
Implement efficient read/write operations suitable for interrupt contexts

Requirements
Part A: Basic Implementation (30 minutes)

Implement a circular buffer with the following interface:
typedef struct {
    // Your implementation here
} circular_buffer_t;

// Initialize the buffer with given memory constraints
int cb_init(circular_buffer_t* cb, size_t max_memory_bytes);

// Add a sensor reading (called from timer interrupt)
int cb_push(circular_buffer_t* cb, const sensor_reading_t* reading);

// Remove oldest reading (called from main loop)
int cb_pop(circular_buffer_t* cb, sensor_reading_t* reading);

// Check buffer status
bool cb_is_empty(const circular_buffer_t* cb);
bool cb_is_full(const circular_buffer_t* cb);
size_t cb_count(const circular_buffer_t* cb);
size_t cb_available_space(const circular_buffer_t* cb);

// Cleanup
void cb_destroy(circular_buffer_t* cb);


Part B: Advanced Features (20 minutes)
Implement additional functionality:

// Peek at the oldest reading without removing it
int cb_peek(const circular_buffer_t* cb, sensor_reading_t* reading);

// Get readings in batch for efficient transmission
int cb_get_batch(circular_buffer_t* cb, sensor_reading_t* readings, 
                 size_t max_count, size_t* actual_count);

// Clear all readings (emergency reset)
void cb_clear(circular_buffer_t* cb);


Part C: Critical Scenarios (10 minutes)
Handle these edge cases:

Power brownout: What happens if power drops during a write operation?
Memory corruption detection: How would you detect if the buffer structure is corrupted?
Interrupt safety: Ensure thread safety between timer interrupt (producer) and main loop (consumer)

Constraints & Considerations

Memory Budget: Your entire buffer implementation must fit within 512 bytes of RAM
Performance: Push/pop operations should be O(1) and suitable for interrupt context
Reliability: No dynamic memory allocation allowed (malloc/free forbidden)
Power Efficiency: Minimize CPU cycles for battery life
Data Integrity: Implement basic corruption detection

Expected Deliverables

Complete C implementation with proper error handling
Memory usage calculation and justification
Discussion of design trade-offs
Test cases for edge conditions
Brief explanation of how your solution handles the 5-minute WiFi outage scenario

Evaluation Criteria

Correctness: Does the implementation work as specified?
Memory Efficiency: How well does it use the limited RAM?
Robustness: How does it handle error conditions and edge cases?
Code Quality: Is the code readable, maintainable, and well-documented?
Embedded Systems Thinking: Does the candidate understand microcontroller constraints?

Bonus Points

Implement wear leveling for flash storage backup
Add compression for sensor data
Design a recovery mechanism after power loss
Optimize for specific ARM Cortex-M instruction set features

*/





// My attempt and correction for Question A

typedef struct {
    uint32_t timestamp;     // 4 bytes - Unix timestamp
    uint16_t temperature;   // 2 bytes - Temperature in 0.1°C units
    uint16_t humidity;      // 2 bytes - Humidity in 0.1% units  
    uint16_t air_quality;   // 2 bytes - Air quality index
    uint16_t light_level;   // 2 bytes - Light level in lux
    uint8_t  battery_level; // 1 byte  - Battery percentage
    uint8_t  sensor_status; // 1 byte  - Status flags
} sensor_reading_t;         // Total: 14 bytes per reading (size : 16 bytes with alignment)

typedef struct {
    // Your implementation here
    sensor_reading_t* buffer;       // Pointer to buffer storage
    volatile uint16_t head;         // Write position (volatile for ISR safety)
    volatile uint16_t tail;         // Read position
    uint16_t capacity;              // Maximum number of readings
    volatile uint16_t count;        // Current number of readings
    
} circular_buffer_t;

// Error codes
#define CB_SUCCESS           0
#define CB_ERROR_NULL_PTR   -1
#define CB_ERROR_NO_MEMORY  -2
#define CB_ERROR_FULL       -3
#define CB_ERROR_EMPTY      -4
#define CB_ERROR_CORRUPTED  -5



// Calculate maximum capacity given memory budget
uint16_t calculate_max_capacity(size_t max_memory_bytes) {
    size_t overhead = sizeof(circular_buffer_t);
    if (max_memory_bytes <= overhead) {
        return 0;
    }
    
    size_t available_memory = max_memory_bytes - overhead;
    return (uint16_t)(available_memory / sizeof(sensor_reading_t));
}


// Initialize the circular buffer
int cb_init(circular_buffer_t* cb, size_t max_memory_bytes) {
    if (!cb) {
        return CB_ERROR_NULL_PTR;
    }

    // Calculate maximum capacity within memory budget
    uint16_t max_capacity = calculate_max_capacity(max_memory_bytes);
    if (max_capacity == 0) {
        return CB_ERROR_NO_MEMORY;
    }

    // allocate the buffer
    cb->buffer = (sensor_reading_t *) malloc(sizeof(sensor_reading_t)*max_capacity);
    if (!cb->buffer) {
        return CB_ERROR_NO_MEMORY;
    }

    // initialize circular buffer
    cb->count = 0;
    cb->tail = 0;
    cb->head = 0;    
    cb->capacity = max_capacity;

    return CB_SUCCESS;
}



int cb_push(circular_buffer_t* cb, const sensor_reading_t* reading){
    // check whether cb and reading is null
    if ( !cb || !reading ){
        return CB_ERROR_NULL_PTR;
    }

    // return error if queue is full
    if ( cb_is_full(cb) ){
        return CB_ERROR_FULL;
    }

    // add the reading
    cb->buffer [cb->head] = *reading;
    cb->head  = (cb->head + 1)%cb->capacity;
    cb->count ++;

    return CB_SUCCESS;
}

// Remove oldest reading (called from main loop)
int cb_pop(circular_buffer_t* cb, sensor_reading_t* reading){
    // check whether cb and reading is null
    if ( !cb || !reading ){
        return CB_ERROR_NULL_PTR;
    }

    // return error if queue is empty
    if ( cb_is_empty(cb) ){
        return CB_ERROR_EMPTY;
    }

    // pop
    cb->tail = (cb->tail + 1)%cb->capacity;;
    cb->count --;

    return CB_SUCCESS;
}

// Check buffer status
bool cb_is_empty(const circular_buffer_t* cb){
    // May check what is the output if cb is NULL

    return ( cb->count == 0 );
}

bool cb_is_full(const circular_buffer_t* cb){
    return ( cb->count == cb->capacity );
}


size_t cb_count(const circular_buffer_t* cb){
    if (!cb) {
        return 0;
    }
    return cb->count;
}

size_t cb_available_space(const circular_buffer_t* cb){
    if (!cb) {
        return 0;
    }
    return (cb->capacity - cb->count);
}

// Cleanup
void cb_destroy(circular_buffer_t* cb){
    if (!cb){
      return;  
    }  

    // free the buffer, remember to set the pointer to NULL after free the buffer
    if (cb->buffer) {
        free(cb->buffer);
        cb->buffer = NULL;
    }

    cb->head = 0;
    cb->read_index = 0;
    cb->count = 0;
    cb->capacity = 0;

}






// Total solution

typedef struct {
    uint32_t timestamp;     // 4 bytes - Unix timestamp
    uint16_t temperature;   // 2 bytes - Temperature in 0.1°C units
    uint16_t humidity;      // 2 bytes - Humidity in 0.1% units  
    uint16_t air_quality;   // 2 bytes - Air quality index
    uint16_t light_level;   // 2 bytes - Light level in lux
    uint8_t  battery_level; // 1 byte  - Battery percentage
    uint8_t  sensor_status; // 1 byte  - Status flags
} sensor_reading_t;         // Total: 14 bytes per reading (size : 16 bytes with alignment)

typedef struct {
    // Your implementation here
    sensor_reading_t* buffer;       // Pointer to buffer storage
    volatile uint16_t write_index;  // Write position (volatile for ISR safety)
    volatile uint16_t read_index;   // Read position
    uint16_t capacity;              // Maximum number of readings
    volatile uint16_t count;        // Current number of readings
    
    // Statistics (for monitoring buffer health)
    uint32_t total_writes;          // Total readings written
    uint32_t total_reads;           // Total readings read
    uint16_t max_usage;             // Peak buffer usage
    uint16_t overrun_count;         // Number of overruns (data loss)
    
    // Corruption detection
    uint32_t magic_start;           // Start magic number (0xDEADBEEF)
    uint32_t magic_end;             // End magic number (0xCAFEBABE)
} circular_buffer_t;

// Magic numbers for corruption detection
#define CB_MAGIC_START 0xDEADBEEF
#define CB_MAGIC_END   0xCAFEBABE

// Error codes
#define CB_SUCCESS           0
#define CB_ERROR_NULL_PTR   -1
#define CB_ERROR_NO_MEMORY  -2
#define CB_ERROR_FULL       -3
#define CB_ERROR_EMPTY      -4
#define CB_ERROR_CORRUPTED  -5


// Calculate maximum capacity given memory budget
static uint16_t calculate_max_capacity(size_t max_memory_bytes) {
    size_t overhead = sizeof(circular_buffer_t);
    if (max_memory_bytes <= overhead) {
        return 0;
    }
    
    size_t available_memory = max_memory_bytes - overhead;
    return (uint16_t)(available_memory / sizeof(sensor_reading_t));
}

// Check buffer structure integrity
static bool check_buffer_integrity(const circular_buffer_t* cb) {
    if (!cb) return false;
    
    return (cb->magic_start == CB_MAGIC_START && 
            cb->magic_end == CB_MAGIC_END &&
            cb->write_index < cb->capacity &&
            cb->read_index < cb->capacity &&
            cb->count <= cb->capacity);
}

// Initialize the circular buffer
int cb_init(circular_buffer_t* cb, size_t max_memory_bytes) {
    if (!cb) {
        return CB_ERROR_NULL_PTR;
    }
    
    // Calculate maximum capacity within memory budget
    uint16_t max_capacity = calculate_max_capacity(max_memory_bytes);
    if (max_capacity == 0) {
        return CB_ERROR_NO_MEMORY;
    }
    
    // For 512 bytes budget:
    // - circular_buffer_t: ~48 bytes
    // - Available for readings: 464 bytes
    // - Max readings: 464/14 = 33 readings (using packed struct)
    // This covers ~16.5 seconds of data (33 * 0.5s), not full 5 minutes
    // For 5 minutes, we'd need ~1680 bytes total
    
    // Initialize buffer structure
    memset(cb, 0, sizeof(circular_buffer_t));
    cb->capacity = max_capacity;
    cb->write_index = 0;
    cb->read_index = 0;
    cb->count = 0;
    
    // Set magic numbers for corruption detection
    cb->magic_start = CB_MAGIC_START;
    cb->magic_end = CB_MAGIC_END;
    
    // Allocate buffer storage (in real embedded system, this would be static)
    // For interview purposes, showing the concept
    cb->buffer = (sensor_reading_t*)((uint8_t*)cb + sizeof(circular_buffer_t));
    
    return CB_SUCCESS;
}

// Add a sensor reading (interrupt-safe)
int cb_push(circular_buffer_t* cb, const sensor_reading_t* reading) {
    if (!cb || !reading) {
        return CB_ERROR_NULL_PTR;
    }
    
    // Check buffer integrity
    if (!check_buffer_integrity(cb)) {
        return CB_ERROR_CORRUPTED;
    }
    
    // Critical section - disable interrupts in real implementation
    // __disable_irq();
    
    bool was_full = (cb->count == cb->capacity);
    
    // Write the reading
    cb->buffer[cb->write_index] = *reading;
    
    // Update write index (wrap around)
    cb->write_index = (cb->write_index + 1) % cb->capacity;
    
    if (was_full) {
        // Buffer overflow - overwrite oldest data
        cb->read_index = (cb->read_index + 1) % cb->capacity;
        cb->overrun_count++;
    } else {
        cb->count++;
    }
    
    // Update statistics
    cb->total_writes++;
    if (cb->count > cb->max_usage) {
        cb->max_usage = cb->count;
    }
    
    // __enable_irq();
    
    return was_full ? CB_ERROR_FULL : CB_SUCCESS;
}

// Remove oldest reading
int cb_pop(circular_buffer_t* cb, sensor_reading_t* reading) {
    if (!cb) {
        return CB_ERROR_NULL_PTR;
    }
    
    // Check buffer integrity
    if (!check_buffer_integrity(cb)) {
        return CB_ERROR_CORRUPTED;
    }
    
    // Critical section
    // __disable_irq();
    
    if (cb->count == 0) {
        // __enable_irq();
        return CB_ERROR_EMPTY;
    }
    
    // Read the data if requested
    if (reading) {
        *reading = cb->buffer[cb->read_index];
    }
    
    // Update read index
    cb->read_index = (cb->read_index + 1) % cb->capacity;
    cb->count--;
    cb->total_reads++;
    
    // __enable_irq();
    
    return CB_SUCCESS;
}

// Check if buffer is empty
bool cb_is_empty(const circular_buffer_t* cb) {
    if (!cb || !check_buffer_integrity(cb)) {
        return true; // Treat corruption as empty for safety
    }
    return (cb->count == 0);
}

// Check if buffer is full
bool cb_is_full(const circular_buffer_t* cb) {
    if (!cb || !check_buffer_integrity(cb)) {
        return true; // Treat corruption as full for safety
    }
    return (cb->count == cb->capacity);
}

// Get current count
size_t cb_count(const circular_buffer_t* cb) {
    if (!cb || !check_buffer_integrity(cb)) {
        return 0;
    }
    return cb->count;
}

// Get available space
size_t cb_available_space(const circular_buffer_t* cb) {
    if (!cb || !check_buffer_integrity(cb)) {
        return 0;
    }
    return (cb->capacity - cb->count);
}

// Peek at oldest reading without removing
int cb_peek(const circular_buffer_t* cb, sensor_reading_t* reading) {
    if (!cb || !reading) {
        return CB_ERROR_NULL_PTR;
    }
    
    if (!check_buffer_integrity(cb)) {
        return CB_ERROR_CORRUPTED;
    }
    
    if (cb->count == 0) {
        return CB_ERROR_EMPTY;
    }
    
    *reading = cb->buffer[cb->read_index];
    return CB_SUCCESS;
}

// Get multiple readings efficiently
int cb_get_batch(circular_buffer_t* cb, sensor_reading_t* readings, 
                 size_t max_count, size_t* actual_count) {
    if (!cb || !readings || !actual_count) {
        return CB_ERROR_NULL_PTR;
    }
    
    if (!check_buffer_integrity(cb)) {
        return CB_ERROR_CORRUPTED;
    }
    
    *actual_count = 0;
    
    // Critical section
    // __disable_irq();
    
    size_t available = cb->count;
    size_t to_read = (max_count < available) ? max_count : available;
    
    for (size_t i = 0; i < to_read; i++) {
        readings[i] = cb->buffer[cb->read_index];
        cb->read_index = (cb->read_index + 1) % cb->capacity;
        cb->count--;
        cb->total_reads++;
    }
    
    *actual_count = to_read;
    
    // __enable_irq();
    
    return CB_SUCCESS;
}

// Clear all readings
void cb_clear(circular_buffer_t* cb) {
    if (!cb || !check_buffer_integrity(cb)) {
        return;
    }
    
    // Critical section
    // __disable_irq();
    
    cb->write_index = 0;
    cb->read_index = 0;
    cb->count = 0;
    
    // __enable_irq();
}

// Cleanup (no dynamic allocation in this implementation)
void cb_destroy(circular_buffer_t* cb) {
    if (cb) {
        // Clear magic numbers to prevent accidental reuse
        cb->magic_start = 0;
        cb->magic_end = 0;
        memset(cb, 0, sizeof(circular_buffer_t));
    }
}

/*
 * MEMORY USAGE ANALYSIS:
 * 
 * Structure size: sizeof(circular_buffer_t) = ~48 bytes
 * Each reading: sizeof(sensor_reading_t) = 14 bytes (packed)
 * 
 * For 512-byte budget:
 * - Buffer structure: 48 bytes
 * - Available for data: 464 bytes
 * - Max readings: 464/14 = 33 readings
 * - Time coverage: 33 * 0.5s = 16.5 seconds
 * 
 * For 5-minute coverage (600 readings):
 * - Need: 48 + (600 * 14) = 8448 bytes
 * - This exceeds our 512-byte constraint significantly
 * 
 * TRADE-OFFS:
 * 1. Could compress data (temperature differences, etc.)
 * 2. Could reduce reading frequency during WiFi outages
 * 3. Could use external storage (flash) for overflow
 * 4. Could prioritize certain sensor types over others
 */

/*
 * EXAMPLE USAGE FOR NEST SENSOR HUB:
 */

// Static allocation for embedded system
static uint8_t buffer_memory[512] __attribute__((aligned(4)));
static circular_buffer_t* sensor_buffer;

// Initialize sensor buffer system
int init_sensor_buffer_system(void) {
    sensor_buffer = (circular_buffer_t*)buffer_memory;
    return cb_init(sensor_buffer, sizeof(buffer_memory));
}

// Called from timer interrupt every 500ms
void sensor_timer_callback(void) {
    sensor_reading_t reading;
    
    // Collect sensor data
    reading.timestamp = get_current_timestamp();
    reading.temperature = read_temperature_sensor();
    reading.humidity = read_humidity_sensor();
    reading.air_quality = read_air_quality_sensor();
    reading.light_level = read_light_sensor();
    reading.battery_level = read_battery_level();
    reading.sensor_status = get_sensor_status();
    
    // Store in buffer (handles overflow automatically)
    cb_push(sensor_buffer, &reading);
}

// Called from main loop when WiFi is available
void transmit_sensor_data(void) {
    sensor_reading_t batch[10]; // Transmit in batches of 10
    size_t count;
    
    while (!cb_is_empty(sensor_buffer)) {
        if (cb_get_batch(sensor_buffer, batch, 10, &count) == CB_SUCCESS) {
            // Transmit batch to cloud
            if (transmit_to_cloud(batch, count) != 0) {
                // Transmission failed - put data back? Or drop?
                // For this implementation, we drop failed transmissions
                break;
            }
        }
    }
}

/*
 * CRITICAL SCENARIOS HANDLING:
 * 
 * 1. Power Brownout:
 *    - Use magic numbers to detect corruption on startup
 *    - Could add checksum for additional validation
 *    - Clear buffer if corruption detected
 * 
 * 2. Memory Corruption Detection:
 *    - Magic number validation in all operations
 *    - Boundary checking on indices
 *    - Return error codes for corruption
 * 
 * 3. Interrupt Safety:
 *    - Volatile keywords on shared variables
 *    - Critical sections for multi-step operations
 *    - Atomic operations where possible
 * 
 * 4. 5-Minute WiFi Outage:
 *    - Current implementation handles ~16.5 seconds
 *    - Would need external storage or larger buffer for full requirement
 *    - Could implement tiered storage (RAM + Flash)
 */