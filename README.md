# non_blocking_timers
Arduino Library for Non Blocking Timers

**Usage:**

Start by Initializing the timer object.

    // There are multiple ways to use timers. The first way, you can create an array of them and pass them as arguments
    non_blocking_timer my_test_timer_array[4];

    // Alternatively you can define them standalone
    non_blocking_timer my_test_function_timer;

    // Or you can store them in heap memory, and if you like store an infite array of them
    non_blocking_timer *my_oscillator_timer;

From there, see example

**Example:**

void my_func(void) {
    Serial.println("Test");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Program Started, Initializing timers");

    my_oscillator_timer = new non_blocking_timer; // for storage on heap, you could even store an array of them on the heap

    init_digitalWrite_oscillator(my_oscillator_timer, 250, LED_BUILTIN, HIGH); // Oscillate D4 with 5ms period, setting the pin HIGH on the first cycle	
    init_digitalWrite_timer(&my_test_timer_array[0], 50, 4, HIGH);     // after 50ms take pin 4 HIGH
    init_digitalWrite_timer(&my_test_timer_array[1], 100, 4, LOW);     // then after 100ms take pin 4 LOW
    init_digitalWrite_timer(&my_test_timer_array[2], 150, 4, HIGH);    // after 150ms take pin 4 HIGH again
    init_function_timer(&my_test_function_timer, 1000, my_func, true);
}

void loop() {
    // using any delays above or below update_timers will ruin the timing of your non-blocking timers. 
    update_timers(); // this line must be at the end of each loop to update the timers    
}

**Important Note - don't use a timer after it's been been descoped**

    // IMPORTANT - You must be careful with your variable scope when using these timers. This library depends on you being responsible for maintaining your own timer objects. 
    // This so that you have the freedom to manage them the way you see fit. Because of this trust, it's easy for you to fall into a trap like this:
    void this_function_crashes(void) {
        non_blocking_timer reset_pin_timer;
        init_digitalWrite_timer(&reset_pin_timer, 40, 3, HIGH); 
        //  when this function finishes, the reset_pin_timer will be de-scoped (and popped off the stack). This breaks every timer and could even lock up the rest of your       program. 
        //  You must ensure that your non_blocking_timer objects are in scope at least long enough for the timer to finish and be tidied up by the back end code. 
    }
    void workarounds_to_this_problem(void) {
        //  Best practice is to declare these outside of the scope of the function (e.g a class member, or a global) so that you have that memory put aside for all future uses.  
        //  Alternatively, you can do one of the following:
        non_blocking_timer *heap_reset_pin_timer; 
        heap_reset_pin_timer = new non_blocking_timer;      // store the object in heap memory. The pointer itself will be de-scoped but the object wont. Just remember to delete the object when you no longer need it

        static non_blocking_timer static_reset_pin_timer;     // or alternatively declare the variable static. This is ideal when you have a function that continually re-sets a timer
    }

**Function references:**

    void init_digitalWrite_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, uint8_t _ulPin, uint8_t _ulVal);

Sets up a non-blocking timer to toggle a digital pin after _end_after

- non_blocking_timer *non_blocking_timer_ptr: Timer Object (see above)
- unsigned int _end_after: How long before the digitalWrite is triggered
- uint32_t _ulPin: The pin you wish to toggle. E.g digital pin 13
- uint32_t _ulVal: The value you wish to write, E.g HIGH


    void init_digitalWrite_oscillator(non_blocking_timer *non_blocking_timer_ptr, unsigned int _cycle_duration, uint32_t _ulPin, uint32_t _start_ulVal);

Like init_digitalWrite_timer, except it will oscillate

- non_blocking_timer *non_blocking_timer_ptr: Timer Object (see above)
- unsigned int _cycle_duration: The period of the oscillator. E.g if set to 50ms, the pin will be set HIGH for 50ms, then LOW for 50ms
- uint32_t _ulPin: The pin you wish to oscillate. E.g digital pin 4
- uint32_t _start_ulVal: You can set your starting value from the start of the oscillation. E.g LOW This will cause the oscillation to set the pin to this value from the first cycle onwards, and this does nothing to change the default Arduino behaviour of starting pin values, so if you find this doesn't work for you, then you may wish to add in your own digitalWrite before hand. 


    void init_function_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, callback_function event_callback, bool _repeating = false);

Set up a timer to call a function when the timer expires. At the moment can only be used with functions with a void return type.

- non_blocking_timer *non_blocking_timer_ptr: Timer Object (see above)
- unsigned int _end_after: The function will be called after this many ms
- callback_function event_callback: The function to call, please provide a function with a void return type
- bool _repeating: If you would like this to repeat every _end_after ms, set this to True


    void update_timers(void);
    
call this function at the end of your loop() function, this updates the timers and keeps everything in sync
