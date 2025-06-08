/*

Question: GPIO Port Controller and SPI Interface
You're implementing firmware for a custom GPIO controller that interfaces with external devices via SPI. The system needs to efficiently manage pin configurations and communicate with multiple SPI devices.
Hardware Setup:

32-bit GPIO port with configurable pins
Each pin can be: Input, Output, Input with Pull-up, or High-Z
SPI controller with 4 chip select lines
Status register shows interrupt flags and device states

Register Definitions:
// GPIO Control Registers (32-bit each)
#define GPIO_DIR_REG     0x40001000  // Direction: 0=Input, 1=Output
#define GPIO_PULLUP_REG  0x40001004  // Pull-up enable: 0=Disable, 1=Enable
#define GPIO_DATA_REG    0x40001008  // Data register
#define GPIO_INT_EN_REG  0x4000100C  // Interrupt enable

// SPI Control Registers
#define SPI_CTRL_REG     0x40002000  // Control register
#define SPI_STATUS_REG   0x40002004  // Status register
#define SPI_DATA_REG     0x40002008  // Data register

// SPI Control Register bits
#define SPI_ENABLE       (1 << 0)
#define SPI_CPOL         (1 << 1)    // Clock polarity
#define SPI_CPHA         (1 << 2)    // Clock phase
#define SPI_CS_MASK      (0xF << 8)  // Chip select bits [11:8]


Part 1: GPIO Configuration 
Implement functions to configure GPIO pins efficiently:
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT,
    GPIO_INPUT_PULLUP,
    GPIO_HIGH_Z
} gpio_mode_t;

// Configure multiple pins at once using bitmasks
void gpio_config_pins(uint32_t pin_mask, gpio_mode_t mode);

// Set/clear specific output pins atomically
void gpio_set_pins(uint32_t pin_mask);
void gpio_clear_pins(uint32_t pin_mask);

// Read input pins and return masked result
uint32_t gpio_read_pins(uint32_t pin_mask);



Part 2: SPI Transaction 
Implement an SPI transaction function that:

Configures SPI mode (CPOL/CPHA)
Selects appropriate chip select
Performs full-duplex data exchange
Handles timeout conditions
Returns transaction status

typedef struct {
    uint8_t chip_select;    // 0-3
    uint8_t cpol;          // Clock polarity
    uint8_t cpha;          // Clock phase
    uint32_t timeout_ms;
} spi_config_t;

typedef enum {
    SPI_SUCCESS = 0,
    SPI_TIMEOUT,
    SPI_BUSY,
    SPI_ERROR
} spi_status_t;

spi_status_t spi_transaction(spi_config_t* config, 
                           uint8_t* tx_data, 
                           uint8_t* rx_data, 
                           uint16_t length);


Part 3: Bit Manipulation Challenge (10 minutes)
The status register contains packed information:

Bits [31:24] - Device temperatures (8 bits, signed, °C)
Bits [23:16] - Error flags (8 individual error bits)  
Bits [15:8]  - SPI transaction counters (4 counters, 2 bits each)
Bits [7:0]   - GPIO interrupt flags


Implement utility functions:
// Extract temperature and convert to actual value
int8_t extract_temperature(uint32_t status_reg);

// Check if any critical errors are set (bits 23, 21, 19 of error flags)
bool has_critical_errors(uint32_t status_reg);

// Increment specific SPI counter (0-3) without affecting others
uint32_t increment_spi_counter(uint32_t status_reg, uint8_t counter_id);

// Find the highest priority active GPIO interrupt (lowest bit number)
int8_t get_highest_priority_interrupt(uint32_t status_reg);

Follow-up Questions:

How would you handle endianness when communicating with different SPI devices?
What's the difference between using bit fields vs bit manipulation macros?
How would you optimize for code size vs execution speed?
Explain the trade-offs of memory-mapped I/O vs port-based I/O.


*/





// My attempt




// GPIO Control Registers (32-bit each)
#define GPIO_DIR_REG     (*(volatile uint32_t*)0x40001000)  // Direction: 0=Input, 1=Output
#define GPIO_PULLUP_REG  (*(volatile uint32_t*)0x40001004)  // Pull-up enable: 0=Disable, 1=Enable
#define GPIO_DATA_REG    (*(volatile uint32_t*)0x40001008)  // Data register
#define GPIO_INT_EN_REG  (*(volatile uint32_t*)0x4000100C)  // Interrupt enable

// SPI Control Registers
#define SPI_CTRL_REG     (*(volatile uint32_t*)0x40002000)  // Control register
#define SPI_STATUS_REG   (*(volatile uint32_t*)0x40002004)  // Status register
#define SPI_DATA_REG     (*(volatile uint32_t*)0x40002008)  // Data register



// SPI Control Register bits
// SPI Control Register bits
#define SPI_ENABLE       (1U << 0)
#define SPI_CPOL         (1U << 1)
#define SPI_CPHA         (1U << 2)
#define SPI_CS_SHIFT     8
#define SPI_CS_MASK      (0xFU << SPI_CS_SHIFT)
#define SPI_BUSY         (1U << 0)  // Status register bit


// Timeout and utility macros
#define SPI_TIMEOUT_COUNT 10000
#define GET_SYSTICK_MS()  (systick_counter)  // Assume system tick counter exists

// Global system tick counter (assume this exists)
extern volatile uint32_t systick_counter;


// Type definitions
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT,
    GPIO_INPUT_PULLUP,
    GPIO_HIGH_Z
} gpio_mode_t;

typedef struct {
    uint8_t chip_select;    // 0-3
    uint8_t cpol;          // Clock polarity
    uint8_t cpha;          // Clock phase
    uint32_t timeout_ms;
} spi_config_t;

typedef enum {
    SPI_SUCCESS = 0,
    SPI_TIMEOUT,
    SPI_BUSY,
    SPI_ERROR
} spi_status_t;



void gpio_config_pins(uint32_t pin_mask, gpio_mode_t mode) {
    // Read current register values
    uint32_t dir_reg = GPIO_DIR_REG;
    uint32_t pullup_reg = GPIO_PULLUP_REG;
    
    switch (mode) {
        case GPIO_INPUT:
            // Clear direction bits (input), clear pull-up bits
            dir_reg &= ~pin_mask;
            pullup_reg &= ~pin_mask;
            break;
            
        case GPIO_OUTPUT:
            // Set direction bits (output), clear pull-up bits
            dir_reg |= pin_mask;
            pullup_reg &= ~pin_mask;
            break;
            
        case GPIO_INPUT_PULLUP:
            // Clear direction bits (input), set pull-up bits
            dir_reg &= ~pin_mask;
            pullup_reg |= pin_mask;
            break;
            
        case GPIO_HIGH_Z:
            // Clear direction bits (input), clear pull-up bits
            // Same as GPIO_INPUT but semantically different
            dir_reg &= ~pin_mask;
            pullup_reg &= ~pin_mask;
            break;
    }
    
    // Write back to registers atomically
    GPIO_DIR_REG = dir_reg;
    GPIO_PULLUP_REG = pullup_reg;
}




void gpio_set_pins(uint32_t pin_mask) {
    // Atomic set operation - only affects specified pins
    GPIO_DATA_REG |= pin_mask;
}

void gpio_clear_pins(uint32_t pin_mask) {
    // Atomic clear operation - only affects specified pins
    GPIO_DATA_REG &= ~pin_mask;
}

uint32_t gpio_read_pins(uint32_t pin_mask) {
    // Read data register and mask to get only requested pins
    return (GPIO_DATA_REG & pin_mask);
}

// ============================================================================
// PART 2: SPI Transaction Function
// ============================================================================

spi_status_t spi_transaction(spi_config_t* config, 
                           uint8_t* tx_data, 
                           uint8_t* rx_data, 
                           uint16_t length) {
    
    if (!config || !tx_data || !rx_data || length == 0) {
        return SPI_ERROR;
    }
    
    // Check if SPI is already busy
    if (SPI_STATUS_REG & SPI_BUSY) {
        return SPI_BUSY;
    }
    
    // Configure SPI control register
    uint32_t ctrl_reg = SPI_ENABLE;  // Enable SPI
    
    // Set clock polarity and phase
    if (config->cpol) {
        ctrl_reg |= SPI_CPOL;
    }
    if (config->cpha) {
        ctrl_reg |= SPI_CPHA;
    }
    
    // Set chip select (validate range first)
    if (config->chip_select > 3) {
        return SPI_ERROR;
    }
    ctrl_reg |= ((uint32_t)(config->chip_select) << SPI_CS_SHIFT);
    
    // Apply configuration
    SPI_CTRL_REG = ctrl_reg;
    
    // Perform transaction with timeout
    uint32_t start_time = GET_SYSTICK_MS();
    
    for (uint16_t i = 0; i < length; i++) {
        // Write data to transmit
        SPI_DATA_REG = tx_data[i];
        
        // Wait for transaction to complete with timeout
        uint32_t timeout_count = 0;
        while (SPI_STATUS_REG & SPI_BUSY) {
            timeout_count++;
            if (timeout_count > SPI_TIMEOUT_COUNT) {
                // Disable SPI before returning error
                SPI_CTRL_REG = 0;
                return SPI_TIMEOUT;
            }
            
            // Check overall timeout
            if ((GET_SYSTICK_MS() - start_time) > config->timeout_ms) {
                SPI_CTRL_REG = 0;
                return SPI_TIMEOUT;
            }
        }
        
        // Read received data
        rx_data[i] = (uint8_t)(SPI_DATA_REG & 0xFF);
    }
    
    // Disable SPI and clear chip select
    SPI_CTRL_REG = 0;
    
    return SPI_SUCCESS;
}

// ============================================================================
// PART 3: Bit Manipulation Utility Functions
// ============================================================================

// Status Register Bit Layout:
// Bits [31:24] - Device temperatures (8 bits, signed, °C)
// Bits [23:16] - Error flags (8 individual error bits)  
// Bits [15:8]  - SPI transaction counters (4 counters, 2 bits each)
// Bits [7:0]   - GPIO interrupt flags

int8_t extract_temperature(uint32_t status_reg) {
    // Extract bits [31:24] and cast to signed 8-bit
    return (int8_t)((status_reg >> 24) & 0xFF);
}

bool has_critical_errors(uint32_t status_reg) {
    // Check bits 23, 21, 19 of error flags (bits 23:16 overall)
    // Bit 23 = bit 7 of error flags
    // Bit 21 = bit 5 of error flags  
    // Bit 19 = bit 3 of error flags
    uint32_t critical_error_mask = (1U << 23) | (1U << 21) | (1U << 19);
    return (status_reg & critical_error_mask) != 0;
}

uint32_t increment_spi_counter(uint32_t status_reg, uint8_t counter_id) {
    if (counter_id > 3) {
        return status_reg;  // Invalid counter ID
    }
    
    // Each counter is 2 bits, starting at bit 8
    uint8_t shift = 8 + (counter_id * 2);
    uint32_t counter_mask = 0x3U << shift;  // 2-bit mask
    
    // Extract current counter value
    uint32_t current_count = (status_reg >> shift) & 0x3;
    
    // Increment with wrap-around (2-bit counter: 0,1,2,3,0,...)
    uint32_t new_count = (current_count + 1) & 0x3;
    
    // Clear old counter value and set new value
    status_reg = (status_reg & ~counter_mask) | (new_count << shift);
    
    return status_reg;
}

int8_t get_highest_priority_interrupt(uint32_t status_reg) {
    // Extract GPIO interrupt flags (bits [7:0])
    uint8_t gpio_flags = (uint8_t)(status_reg & 0xFF);
    
    if (gpio_flags == 0) {
        return -1;  // No interrupts active
    }
    
    // Find the lowest set bit (highest priority)
    // Using built-in function if available, otherwise manual loop
    #ifdef __GNUC__
        return __builtin_ctz(gpio_flags);  // Count trailing zeros
    #else
        for (int8_t i = 0; i < 8; i++) {
            if (gpio_flags & (1U << i)) {
                return i;
            }
        }
        return -1;  // Should never reach here if gpio_flags != 0
    #endif
}

// ============================================================================
// Additional Utility Functions
// ============================================================================

// Helper function to configure multiple GPIO pins with different modes
void gpio_config_pin_array(const uint8_t* pins, const gpio_mode_t* modes, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        if (pins[i] < 32) {  // Validate pin number
            gpio_config_pins(1U << pins[i], modes[i]);
        }
    }
}

// Optimized function to toggle GPIO pins
void gpio_toggle_pins(uint32_t pin_mask) {
    // Read current state and XOR with mask to toggle
    uint32_t current_state = GPIO_DATA_REG;
    GPIO_DATA_REG = current_state ^ pin_mask;
}

// Status register analysis function
void analyze_status_register(uint32_t status_reg) {
    int8_t temp = extract_temperature(status_reg);
    bool critical = has_critical_errors(status_reg);
    int8_t highest_int = get_highest_priority_interrupt(status_reg);
    
    // Extract individual SPI counters
    uint8_t spi_counters[4];
    for (int i = 0; i < 4; i++) {
        spi_counters[i] = (status_reg >> (8 + i * 2)) & 0x3;
    }
    
    // Results would typically be used for system decisions
    // In embedded systems, this might trigger different actions
}



