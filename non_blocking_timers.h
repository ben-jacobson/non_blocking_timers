#ifndef non_blocking_timers_h
#define non_blocking_timers_h

#include <Arduino.h>

// not recommended for anything less than 5ms

typedef void (*callback_function)(void);    // syntactic sugar for a function pointer, allows for cleaner code when taking a function pointer as an argument. 

struct non_blocking_timer {
    unsigned long int start_time, end_after;
    bool finished = true; 
    bool repeating = false; 
    non_blocking_timer *next_timer_in_list = nullptr;     // we use a linked list to make timers easy to create in arrays, or standalone, independant of scope
    uint8_t ulPin, ulVal; // for use with timers needing to alter pin values
    void (*event_callback)(void) = nullptr; // for use with timers needing to call a callback function, at the moment only works with functions that return void types
}; 

void add_new_generic_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, bool _repeating);
void init_digitalWrite_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, uint8_t _ulPin, uint8_t _ulVal);
void init_digitalWrite_oscillator(non_blocking_timer *non_blocking_timer_ptr, unsigned int _cycle_duration, uint32_t _ulPin, uint32_t _start_ulVal);
void init_function_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, callback_function event_callback, bool _repeating);
void update_timers(void);

non_blocking_timer static_timer_first_in_list; // the first item in the list is an unused timer which acts as the static base
non_blocking_timer *last_timer_in_list = &static_timer_first_in_list;

#endif