/**
 * Heartbeat
 * @author Curt Henrichs
 * @date 9-17-17
 *
 * Heartbeat module is a software module tasked with collecting heartbeat
 * rate from the user for the treadmill application. Heartbeat module operates
 * via interrupts. Interrupts are on ADC (for waveform recreation), on counter
 * (for overflow, signalling no pulse), and on an analog comparator (to detect
 * heartbeat peak and therefore measure frequency).
 */

//=============================================================================
//                                  Libraries
//=============================================================================
   
#include "Heartbeat.h"
#include <project.h>
#include "RunningAverage.h"

//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

#define HB_MUX_INTERNAL_CH 0 //! Heartbeat internal channel connected to DAC
#define HB_MUX_EXTERNAL_CH 1 //! Heartbeat external channel connected to pin

#define HB_COMP_HEARTBEAT_THRESHOLD_VALUE 100 //! DAC value for comparator //125

#define HEARTBEAT_FILTER_SIZE 10 //! Filter bins for running average

#define ADC_TO_DAC_SCALER 1.6f //! Scaler to convert ADC range to DAC range

#define MAX_POSSIBLE_HEARTBEAT 300 //! Highest heartbeat allowed, else zero

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * FIR filter of BPM to remove noise in measurement
 */
static volatile Running_Average_t _filter;

static volatile bool _use_led_state = false;

//=============================================================================
//                         Interrupt Handler Prototypes
//=============================================================================

/**
 * Comparator ISR is triggered on rising edge of peak detection. Responsible
 * for calculating current beats per minute and storing that result into
 * running average. Resets counter to zero for next comparison.
 */
CY_ISR_PROTO(comp_isr);
/**
 * ADC EOC ISR is triggered on end of conversion event. Responsible for reading
 * result from ADC, scaling it, and recreating it on the DAC.
 */
CY_ISR_PROTO(ADC_EOC_isr);
/**
 * Counter overflow ISR is triggered when counter register overflows. This
 * interrupt is a fail-safe to record zero heartbeat. 
 */
CY_ISR_PROTO(counter_overflow_isr);

//=============================================================================
//                         Public Function Implementation
//=============================================================================
  
/**
 * Heartbeat module intialization function will start all module hardware and
 * intialize all private attributes. This module is entirely driven by
 * interrupts so it is crucial that global interrupts are enabled.
 * @param source is the heartbeat input source for analog MUX channel select
 */
void heartbeat_init(Heartbeat_Source_e source){
    
    //initalize attributes
    running_average_init(&_filter,HEARTBEAT_FILTER_SIZE);
    
    //start analog output hardware
    HB_Comp_Neg_DAC_Start();
    HB_Comp_Neg_DAC_SetValue(HB_COMP_HEARTBEAT_THRESHOLD_VALUE);
    HB_DAC_Start();
    HB_DAC_SetValue(0);
    
    //start analog input hardware
    HB_ADC_Start();
    HB_ADC_StartConvert();
    HB_ADC_IRQ_Enable();
    HB_Comp_Start();
    HB_Counter_Start();
    
    //select source and start
    HB_MUX_Start();
    HB_PGA_Start();
    if(source == HEARTBEAT_SOURCE_INTERNAL){
        //internal state
        I_HB_Start();
        HB_MUX_FastSelect(HB_MUX_INTERNAL_CH);
    }else if(source == HEARTBEAT_SOURCE_EXTERNAL){
        //external state
        HB_MUX_FastSelect(HB_MUX_EXTERNAL_CH);
    }else{
        //off state uses turned off DAC for 0V input
        HB_MUX_FastSelect(HB_MUX_INTERNAL_CH);
    }
    
    //start interrupts
    HB_ADC_IRQ_StartEx(ADC_EOC_isr);
    HB_Comp_Int_StartEx(comp_isr);
    HB_Overflow_Int_StartEx(counter_overflow_isr);
}

/**
 * @return heartbeat beats per minute as standard measure of user's heartrate
 */
uint16_t heartbeat_get_bpm(void){
    return running_average_result(&_filter);
}

/**
 * Clears the previous heartbeat data, thereby reseting the module
 * Note hardware is not modified.
 */
void heartbeat_clear(void){
    running_average_clear(&_filter);
}

/**
 * Sets the use LED flag so that heartbeat is pulsed out
 * @param state, true if activate LED heartbeat, false if disable
 */
void heartbeat_set_use_LED(bool state){
    _use_led_state = state;
    if(!_use_led_state){
        LED_Write(0);     
    }
}

/**
 * @return gets the use LED flag state. true if using, false if disabled
 */
bool heartbeat_get_use_LED(void){
    return _use_led_state;
}
    
//=============================================================================
//                       Interrupt Handler Implementation
//=============================================================================

/**
 * Comparator ISR is triggered on rising edge of peak detection. Responsible
 * for calculating current beats per minute and storing that result into
 * running average. Resets counter to zero for next comparison.
 */
CY_ISR(comp_isr){
    //calculate the new heartrate in BPM
    uint16_t count = HB_Counter_ReadCounter(); //count is ms between peaks
    float bpm = 60000 / ((count > 0) ? (count) : (1)); // in BPM
    bpm = (bpm > MAX_POSSIBLE_HEARTBEAT) ? (0) : (bpm);
    running_average_update(&_filter,(uint16_t)(bpm));
    
    //clear source of interrupt
    HB_Counter_WriteCounter(0);
    HB_Comp_Int_ClearPending();
    
    //pulse LED to simulate heartbeat
    if(_use_led_state){
        LED_Write(!LED_Read());
    }
}

/**
 * ADC EOC ISR is triggered on end of conversion event. Responsible for reading
 * result from ADC, scaling it, and recreating it on the DAC.
 */
CY_ISR(ADC_EOC_isr){
    //get value and clear interrupt source
    uint8_t adc_val = HB_ADC_GetResult8();
    
    //send DAC new value
    HB_DAC_SetValue((uint8_t)(adc_val * ADC_TO_DAC_SCALER));
    HB_ADC_IRQ_ClearPending();
}

/**
 * Counter overflow ISR is triggered when counter register overflows. This
 * interrupt is a fail-safe to record zero heartbeat. 
 */
CY_ISR(counter_overflow_isr){
    //store zero to heartbeat
    running_average_update(&_filter,0);
    
    //clear source of interrupt
    HB_Counter_ReadStatusRegister();
    HB_Overflow_Int_ClearPending(); 
}
