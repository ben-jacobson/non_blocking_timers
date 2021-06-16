#ifndef non_blocking_timers_h
#define non_blocking_timers_h

#include <Arduino.h>

// not recommended for anything less than 5ms
typedef void (*callback_function)(void);    // syntactic sugar for function pointer, allows for cleaner code when taking a function pointer as an argument. 

struct non_blocking_timer {
    unsigned long int start_time, end_after;
    void (*event_callback)(void); // for use with timers needing to call a callback function, at the moment only works with functions that return void types
    uint8_t ulPin, ulVal; // for use with timers needing to alter pin values
    bool finished = true; 
    bool repeating = false; 
    non_blocking_timer *first_timer_in_list, *next_timer_in_list = nullptr;     // we used linked lists to create multiple pointers. The first timer is the first in the list, and every object points to the next timer in the list. 
}; 

non_blocking_timer *get_last_timer_in_list(non_blocking_timer *timer);
void add_new_timer(non_blocking_timer timer_array[], unsigned int _end_after, bool _repeating);
void init_digitalWrite_timer(non_blocking_timer timer_array[], unsigned int _end_after, uint8_t _ulPin, uint8_t _ulVal);
void init_digitalWrite_oscillator(non_blocking_timer timer_array[], unsigned int _cycle_duration, uint32_t _ulPin, uint32_t _start_ulVal);
void init_function_timer(non_blocking_timer timer_array[], unsigned int _end_after, callback_function event_callback, bool _repeating);
void update_timers(non_blocking_timer timer_array[]);

#endif