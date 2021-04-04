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

//=============================================================================
//                                  Libraries
//=============================================================================

#include "Music.h"
#include <project.h>

//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

#define TONE_F_CLK_FREQ_IN 40000    //! Clock frequency that is divided for out

//=============================================================================
//                             Private Attributes
//=============================================================================

static const Note_t* _current_buffer = NULL; //! Music buffer association
static uint32_t _current_buffer_length = 0;  //! Size of buffer
static uint32_t _current_buffer_index = 0;   //! Index into buffer clockout

//=============================================================================
//                         Interrupt Handler Prototypes
//=============================================================================

/**
 * Music note clock out ISR is linked to the duration counter. Only active
 * when music is actively being played
 */
CY_ISR_PROTO(music_isr);

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * Initalization function will start hardware, set ISRs for hardware, and
 * set internal data structures for playback.
 */
void music_init(void){
    Tone_F_Start();
    Tone_F_WriteCompare(0);
    Tone_D_Start();
    Tone_D_Stop(); //sets up hardware but then disables timer
    Tone_Int_StartEx(music_isr);
}

/**
 * Sets a tune to play. This is a non-blocking function so that other things
 * may happen. Most of the music generation is in hardware but a periodic 
 * interrupt is needed to update notes.
 * @param buffer is array of notes to play on the buzzer
 * @param length is the number of Notes in the buffer to play
 */
void music_set_tune(const Note_t* buffer, uint32_t length){
    Tone_Int_Disable();
    
    //copy buffer location for playback
    _current_buffer = buffer;
    _current_buffer_length = length;
    _current_buffer_index = 0;
    
    //set up timer counter
    Tone_D_WritePeriod(1);
    Tone_D_WriteCounter(0);
    Tone_Int_Enable();
    Tone_D_Start();
}

/**
 * Checks to see if the music is still playing
 * @return 1 if music is playing else 0
 */
uint8_t music_is_playing(void){
    return _current_buffer != NULL;   
}

/**
 * Stops the music from playing then clears the music buffer assoication
 */
void music_stop_playing(void){
    _current_buffer = NULL;
    Tone_F_WriteCompare(0);
    Tone_D_Stop();
}

//=============================================================================
//                       Interrupt Handler Implementation
//=============================================================================

/**
 * Music note clock out ISR is linked to the duration counter. Only active
 * when music is actively being played
 */
CY_ISR(music_isr){
    Tone_D_Stop();
    
    if(_current_buffer != NULL){
        if(_current_buffer_index >= _current_buffer_length){
            //end of tune
            _current_buffer = NULL;
            Tone_F_WriteCompare(0);
            Tone_D_Stop();
        }else{
            //play for current frequency and duration
            uint16_t freq = _current_buffer[_current_buffer_index].frequency;
            uint16_t per = TONE_F_CLK_FREQ_IN / ((freq > 0)? freq : 1);
            uint16_t cmp = (freq > 0) ? per / 2 : 0;
            uint16_t dur = _current_buffer[_current_buffer_index].duration;
            _current_buffer_index++;
            
            Tone_F_WritePeriod(per);
            Tone_F_WriteCompare(cmp);
            Tone_D_WritePeriod(dur);
            Tone_D_WriteCounter(0);
            Tone_D_Start();
        }
    }
    Tone_D_ReadStatusRegister();
    Tone_Int_ClearPending();
}