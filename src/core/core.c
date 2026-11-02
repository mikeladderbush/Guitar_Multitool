#include <stdlib.h>

#include "core.h"
#include "reg_manip.h"

/*
*********************************************************************************************

    Tools for drawing to the display.

*********************************************************************************************
*/
void set_pixel(uint8_t x, uint8_t y, uint8_t on){
    // TODO: Add gates for out of bounds/incorrect params
    // TODO: Adds a pixel with from 6,64 to the frame buffer.
}

void flush_buffer(){
    // TODO: push buffer to the I2C
}

void clear_buffer(){
    for (int i = 0; i < 384; i++) frame_buffer[i] = 0;
}

void draw_char(uint8_t char_num, uint8_t x, uint8_t y){
    // TODO: uses set_pixel() in a loop
}

void draw_string(char characters[], uint8_t x, uint8_t y){
    // TODO: Loop over characters using draw_char for each.
}


/*
*********************************************************************************************

    Random note generation.

*********************************************************************************************
*/
typedef enum { A, A_SHARP, B_FLAT, B, C, C_SHARP, D_FLAT, D, D_SHARP, E_FLAT, E, F, F_SHARP, G_FLAT, G} NOTES;
const char *note_names[15] = {"A", "A#", "Bb", "B", "C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G"};
const char* enum_to_string(NOTES note){
    return note_names[note];
}
NOTES generate_note(){
    return (NOTES)(rng_next() % 15);
}


/*
*********************************************************************************************

    Functionality options selected through the main menu.

*********************************************************************************************
*/
typedef struct {

    uint8_t note_speed;
    uint8_t prog_speed;
    uint8_t chords_per_prog;

} Settings;

void note_test(Settings* settings){
    NOTES note;
    const char* note_string;

    for (;;){
        uint8_t now_button3 = check_button(2);
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;

        note = generate_note();
        note_string = enum_to_string(note);
        // TODO: draw_string(note_string, x, y)

        if(exit_prog == 1){
            exit_prog = 0;
            return;
        }
    }
}

void chord_progression(Settings* settings){
    for (;;){
        uint8_t now_button3 = check_button(2);
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;

        uint8_t speed = settings->prog_speed;
        uint8_t progression_length = rng_next() % settings->chords_per_prog;

        while(exit_prog != 1){
            if (now_button3 == 0 && prev_button3 == 1){
                exit_prog = 1;
            }
            // Draw current progression chord
            // sleep(speed);
        }

        if(exit_prog == 1){
            exit_prog = 0;
            return;
        }
    }
}

void set_settings(Settings* settings){ 
    for (;;){
        uint8_t now_button3 = check_button(2);
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;
        // Display current setting 1, let button 1 change, button 2 move to next setting.
        // setting 2, 3, ...

        if(exit_prog == 1){
            exit_prog = 0;
            return;
        }
    } 
}