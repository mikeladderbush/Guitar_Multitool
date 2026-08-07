#include <stdint.h>

#define RCC_IOPENR_REG (*(volatile uint32_t *)0x40021034)

#define GPIOB_MODER_REG (*(volatile uint32_t *)0x50000400)
#define GPIOB_BSRR_REG (*(volatile uint32_t *)0x50000418)
#define GPIOB_PUPDR_REG (*(volatile uint32_t *)0x5000040C)
#define GPIOB_IDR_REG (*(volatile uint32_t *)0x50000410)

uint8_t cursor = 0; // Initial cursor state.
uint8_t selection = 0; // Selection boolean.
uint8_t prev_button1 = 1;
uint8_t prev_button2 = 1;
uint8_t prev_button3 = 1;
uint8_t cursor_changed = 0;
uint8_t frame_buffer[384]; // For mapping display bytes to screen.
uint8_t exit_prog = 0;

struct Settings {

    uint8_t note_speed;
    uint8_t prog_speed;

};

typedef struct Settings Settings;

void set_pixel(uint8_t x, uint8_t y, uint8_t on){
    // Add gates for out of bounds/incorrect params
    // Adds a pixel with from 6,64 to the frame buffer.
}

void flush_buffer(){
    // push buffer to the I2C
}

void clear_buffer(){
    for (int i = 0; i < 384; i++) frame_buffer[i] = 0;
}

void draw_char(uint8_t char_num, uint8_t x, uint8_t y){
    // uses set_pixel() in a loop
}

void draw_string(char characters[], uint8_t x, uint8_t y){
    // Loop over characters using draw_char for each.
}

typedef enum { A, A_SHARP, B_FLAT, B, C, C_SHARP, D_FLAT, D, D_SHARP, E_FLAT, E, F, F_SHARP, G_FLAT, G, last} NOTES;

/*

    Randomness generator.

*/
NOTES generate_note(){

    /*
    
        1. Enable ADC clock: in RCC
        2. Enable the ADC's internal voltage regulator
        3. Run calibration, set calibration bit, poll until ready flag.
        4. Enable the ADC.
        5. Select the channel
        6. Set sampling time.
        7. Start a conversion.
        8. Poll the end-of-conversion flag
        9. Read the data register.
    
    */ 

    return last;
}

/*

    This runs the note test functionality until a user exit input.

*/
void note_test(Settings* settings){
    // Go back on exit pressed.

    for (;;){
        uint8_t now_button3 = (GPIOB_IDR_REG >> 2) & 0b1u;
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;



        if(exit_prog == 1){
            exit_prog = 0;
            return;
        }
    }
}

/*

    Functionality for chord progressions until user exit.

*/
void chord_progression(Settings* settings){
    // Go back on exit pressed.
    for (;;){
        uint8_t now_button3 = (GPIOB_IDR_REG >> 2) & 0b1u;
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;

        if(exit_prog == 1){
            exit_prog = 0;
            return;
        }
    }
}

/*

    Settings
    
    Will change a global settings object eventually.

*/
void set_settings(Settings* settings){ 
    // Go back on exit pressed.
    for (;;){
        uint8_t now_button3 = (GPIOB_IDR_REG >> 2) & 0b1u;
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;

        if(exit_prog == 1){
            exit_prog = 0;
            return;
        }
    } 
}


int main(void){

    Settings* settings;

    // PB6 and PB9 are I2C1_SCL and I2C1_SDA respectively.

    RCC_IOPENR_REG |= (0b1u << 1); // Sets bit for GPIOB

    GPIOB_MODER_REG &= ~(0b11u << 0); // clears 0-1 for pin 0
    GPIOB_MODER_REG &= ~(0b11u << 2); // clears 2-3 for pin 1
    GPIOB_MODER_REG &= ~(0b11u << 4); // clears 4-5 for pin 2

    GPIOB_PUPDR_REG &= ~(0b11u << 0);
    GPIOB_PUPDR_REG &= ~(0b11u << 2);
    GPIOB_PUPDR_REG &= ~(0b11u << 4);
    GPIOB_PUPDR_REG |= (0b01u << 0); // set pull-up on pin 0
    GPIOB_PUPDR_REG |= (0b01u << 2); // set pull-up on pin 1 
    GPIOB_PUPDR_REG |= (0b01u << 4); // set pull-up on pin 2

    for(;;){

        uint8_t now_button1 = (GPIOB_IDR_REG >> 0) & 0b1u;
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
                    // draw_string("Note Test")
                    break;
                case 1:
                    // draw_string("Progression")
                    break;
                case 2:
                    // draw_string("Settings")
                    break;
            }
            flush_buffer();
        }
        
        uint8_t now_button2 = (GPIOB_IDR_REG >> 1) & 0b1u;
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

        uint8_t now_button3 = (GPIOB_IDR_REG >> 2) & 0b1u;
        if (now_button3 == 0 && prev_button3 == 1){
            exit_prog = 1;
        }
        prev_button3 = now_button3;
        // Button 3 noop for now.
    }
}

