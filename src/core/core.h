#ifndef CORE_HEADER_H
#define CORE_HEADER_H

/*
*********************************************************************************************

    Tools for drawing to the display.

*********************************************************************************************
*/
void set_pixel(uint8_t x, uint8_t y, uint8_t on);
void flush_buffer();
void clear_buffer();
void draw_char(uint8_t char_num, uint8_t x, uint8_t y);
void draw_string(char characters[], uint8_t x, uint8_t y);

/*
*********************************************************************************************

    Random note generation.

*********************************************************************************************
*/
typedef enum { A, A_SHARP, B_FLAT, B, C, C_SHARP, D_FLAT, D, D_SHARP, E_FLAT, E, F, F_SHARP, G_FLAT, G} NOTES;
const char *note_names[15] = {"A", "A#", "Bb", "B", "C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G"};
const char* enum_to_string(NOTES note);
NOTES generate_note();

#endif