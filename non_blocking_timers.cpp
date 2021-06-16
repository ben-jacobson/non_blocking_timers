#include "non_blocking_timers.h"

non_blocking_timer *get_last_timer_in_list(non_blocking_timer *timer_ptr) {
    non_blocking_timer *last_timer_in_list = timer_ptr->first_timer_in_list; 

    while (last_timer_in_list->next_timer_in_list != nullptr) {
        last_timer_in_list = last_timer_in_list->next_timer_in_list;
    }
    return last_timer_in_list;
}


void add_new_timer(non_blocking_timer *timer_array, unsigned int _end_after, bool _repeating) {
    int first_available = scan_for_available_timers(timer_array);

    if (first_available != -1)  {
        timer_array[first_available].finished = false;      
        timer_array[first_available].repeating = _repeating;      
        timer_array[first_available].end_after = _end_after;
        timer_array[first_available].start_time = millis();
    }
}

void init_digitalWrite_timer(non_blocking_timer timer_array[], unsigned int _end_after, uint32_t _ulPin, uint32_t _ulVal) {
    int first_available = scan_for_available_timers(timer_array);

    pinMode(_ulPin, OUTPUT);
    
    if (first_available != -1) {        
        timer_array[first_available].ulPin = _ulPin;
        timer_array[first_available].ulVal = _ulVal;
        timer_array[first_available].event_callback = nullptr; // disable the callback fn to avoid any confusion
        add_new_timer(&timer_array[first_available], _end_after, false);
    }
}

void init_digitalWrite_oscillator(non_blocking_timer timer_array[], unsigned int _cycle_duration, uint32_t _ulPin, uint32_t _start_ulVal) {
    int first_available = scan_for_available_timers(timer_array);
    
    pinMode(_ulPin, OUTPUT);

    if (first_available != -1) {        
        timer_array[first_available].ulPin = _ulPin;
        timer_array[first_available].ulVal = _start_ulVal;
        timer_array[first_available].event_callback = nullptr; // disable the callback fn to avoid any confusion
        add_new_timer(&timer_array[first_available], _cycle_duration, true);
    }    
}

void init_function_timer(non_blocking_timer timer_array[], unsigned int _end_after, callback_function event_callback, bool _repeating = false) {
    uint8_t first_available = scan_for_available_timers(timer_array);                
    timer_array[first_available].event_callback = event_callback;
    add_new_timer(&timer_array[first_available], _end_after, _repeating);
}

void update_timers(non_blocking_timer timer_array[]) {
    unsigned long int current_time = millis();

    for (uint32_t i = 0; i < non_blocking_timer::MAX_TIMERS; i++) {
        if (timer_array[i].finished == false && current_time - timer_array[i].start_time >= timer_array[i].end_after) {
            // timer has expired
            if (timer_array[i].event_callback != nullptr) { // execute the callback if that's how the timer is set up
                timer_array[i].event_callback();
            }
            else {
                digitalWrite(timer_array[i].ulPin, timer_array[i].ulVal); // change our pin value if that's how the timer is set up

                if (timer_array[i].repeating == true) {
                    timer_array[i].ulVal = !timer_array[i].ulVal; // flip the HIGH/LOW if a pin.
                }
            }

            if (timer_array[i].repeating == false) {
                timer_array[i].finished = true; // allow this to be used once again. 
            }
            else {
                timer_array[i].start_time = millis(); // reset timer                
            }
        }
    }
}