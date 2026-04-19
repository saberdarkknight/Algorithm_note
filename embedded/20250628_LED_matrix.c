/*

Problem: LED Matrix Controller Driver

You're developing firmware for a 16x16 LED matrix display controller. 
The hardware uses memory-mapped I/O where each LED's state is controlled by specific bits in control registers.

Hardware Details:
16x16 LED matrix (256 LEDs total)
LEDs are controlled via 16 32-bit control registers (REG0-REG15)
Each LED can be: OFF (00), DIM (01), MEDIUM (10), BRIGHT (11)
LEDs are mapped row-wise: LED(0,0) uses bits 0-1 of REG0, LED(0,1) uses bits 2-3 of REG0, etc.
Hardware has a "blink mask" register that can make any LED blink



*/


#define MATRIX_SIZE 16
#define NUM_REGISTERS 8
#define LEDS_PER_REGISTER 16

typedef enum {
    LED_OFF = 0,
    LED_DIM = 1,
    LED_MEDIUM = 2,
    LED_BRIGHT = 3
} led_brightness_t;

typedef enum {
    STATE_IDLE,
    STATE_UPDATING_LED,
    STATE_UPDATING_ROW,
    STATE_DRAWING_PATTERN,
    STATE_ROTATING,
    STATE_ERROR
} matrix_state_t;

typedef struct {
    uint32_t registers[NUM_REGISTERS];
    uint32_t blink_mask[NUM_REGISTERS];
    matrix_state_t current_state;
    int operation_progress;
    bool operation_complete;
} led_matrix_t;

// Initialize the LED matrix
void init_matrix(led_matrix_t *matrix) {
    memset(matrix->registers, 0, sizeof(matrix->registers));
    memset(matrix->blink_mask, 0, sizeof(matrix->blink_mask));
    matrix->current_state = STATE_IDLE;
    matrix->operation_progress = 0;
    matrix->operation_complete = true;
}

// Helper function to calculate register and bit position
static void get_position(int row, int col, int *reg_index, int *bit_offset) {
    int led_index = row * MATRIX_SIZE + col;
    *reg_index = led_index / LEDS_PER_REGISTER;
    *bit_offset = (led_index % LEDS_PER_REGISTER) * 2;
}

// Set individual LED brightness
void set_led(led_matrix_t *matrix, int row, int col, led_brightness_t brightness) {
    if (row < 0 || row >= MATRIX_SIZE || col < 0 || col >= MATRIX_SIZE) {
        matrix->current_state = STATE_ERROR;
        return;
    }
    
    matrix->current_state = STATE_UPDATING_LED;
    matrix->operation_complete = false;
    
    int reg_index, bit_offset;
    get_position(row, col, &reg_index, &bit_offset);
    
    // Clear the 2 bits for this LED
    uint32_t mask = ~(0x3 << bit_offset);
    matrix->registers[reg_index] &= mask;
    
    // Set the new brightness value
    matrix->registers[reg_index] |= ((uint32_t)brightness << bit_offset);
    
    matrix->current_state = STATE_IDLE;
    matrix->operation_complete = true;
}

// Get individual LED brightness
led_brightness_t get_led(led_matrix_t *matrix, int row, int col) {
    if (row < 0 || row >= MATRIX_SIZE || col < 0 || col >= MATRIX_SIZE) {
        matrix->current_state = STATE_ERROR;
        return LED_OFF;
    }
    
    int reg_index, bit_offset;
    get_position(row, col, &reg_index, &bit_offset);
    
    return (led_brightness_t)((matrix->registers[reg_index] >> bit_offset) & 0x3);
}

// Set an entire row to the same brightness
void set_row(led_matrix_t *matrix, int row, led_brightness_t brightness) {
    if (row < 0 || row >= MATRIX_SIZE) {
        matrix->current_state = STATE_ERROR;
        return;
    }
    
    matrix->current_state = STATE_UPDATING_ROW;
    matrix->operation_complete = false;
    matrix->operation_progress = 0;
    
    for (int col = 0; col < MATRIX_SIZE; col++) {
        int reg_index, bit_offset;
        get_position(row, col, &reg_index, &bit_offset);
        
        // Clear the 2 bits for this LED
        uint32_t mask = ~(0x3 << bit_offset);
        matrix->registers[reg_index] &= mask;
        
        // Set the new brightness value
        matrix->registers[reg_index] |= ((uint32_t)brightness << bit_offset);
        
        matrix->operation_progress++;
    }
    
    matrix->current_state = STATE_IDLE;
    matrix->operation_complete = true;
}

// Enable/disable blinking for a specific LED
void set_led_blink(led_matrix_t *matrix, int row, int col, bool enable) {
    if (row < 0 || row >= MATRIX_SIZE || col < 0 || col >= MATRIX_SIZE) {
        matrix->current_state = STATE_ERROR;
        return;
    }
    
    int led_index = row * MATRIX_SIZE + col;
    int reg_index = led_index / 32;  // 32 bits per register for blink mask
    int bit_offset = led_index % 32;
    
    if (enable) {
        matrix->blink_mask[reg_index] |= (1 << bit_offset);
    } else {
        matrix->blink_mask[reg_index] &= ~(1 << bit_offset);
    }
}

// Draw a pattern: set LEDs to BRIGHT if corresponding bit in pattern is 1
void draw_pattern(led_matrix_t *matrix, uint16_t pattern[16]) {
    matrix->current_state = STATE_DRAWING_PATTERN;
    matrix->operation_complete = false;
    matrix->operation_progress = 0;
    
    for (int row = 0; row < MATRIX_SIZE; row++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            led_brightness_t brightness = (pattern[row] & (1 << col)) ? LED_BRIGHT : LED_OFF;
            
            int reg_index, bit_offset;
            get_position(row, col, &reg_index, &bit_offset);
            
            // Clear the 2 bits for this LED
            uint32_t mask = ~(0x3 << bit_offset);
            matrix->registers[reg_index] &= mask;
            
            // Set the new brightness value
            matrix->registers[reg_index] |= ((uint32_t)brightness << bit_offset);
        }
        matrix->operation_progress++;
    }
    
    matrix->current_state = STATE_IDLE;
    matrix->operation_complete = true;
}

// Rotate the entire display 90 degrees clockwise
void rotate_display_90(led_matrix_t *matrix) {
    matrix->current_state = STATE_ROTATING;
    matrix->operation_complete = false;
    matrix->operation_progress = 0;
    
    uint32_t temp_registers[NUM_REGISTERS] = {0};
    
    for (int row = 0; row < MATRIX_SIZE; row++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            // Get current LED brightness
            led_brightness_t brightness = get_led(matrix, row, col);
            
            // Calculate new position after 90-degree clockwise rotation
            int new_row = col;
            int new_col = MATRIX_SIZE - 1 - row;
            
            // Set in temporary registers
            int reg_index, bit_offset;
            get_position(new_row, new_col, &reg_index, &bit_offset);
            
            uint32_t mask = ~(0x3 << bit_offset);
            temp_registers[reg_index] &= mask;
            temp_registers[reg_index] |= ((uint32_t)brightness << bit_offset);
        }
        matrix->operation_progress++;
    }
    
    // Copy temporary registers back to main registers
    memcpy(matrix->registers, temp_registers, sizeof(temp_registers));
    
    matrix->current_state = STATE_IDLE;
    matrix->operation_complete = true;
}

// State machine update function
void update_matrix_state(led_matrix_t *matrix) {
    switch (matrix->current_state) {
        case STATE_IDLE:
            // Ready for new operations
            break;
            
        case STATE_UPDATING_LED:
        case STATE_UPDATING_ROW:
        case STATE_DRAWING_PATTERN:
        case STATE_ROTATING:
            // Operations are handled synchronously in this implementation
            // In a real embedded system, these might be interrupt-driven
            break;
            
        case STATE_ERROR:
            printf("Matrix controller error state\n");
            // Error recovery logic could go here
            matrix->current_state = STATE_IDLE;
            break;
    }
}

// Utility function to print matrix state
void print_matrix(led_matrix_t *matrix) {
    printf("LED Matrix State:\n");
    printf("Current State: %d\n", matrix->current_state);
    printf("Operation Progress: %d\n", matrix->operation_progress);
    printf("Operation Complete: %s\n", matrix->operation_complete ? "Yes" : "No");
    
    printf("\nMatrix Display:\n");
    for (int row = 0; row < MATRIX_SIZE; row++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            led_brightness_t brightness = get_led(matrix, row, col);
            printf("%d ", brightness);
        }
        printf("\n");
    }
    printf("\n");
}

// Test function demonstrating the state machine
int main() {
    led_matrix_t matrix;
    init_matrix(&matrix);
    
    printf("=== LED Matrix Controller Test ===\n");
    
    // Test 1: Set individual LEDs
    printf("Test 1: Setting individual LEDs\n");
    set_led(&matrix, 0, 0, LED_BRIGHT);
    set_led(&matrix, 0, 1, LED_MEDIUM);
    set_led(&matrix, 1, 0, LED_DIM);
    update_matrix_state(&matrix);
    
    // Test 2: Set entire row
    printf("Test 2: Setting entire row 2 to BRIGHT\n");
    set_row(&matrix, 2, LED_BRIGHT);
    update_matrix_state(&matrix);
    
    // Test 3: Enable blinking
    printf("Test 3: Enabling blink for LED (0,0)\n");
    set_led_blink(&matrix, 0, 0, true);
    
    // Test 4: Draw pattern (simple cross)
    printf("Test 4: Drawing cross pattern\n");
    uint16_t cross_pattern[16] = {0};
    for (int i = 0; i < 16; i++) {
        cross_pattern[i] = (1 << 8) | (i == 8 ? 0xFFFF : 0);  // Vertical and horizontal line
    }
    draw_pattern(&matrix, cross_pattern);
    update_matrix_state(&matrix);
    
    // Test 5: Rotate display
    printf("Test 5: Rotating display 90 degrees\n");
    rotate_display_90(&matrix);
    update_matrix_state(&matrix);
    
    // Print final state
    print_matrix(&matrix);
    
    // Show register contents
    printf("Register Contents:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("REG%d: 0x%08X\n", i, matrix.registers[i]);
    }
    
    return 0;
}