#include <stdint.h>
#include "core.h"

uint8_t cursor = 0; // Initial cursor state.
uint8_t selection = 0; // Selection boolean.
uint8_t prev_button1 = 1;
uint8_t prev_button2 = 1;
uint8_t prev_button3 = 1;
uint8_t cursor_changed = 0;

__attribute__((section(".cbuf"), aligned(4)))
uint8_t frame_buffer[384]; // For mapping display bytes to screen.
uint8_t exit_prog = 0;
uint32_t rng_state;

int main(void){

    init_buttons();
    init_display_conn();
    seed_rng();
    Settings* settings;

    for(;;){
        uint8_t now_button1 = check_button(0);
        if (now_button1 == 0 && prev_button1 == 1){
            cursor++;
            cursor_changed = 1;
            if (cursor > 2) cursor = 0;
        } 
        prev_button1 = now_button1;

        if (cursor_changed == 1){
            cursor_changed = 0;
            clear_buffer();
            switch(cursor){
                case 0:
                    // TODO: draw_string("Note Test")
                    break;
                case 1:
                    // TODO: draw_string("Progression")
                    break;
                case 2:
                    // TODO: draw_string("Settings")
                    set_settings(settings);
                    break;
            }
            flush_buffer();
        }
        
        uint8_t now_button2 = check_button(1);
        if (now_button2 == 0 && prev_button2 == 1){
            selection = 1;
        } 
        prev_button2 = now_button2;
        
        if (selection == 1){
            selection = 0;
            switch (cursor){
            case 0:
                note_test(settings);
                break;
            case 1:
                chord_progression(settings);
                break;
            case 2:
                set_settings(settings);
                break;
            }
        } 

        uint8_t now_button3 = check_button(2);
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;
        // TODO: Button 3 noop for now.
    }
}

