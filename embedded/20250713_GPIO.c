/*

Problem Statement 

You are working on an embedded system with a custom GPIO (General Purpose Input/Output) controller. The GPIO controller manages 32 pins and is memory-mapped to specific addresses. 
Your task is to implement a GPIO driver that provides basic functionality for configuring and controlling GPIO pins.



Memory Map Specification
The GPIO controller has the following memory-mapped registers:
Base Address: 0x40020000

Offset 0x00: GPIO_DIR     - Direction register (0=input, 1=output)
Offset 0x04: GPIO_DATA    - Data register (read input/write output)
Offset 0x08: GPIO_SET     - Set register (write 1 to set pin high)
Offset 0x0C: GPIO_CLR     - Clear register (write 1 to set pin low)
Offset 0x10: GPIO_TOGGLE  - Toggle register (write 1 to toggle pin)
Offset 0x14: GPIO_PULLUP  - Pull-up enable register (0=disabled, 1=enabled)



Part 1: Implementation 
Implement the following functions:

#include <stdint.h>

#define GPIO_BASE_ADDR  0x40020000
#define GPIO_DIR        (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x00))
#define GPIO_DATA       (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x04))
#define GPIO_SET        (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x08))
#define GPIO_CLR        (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x0C))
#define GPIO_TOGGLE     (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x10))
#define GPIO_PULLUP     (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x14))

typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} gpio_direction_t;

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_state_t;

// Function prototypes to implement:
void gpio_set_direction(uint8_t pin, gpio_direction_t dir);
void gpio_write(uint8_t pin, gpio_state_t state);
gpio_state_t gpio_read(uint8_t pin);
void gpio_toggle(uint8_t pin);
void gpio_enable_pullup(uint8_t pin, uint8_t enable);


Part 2: Advanced Functionality 

Implement these more complex functions:
// Set multiple pins to output and configure their initial states
// pins: bitmask of pins to configure
// states: bitmask of initial states (1=high, 0=low)
void gpio_configure_output_pins(uint32_t pins, uint32_t states);

// Read multiple input pins efficiently
// pins: bitmask of pins to read
// returns: bitmask of pin states
uint32_t gpio_read_pins(uint32_t pins);

// Atomic bit manipulation - set some pins high, some low in one operation
// set_mask: pins to set high
// clear_mask: pins to set low
void gpio_atomic_update(uint32_t set_mask, uint32_t clear_mask);

// Find the first set bit in the GPIO data register (useful for interrupt handling)
// returns: pin number (0-31) or -1 if no bits set
int gpio_find_first_set(void);

Part 3: Error Handling and Optimization (10 minutes)
Discuss and potentially implement:

Input Validation: How would you handle invalid pin numbers?
Performance Optimization: What optimizations would you make for frequently called functions?
Thread Safety: How would you make these functions thread-safe in an RTOS environment?
Power Management: How might you implement a function to put unused pins in a low-power state?

Expected Discussion Points

Volatile keyword: Why is volatile necessary for memory-mapped registers?
Bit manipulation efficiency: Compare different approaches (bit shifts vs. bit masks)
Memory barriers: When might you need memory barriers in embedded systems?
Register caching: Why shouldn't you cache register values?
Endianness: How might byte order affect your implementation?

Sample Solution Structure
The interviewer should expect candidates to:

Understand memory mapping: Correctly use pointer dereferencing for register access
Implement bit manipulation: Use appropriate bit operations (|, &, ^, <<, >>)
Handle edge cases: Validate pin numbers (0-31 range)
Optimize for performance: Use efficient bit operations, avoid unnecessary reads
Consider hardware constraints: Understand why certain operations are atomic vs. non-atomic

Bonus Challenge (if time permits)
Implement a function that can debounce a button input:

// Returns 1 if button press detected, 0 otherwise
// pin: GPIO pin connected to button
// debounce_ms: debounce time in milliseconds
uint8_t gpio_debounce_button(uint8_t pin, uint32_t debounce_ms);

This requires understanding of timing, state machines, and practical embedded considerations.

*/



#include <stdint.h>

#define GPIO_BASE_ADDR  0x40020000
#define GPIO_DIR        (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x00))
#define GPIO_DATA       (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x04))
#define GPIO_SET        (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x08))
#define GPIO_CLR        (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x0C))
#define GPIO_TOGGLE     (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x10))
#define GPIO_PULLUP     (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x14))

#define GPIO_PIN_MAX    31
#define GPIO_INVALID_PIN 0xFF

typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} gpio_direction_t;

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_state_t;

// Helper function to validate pin number
static inline int is_valid_pin(uint8_t pin) {
    return (pin <= GPIO_PIN_MAX);
}

// ==================== PART 1: Basic Implementation ====================

void gpio_set_direction(uint8_t pin, gpio_direction_t dir) {
    if (!is_valid_pin(pin)) {
        return; // Invalid pin number
    }
    
    if (dir == GPIO_OUTPUT) {
        GPIO_DIR |= (1U << pin);  // Set bit to 1 for output
    } else {
        GPIO_DIR &= ~(1U << pin); // Clear bit to 0 for input
    }
}

void gpio_write(uint8_t pin, gpio_state_t state) {
    if (!is_valid_pin(pin)) {
        return; // Invalid pin number
    }
    
    if (state == GPIO_HIGH) {
        GPIO_SET = (1U << pin);   // Use SET register for atomic operation
    } else {
        GPIO_CLR = (1U << pin);   // Use CLR register for atomic operation
    }
}

gpio_state_t gpio_read(uint8_t pin) {
    if (!is_valid_pin(pin)) {
        return GPIO_LOW; // Return safe default for invalid pin
    }
    
    return (GPIO_DATA & (1U << pin)) ? GPIO_HIGH : GPIO_LOW;
}

void gpio_toggle(uint8_t pin) {
    if (!is_valid_pin(pin)) {
        return; // Invalid pin number
    }
    
    GPIO_TOGGLE = (1U << pin); // Use TOGGLE register for atomic operation
}

void gpio_enable_pullup(uint8_t pin, uint8_t enable) {
    if (!is_valid_pin(pin)) {
        return; // Invalid pin number
    }
    
    if (enable) {
        GPIO_PULLUP |= (1U << pin);  // Enable pull-up
    } else {
        GPIO_PULLUP &= ~(1U << pin); // Disable pull-up
    }
}

// ==================== PART 2: Advanced Functionality ====================

void gpio_configure_output_pins(uint32_t pins, uint32_t states) {
    // First, set all specified pins to output
    GPIO_DIR |= pins;
    
    // Then set the initial states atomically
    uint32_t pins_to_set = pins & states;      // Pins that should be high
    uint32_t pins_to_clear = pins & (~states); // Pins that should be low
    
    if (pins_to_set) {
        GPIO_SET = pins_to_set;
    }
    if (pins_to_clear) {
        GPIO_CLR = pins_to_clear;
    }
}

uint32_t gpio_read_pins(uint32_t pins) {
    return GPIO_DATA & pins; // Return only the requested pins
}

void gpio_atomic_update(uint32_t set_mask, uint32_t clear_mask) {
    // Ensure we don't set and clear the same pin simultaneously
    set_mask &= ~clear_mask;
    
    if (set_mask) {
        GPIO_SET = set_mask;
    }
    if (clear_mask) {
        GPIO_CLR = clear_mask;
    }
}

int gpio_find_first_set(void) {
    uint32_t data = GPIO_DATA;
    
    if (data == 0) {
        return -1; // No bits set
    }
    
    // Find first set bit using bit manipulation
    // This is equivalent to __builtin_ctz() but implemented manually
    int pin = 0;
    if ((data & 0xFFFF) == 0) { data >>= 16; pin += 16; }
    if ((data & 0xFF) == 0)   { data >>= 8;  pin += 8;  }
    if ((data & 0xF) == 0)    { data >>= 4;  pin += 4;  }
    if ((data & 0x3) == 0)    { data >>= 2;  pin += 2;  }
    if ((data & 0x1) == 0)    { pin += 1; }
    
    return pin;
}

// Alternative implementation using lookup table (faster but uses more memory)
int gpio_find_first_set_fast(void) {
    static const int first_set_table[256] = {
        -1, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
    };
    
    uint32_t data = GPIO_DATA;
    
    if (data == 0) return -1;
    
    if (data & 0xFF) return first_set_table[data & 0xFF];
    if (data & 0xFF00) return 8 + first_set_table[(data >> 8) & 0xFF];
    if (data & 0xFF0000) return 16 + first_set_table[(data >> 16) & 0xFF];
    return 24 + first_set_table[(data >> 24) & 0xFF];
}

// ==================== PART 3: Error Handling and Optimization ====================

// Thread-safe versions (assuming mutex is available)
#ifdef USE_MUTEX
#include "mutex.h" // Hypothetical mutex header
static mutex_t gpio_mutex;

void gpio_write_safe(uint8_t pin, gpio_state_t state) {
    mutex_lock(&gpio_mutex);
    gpio_write(pin, state);
    mutex_unlock(&gpio_mutex);
}

void gpio_atomic_update_safe(uint32_t set_mask, uint32_t clear_mask) {
    mutex_lock(&gpio_mutex);
    gpio_atomic_update(set_mask, clear_mask);
    mutex_unlock(&gpio_mutex);
}
#endif

// Power management - put unused pins in low-power state
void gpio_set_low_power_mode(uint32_t pin_mask) {
    // Set pins as inputs (typically lower power)
    GPIO_DIR &= ~pin_mask;
    
    // Disable pull-ups to save power
    GPIO_PULLUP &= ~pin_mask;
    
    // Ensure pins are driven low if they were outputs
    GPIO_CLR = pin_mask;
}

// Optimized batch operations for performance-critical code
void gpio_batch_write(uint32_t pin_mask, uint32_t states) {
    // Calculate which pins to set high and low
    uint32_t set_pins = pin_mask & states;
    uint32_t clear_pins = pin_mask & ~states;
    
    // Atomic operations - no need for mutex if hardware supports it
    if (set_pins) GPIO_SET = set_pins;
    if (clear_pins) GPIO_CLR = clear_pins;
}

// ==================== BONUS: Button Debouncing ====================

#ifdef INCLUDE_TIMING_FUNCTIONS
#include "timer.h" // Hypothetical timer functions

typedef struct {
    uint32_t last_sample_time;
    uint8_t last_stable_state;
    uint8_t current_state;
    uint8_t debounce_count;
} debounce_state_t;

static debounce_state_t debounce_states[32]; // One per pin

uint8_t gpio_debounce_button(uint8_t pin, uint32_t debounce_ms) {
    if (!is_valid_pin(pin)) {
        return 0;
    }
    
    debounce_state_t* state = &debounce_states[pin];
    uint32_t current_time = get_system_time_ms();
    uint8_t raw_input = gpio_read(pin);
    
    // If input changed, reset debounce timer
    if (raw_input != state->current_state) {
        state->current_state = raw_input;
        state->last_sample_time = current_time;
        state->debounce_count = 0;
        return 0; // No stable change yet
    }
    
    // If enough time has passed and state is stable
    if ((current_time - state->last_sample_time) >= debounce_ms) {
        if (state->current_state != state->last_stable_state) {
            state->last_stable_state = state->current_state;
            
            // Return 1 only on press (assuming button is active low)
            return (state->current_state == GPIO_LOW) ? 1 : 0;
        }
    }
    
    return 0; // No new button press
}
#endif

// ==================== Utility Functions ====================

// Count number of set bits in GPIO data (population count)
int gpio_count_active_pins(void) {
    uint32_t data = GPIO_DATA;
    int count = 0;
    
    // Brian Kernighan's algorithm
    while (data) {
        data &= (data - 1); // Clear the lowest set bit
        count++;
    }
    
    return count;
}

// Get bitmask of all output pins
uint32_t gpio_get_output_pins(void) {
    return GPIO_DIR;
}

// Get bitmask of all input pins
uint32_t gpio_get_input_pins(void) {
    return ~GPIO_DIR;
}

// Example usage and test functions
void gpio_example_usage(void) {
    // Configure pin 5 as output, set it high
    gpio_set_direction(5, GPIO_OUTPUT);
    gpio_write(5, GPIO_HIGH);
    
    // Configure pins 0-7 as outputs with alternating pattern
    gpio_configure_output_pins(0xFF, 0xAA); // 10101010 pattern
    
    // Read multiple input pins
    uint32_t input_states = gpio_read_pins(0xFF00); // Read pins 8-15
    
    // Atomic update: set pins 0,2,4 high and pins 1,3,5 low
    gpio_atomic_update(0x15, 0x2A);
    
    // Find first active pin
    int first_pin = gpio_find_first_set();
    if (first_pin >= 0) {
        // Handle the first active pin
    }
}