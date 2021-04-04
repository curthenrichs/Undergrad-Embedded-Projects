/**
 * Music
 * @author Curt Henrichs
 * @date 9-10-17
 * 
 * Music module provides interface to the Piezo buzzer hardware to produce
 * frequency modulated, variable duration waveforms. An object is developed
 * to interface with the hardware and create music arrays. Finally all 
 * operations return control quickly to the caller as the hardware uses
 * interruts to handle note clockout.
 */

#ifndef MUSIC_H
#define MUSIC_H
       
//=============================================================================
//                                  Libraries
//=============================================================================
    
#include <stdint.h>
    
//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

#define NOTE_C      (262) //! Hz
#define NOTE_C_S    (277) //! Hz
#define NOTE_D      (294) //! Hz
#define NOTE_D_S    (311) //! Hz
#define NOTE_E      (330) //! Hz
#define NOTE_F      (349) //! Hz
#define NOTE_F_S    (370) //! Hz
#define NOTE_G      (392) //! Hz
#define NOTE_G_S    (415) //! Hz
#define NOTE_A      (440) //! Hz
#define NOTE_B      (494) //! Hz 
    
#define NOTE_PAUSE  (0)   //! No frequency, thus no sound

//=============================================================================
//                          Data Structure Declaration
//=============================================================================
    
/**
 * Note object describes the piezo note to play at a frequency and duration
 * By chainging these together in an array, one can generate a song
 */
typedef struct Note {
    uint16_t frequency;     //! Frequency of note in Hz
    uint16_t duration;      //! Duration of note in milliseconds
} Note_t;

//=============================================================================
//                         Public Function Prototypes
//=============================================================================
 
/**
 * Initalization function will start hardware, set ISRs for hardware, and
 * set internal data structures for playback.
 */
void music_init(void);
/**
 * Sets a tune to play. This is a non-blocking function so that other things
 * may happen. Most of the music generation is in hardware but a periodic 
 * interrupt is needed to update notes.
 * @param buffer is array of notes to play on the buzzer
 * @param length is the number of Notes in the buffer to play
 */
void music_set_tune(const Note_t* buffer, uint32_t length);
/**
 * Checks to see if the music is still playing
 * @return 1 if music is playing else 0
 */
uint8_t music_is_playing(void);
/**
 * Stops the music from playing then clears the music buffer assoication
 */
void music_stop_playing(void);

#endif
    