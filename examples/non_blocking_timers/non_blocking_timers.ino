#include <Arduino.h>
#include <non_blocking_timers.h>

void my_test_func(void) {
    Serial.println("Test"); // keep these call back functions short, and do not use any delays within this. 
}

// You will need a globally scoped array of timers. MAX_TIMERS = 32, but feel free to use less, to make this more memory efficient. 
non_blocking_timer test_timer_array[non_blocking_timer::MAX_TIMERS];

void setup() {
    Serial.begin(9600);
    Serial.println("Program Started, Initializing timers");

	init_digitalWrite_timer(test_timer_array, 50, 3, HIGH); // after 50ms, set pin 3 to HIGH
	init_digitalWrite_oscillator(test_timer_array, 250, LED_BUILTIN, HIGH); // Oscillate D4 with 5ms period, setting the pin HIGH on the first cycle
	init_function_timer(test_timer_array, 1000, my_test_func, true);    // call the my_test_func() every 1000ms.
}

void loop() {
    // using any delays above or below update_timers will ruin the timing of your non-blocking timers. 
    update_timers(test_timer_array); // this line must be at the end of each loop to update the timers
}