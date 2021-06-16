# non_blocking_timers
Arduino Library for Non Blocking Timers

**Usage:
**

Start by Initializing a globally scoped array of timers. by default, MAX_TIMERS = 32.

non_blocking_timer test_timer_array[non_blocking_timer::MAX_TIMERS];

If you would like to use less, you may change the value of MAX_TIMERS manually before initializing this line.

From there, see example

**Example:
**

void setup() {
    Serial.begin(9600);
    Serial.println("Program Started, Initializing timers");

    init_digitalWrite_timer(test_timer_array, 50, 3, HIGH); // after 50ms, set D3 to HIGH
    init_digitalWrite_oscillator(test_timer_array, 250, LED_BUILTIN, HIGH); // Oscillate D4 with 5ms period, setting the pin HIGH on the first cycle
    init_function_timer(test_timer_array, 1000, my_test_func, true);    // call the my_test_func() every 1000ms.
}

void loop() {
    // using any delays above or below update_timers will ruin the timing of your non-blocking timers. 
    update_timers(test_timer_array); // this line must be at the end of each loop to update the timers
}

**Function references:
**

void init_digitalWrite_timer(non_blocking_timer timer_array[], unsigned int _end_after, uint32_t _ulPin, uint32_t _ulVal);
Sets up a non-blocking timer to toggle a digital pin after _end_after

- non_blocking_timer timer_array[]: Timer Object (see above)
- unsigned int _end_after: How long before the digitalWrite is triggered
- uint32_t _ulPin: The pin you wish to toggle. E.g D13
- uint32_t _ulVal: The value you wish to write, E.g HIGH


void init_digitalWrite_oscillator(non_blocking_timer timer_array[], unsigned int _cycle_duration, uint32_t _ulPin, uint32_t _start_ulVal);
Like init_digitalWrite_timer, except it will oscillate

- non_blocking_timer timer_array[]: Timer Object (see above)
- unsigned int _cycle_duration: The period of the oscillator. E.g if set to 50ms, the pin will be set HIGH for 50ms, then LOW for 50ms
- uint32_t _ulPin: The pin you wish to oscillate. E.g D4
- uint32_t _start_ulVal: You can set your starting value from the start of the oscillation. E.g LOW This will cause the oscillation to set the pin to this value from the first cycle onwards, and this does nothing to change the default Arduino behaviour of starting pin values, so if you find this doesn't work for you, then you may wish to add in your own digitalWrite before hand. 

void init_function_timer(non_blocking_timer timer_array[], unsigned int _end_after, callback_function event_callback, bool _repeating);
Set up a timer to call a function when the timer expires. At the moment can only be used with functions with a void return type.

- non_blocking_timer timer_array[]: Timer Object (see above)
- unsigned int _end_after: The function will be called after this many ms
- callback_function event_callback: The function to call, please provide a function with a void return type
- bool _repeating: If you would like this to repeat every _end_after ms, set this to True

void update_timers(non_blocking_timer timer_array[]);
call this function at the end of your loop() function, this updates the timers and keeps everything in sync
