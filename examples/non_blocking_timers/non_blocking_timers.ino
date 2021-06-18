#include <Arduino.h>
#include <non_blocking_timers.h>

// There are multiple ways to use timers. The first way, you can create an array of them and pass them as arguments
non_blocking_timer my_test_timer_array[4];

// Alternatively you can define them standalone
non_blocking_timer my_test_function_timer;

// Or you can store them in heap memory, and if you like store an infite array of them
non_blocking_timer *my_oscillator_timer;

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

// IMPORTANT - You must be careful with your variable scope when using these timers. This library depends on you being responsible for maintaining your own timer objects. 
// This so that you have the freedom to manage them the way you see fit. Because of this trust, it's easy for you to fall into a trap like this:
void this_function_crashes(void) {
    non_blocking_timer reset_pin_timer;
    init_digitalWrite_timer(&reset_pin_timer, 40, 3, HIGH); 
    //  when this function finishes, the reset_pin_timer will be de-scoped (and popped off the stack). This breaks every timer and could even lock up the rest of your program. 
    //  You must ensure that your non_blocking_timer objects are in scope at least long enough for the timer to finish and be tidied up by the back end code. 
}
void workarounds_to_this_problem(void) {
    //  Best practice is to declare these outside of the scope of the function (e.g a class member, or a global) so that you have that memory put aside for all future uses.  
    //  Alternatively, you can do one of the following:
    non_blocking_timer *heap_reset_pin_timer; 
    heap_reset_pin_timer = new non_blocking_timer;      // store the object in heap memory. The pointer itself will be de-scoped but the object wont. Just remember to delete the object when you no longer need it
    
    static non_blocking_timer static_reset_pin_timer;     // or alternatively declare the variable static. This is ideal when you have a function that continually re-sets a timer
}
