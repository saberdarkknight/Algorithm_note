/*

Problem Statement
You're designing firmware for a microcontroller that processes sensor data. The system has:

A timer interrupt that occurs every 1ms and reads sensor values
A main loop that processes these sensor readings
Limited RAM (only 512 bytes available for the queue)

Task: Implement an interrupt-safe circular queue that can safely handle concurrent access between interrupt context and main program context.
Requirements

Thread Safety: Queue must be safe for use between interrupt handlers and main code
Memory Efficient: Use a fixed-size circular buffer (no dynamic allocation)
Lock-Free: Cannot use mutexes/semaphores (common constraint in embedded systems)
Atomic Operations: Handle race conditions properly
Performance: Minimize interrupt disable time

typedef struct {
    uint16_t data;
    uint32_t timestamp;
} sensor_reading_t;

typedef struct {
    sensor_reading_t* buffer;
    volatile uint8_t head;
    volatile uint8_t tail;
    uint8_t size;
} interrupt_safe_queue_t;

// Initialize the queue
void queue_init(interrupt_safe_queue_t* q, sensor_reading_t* buffer, uint8_t size);

// Add item (called from interrupt context)
bool queue_enqueue(interrupt_safe_queue_t* q, const sensor_reading_t* item);

// Remove item (called from main loop)
bool queue_dequeue(interrupt_safe_queue_t* q, sensor_reading_t* item);

// Check if queue is empty (safe to call from both contexts)
bool queue_is_empty(const interrupt_safe_queue_t* q);

// Check if queue is full (safe to call from both contexts)
bool queue_is_full(const interrupt_safe_queue_t* q);



Example Usage Scenario
// Timer interrupt handler (1ms interval)
void timer_interrupt_handler(void) {
    sensor_reading_t reading;
    reading.data = read_sensor();
    reading.timestamp = get_system_time();
    
    if (!queue_enqueue(&sensor_queue, &reading)) {
        // Handle queue full error
        error_count++;
    }
}

// Main loop
int main(void) {
    sensor_reading_t buffer[64];  // Fixed size buffer
    interrupt_safe_queue_t sensor_queue;
    
    queue_init(&sensor_queue, buffer, 64);
    
    while (1) {
        sensor_reading_t reading;
        if (queue_dequeue(&sensor_queue, &reading)) {
            process_sensor_data(&reading);
        }
        // Other main loop tasks...
    }
}


Follow-up Questions
After implementing the solution, be prepared to discuss:

Race Conditions: What specific race conditions could occur and how did you prevent them?
Memory Barriers: On some architectures, would you need memory barriers? Why or why not?
Overflow Handling: How would you modify the design to handle queue overflow gracefully?
Performance Analysis: What's the worst-case interrupt disable time in your implementation?
Multi-Producer Scenario: How would you modify this for multiple interrupt sources writing to the same queue?
Power Considerations: How might this design impact power consumption in a battery-powered device?

Evaluation Criteria

Correctness: Handles all race conditions properly
Efficiency: Minimal critical sections and atomic operations
Code Quality: Clean, readable, well-commented code
Edge Case Handling: Proper handling of full/empty conditions
Understanding: Can explain the reasoning behind design choices


*/





// My attempt
typedef struct {
    uint16_t data;
    uint32_t timestamp;
} sensor_reading_t;

typedef struct {
    sensor_reading_t* buffer;
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
    uint8_t size;
} interrupt_safe_queue_t;


#define ENTER_CRITICAL_SECTION() uint32_t primask = __get_PRIMASK(); __disable_irq()
#define EXIT_CRITICAL_SECTION() __set_PRIMASK(primask)


// Initialize the queue
void queue_init(interrupt_safe_queue_t* q, sensor_reading_t* buffer, uint8_t size){
    if (!q || !buffer || size == 0) {
        return;
    }
    memset( q, 0, sizeof(interrupt_safe_queue_t) );
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->size = size;
    q->buffer = buffer;
}

// Add item (called from interrupt context)
bool queue_enqueue(interrupt_safe_queue_t* q, const sensor_reading_t* item){
    if (!q || !item || !q->buffer) {
        return false;
    }
    if( queue_is_full(q) ){
        return false;
    }
    ENTER_CRITICAL_SECTION();  // Disable interrupts
    q->buffer[q->tail] = *item;
    q->tail = ( q->tail ++)%q->size;
    q->count ++;
    EXIT_CRITICAL_SECTION();
    return true;
}

// Remove item (called from main loop)
bool queue_dequeue(interrupt_safe_queue_t* q, sensor_reading_t* item){
    if( !q || !q->buffer || !item ){
        return false;
    }
    if( queue_is_empty(q) ){
        return false;
    }
    ENTER_CRITICAL_SECTION();  // Disable interrupts
    *item = q->buffer[q->head];
    q->head = ( q->head ++)%q->size;
    q->count --;
    EXIT_CRITICAL_SECTION();
    return true;
}

// Check if queue is empty (safe to call from both contexts)
bool queue_is_empty(const interrupt_safe_queue_t* q){
    if( !q || !q->buffer ){
        return false;
    }
    return( q->count == 0 );
}

// Check if queue is full (safe to call from both contexts)
bool queue_is_full(const interrupt_safe_queue_t* q){
   if( !q || !q->buffer ){
        return false;
    }
    return( q->count == q->size );
}


/*

Race Conditions: What specific race conditions could occur and how did you prevent them?

1. Race Conditions: What specific race conditions could occur and how did you prevent them?
Potential Race Conditions:
Prevention: Our circular buffer size and single-producer design prevent tail from "lapping" head within a single operation.


2. Memory Barriers: On some architectures, would you need memory barriers? Why or why not?
When Memory Barriers Are Needed:
Architectures Requiring Barriers:

ARM Cortex-A (application processors with out-of-order execution)
x86-64 (for some operations)
RISC-V (depending on implementation)
Multi-core systems
*/