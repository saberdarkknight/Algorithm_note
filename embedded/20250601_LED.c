
/*
You are writing firmware for a simple LED controller. This controller has one button and one LED.

Each time the button is pressed (short press), the LED's state should toggle (on -> off, off -> on).
If the button is long-pressed (e.g., held down for more than 2 seconds), the LED should blink rapidly (e.g., change state every 100ms) until the button is released.
When the button is released after a long press, the LED should return to its state before the long press.
Implement a ButtonHandler() function in C. This function will be called periodically at a fixed time interval (e.g., every 10ms).

Assumed Hardware Interface:

To make the code testable, let's assume the following two simple hardware abstraction functions:

int read_button_gpio(void);
Returns 1 if the button is pressed (high or low, depending on your design assumption).
Returns 0 if the button is not pressed.
void set_led_gpio(int state);
state of 1 means the LED is on.
state of 0 means the LED is off.
Expected Considerations:

Debouncing: You'll need to debounce the button input to prevent misinterpretation due to contact bounce.
State Machine: Use a state machine to handle the different button states: short press, long press, and release.
Timers: Determine the duration of a long press. You can use the periodic calls to ButtonHandler() to simulate a timer.
Bit Manipulation: (Though abstracted by set_led_gpio here, in actual embedded systems, this often involves direct register bit operations).
Real-time constraints: Consider the execution time of ButtonHandler() and its impact on the periodic calls.
Hints:

Define several states to represent different button conditions, such as BUTTON_IDLE, BUTTON_PRESS_DETECTED, BUTTON_LONG_PRESS_ACTIVE, BUTTON_RELEASED.
Use some static variables to keep track of the button's state, press duration counter, and the LED's current state/pre-long-press state.
Since ButtonHandler() is called every 10ms, you can set your counter unit to 10ms. This means 2 seconds equals 200 counter units. A blinking frequency of changing state every 100ms means changing state every 10 counter units.

*/





// My attempt

#define BUTTON_IDLE 		  	 1
#define BUTTON_PRESS_DETECTED 	 2
#define BUTTON_LONG_PRESS_ACTIVE 3
#define BUTTON_RELEASED 		 4


ButtonHandler(){
	// set the time to static and every time function is called, we can add 10 ms
	static int button_state = 0;
	static int press_counter = 0;
	static int led_current_state = 0;
	static int led_state_before_long_press = 0;
		
	// read read_button_gpio
	int gpio_state = read_button_gpio();
	// press the LED and start the counter
	if ( gpio_state  && ( button_state == BUTTON_IDLE) ){
		button_state = BUTTON_PRESS_DETECTED;
	} 
	if ( gpio_state  && ( button_state == BUTTON_PRESS_DETECTED )  ){
		press_counter += 10;
	}
	if ( ( gpio_state == 0)  && ( button_state == BUTTON_PRESS_DETECTED )  ){
		button_state = BUTTON_RELEASED;
	}


	// set_led_gpio by counter 
	if ( ( button_state == BUTTON_RELEASED ) && (press_counter < 2000)  ){
		button_state = BUTTON_IDLE;
		led_current_state = ( led_current_state + 1 )%2;
		set_led_gpio(led_current_state);
 		press_counter = 0;
	}


	// process the long press
	if ( ( button_state == BUTTON_PRESS_DETECTED ) && (press_counter > 2000)  ){
		button_state = BUTTON_LONG_PRESS_ACTIVE;
		led_state_before_long_press = led_current_state;
		press_counter = 0;
	}
	if ( ( button_state == BUTTON_LONG_PRESS_ACTIVE )  &&  ( press_counter % 100 == 0)  ){
		led_current_state = ( led_current_state + 1 )%2;
		set_led_gpio(led_current_state);
		press_counter += 10;
	} else if (   button_state == BUTTON_LONG_PRESS_ACTIVE  ) {
		press_counter += 10;
	}
	if ( ( gpio_state == 0)  && ( button_stat == BUTTON_LONG_PRESS_ACTIVE)  ){
		press_counter = 0;
		led_current_state = led_state_before_long_press;
		set_led_gpio(led_current_state);
		button_state  = BUTTON_IDLE;
	}
}


// Better solution

void ButtonHandler() {
    // set the time to static and every time function is called, we can add 10 ms
    static int button_state = 0;
    static int press_counter = 0;
    static int led_current_state = 0;
    static int led_state_before_long_press = 0;
    
// read read_button_gpio
    int gpio_state = read_button_gpio();

    // debouncing

    switch (button_state) {
        case BUTTON_IDLE:
            if (gpio_state) {
                button_state=BUTTON_PRESS_DETECTED;
            }
            break;
        case BUTTON_PRESS_DETECTED:

        	// debouncing
            while(cnt<10000){
                cnt+=1;
            }
            gpio_state = read_button_gpio();
            if (gpio_state) {
                press_counter+=10;
                // long press
                if(press_counter>=2000){
                    button_state=BUTTON_LONG_PRESS_ACTIVE;
                    led_state_before_long_press=led_current_state;
                    press_counter=0;
                }
            } else {
                button_state=BUTTON_RELEASED;
            }
            break;
        case BUTTON_LONG_PRESS_ACTIVE:
            if(gpio_state) {
                if(press_counter%100==0){
                    led_current_state^=1;
                    set_led_gpio(led_current_state);
                    
                }
                press_counter+=10;
            } else {
                button_state=BUTTON_IDLE;
                led_current_state=led_state_before_long_press;
                set_led_gpio(led_current_state);
                press_counter=0;
            }
            break;
        case BUTTON_RELEASED:
            if (press_counter<2000) {
                led_current_state^=1;
                set_led_gpio(led_current_state);
                button_state=BUTTON_IDLE;
            }
            break;
        default:
            break;
    }

    