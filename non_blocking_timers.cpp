#include "non_blocking_timers.h"

non_blocking_timer static_timer_first_in_list; // the first item in the list is an unused timer which acts as the static base
non_blocking_timer *last_timer_in_list = &static_timer_first_in_list;   // last timer in linked list, this will continually update as needed.

void add_new_generic_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, bool _repeating) {
    // initialize the timer 
    non_blocking_timer_ptr->finished = false;      
    non_blocking_timer_ptr->repeating = _repeating;      
    non_blocking_timer_ptr->end_after = _end_after;
    non_blocking_timer_ptr->start_time = millis();

    last_timer_in_list->next_timer_in_list = non_blocking_timer_ptr; // update the last timer to point to the new timer (even if its the first)
    non_blocking_timer_ptr->prev_timer_in_list = last_timer_in_list; // update the new timers prev timer
    last_timer_in_list = non_blocking_timer_ptr; // then update the last timer in the list
}

void delete_timer(non_blocking_timer *non_blocking_timer_ptr) {
    non_blocking_timer *next_timer = non_blocking_timer_ptr->next_timer_in_list;
    non_blocking_timer *prev_timer = non_blocking_timer_ptr->prev_timer_in_list;
    prev_timer->next_timer_in_list = next_timer; // link the previous item to the next item, thus removing the current item. 
    next_timer->prev_timer_in_list = prev_timer;    // if this happens to the first item in linked lust, it will copy the static base next timer which is nullptr
}

void init_digitalWrite_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, uint8_t _ulPin, uint8_t _ulVal) {
    pinMode(_ulPin, OUTPUT); // the pin must be an digital output
    non_blocking_timer_ptr->ulPin = _ulPin;
    non_blocking_timer_ptr->ulVal = _ulVal;
    add_new_generic_timer(non_blocking_timer_ptr, _end_after, false);
}

void init_digitalWrite_oscillator(non_blocking_timer *non_blocking_timer_ptr, unsigned int _cycle_duration, uint32_t _ulPin, uint32_t _start_ulVal) {
    pinMode(_ulPin, OUTPUT); // the pin must be an digital output
    non_blocking_timer_ptr->ulPin = _ulPin;
    non_blocking_timer_ptr->ulVal = _start_ulVal;
    add_new_generic_timer(non_blocking_timer_ptr, _cycle_duration, true);  
}

void init_function_timer(non_blocking_timer *non_blocking_timer_ptr, unsigned int _end_after, callback_function event_callback, bool _repeating) {
    non_blocking_timer_ptr->event_callback = event_callback;
    add_new_generic_timer(non_blocking_timer_ptr, _end_after, _repeating);
}

void update_timers(void) {
    unsigned long int current_time = millis();
    non_blocking_timer *timer_to_update = &static_timer_first_in_list;

    while (timer_to_update->next_timer_in_list != nullptr) {
        timer_to_update = timer_to_update->next_timer_in_list;

        if (timer_to_update->finished == false && current_time - timer_to_update->start_time >= timer_to_update->end_after) {
            // timer has expired
            if (timer_to_update->event_callback != nullptr) { // execute the callback if that's how the timer is set up
                timer_to_update->event_callback();
            }
            else {
                digitalWrite(timer_to_update->ulPin, timer_to_update->ulVal); // change our pin value if that's how the timer is set up

                if (timer_to_update->repeating == true) {
                    timer_to_update->ulVal = !timer_to_update->ulVal; // flip the HIGH/LOW if a pin.
                }
            }

            if (timer_to_update->repeating == false) {
                timer_to_update->finished = true; // allow this to be used once again.
                delete_timer(timer_to_update);
            }
            else {
                timer_to_update->start_time = millis(); // reset timer                
            }
        }      
    }
}