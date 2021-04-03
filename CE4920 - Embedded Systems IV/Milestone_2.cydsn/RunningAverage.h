/**
 * RunningAverage
 * @author Curt Henrichs
 * @date 9-17-17
 *
 * Running average FIR filter object. Goal of this module is to provide an
 * object that takes in sensor data and through an averaging FIR filter, 
 * produce a lowpass average. Module allows user to specify number of bins
 * which are then constructed in heap space.
 */

#ifndef RUNNINGAVERAGE_H
#define RUNNINGAVERAGE_H
    
//=============================================================================
//                                  Libraries
//=============================================================================

#include <stdint.h>
#include <stdbool.h>

//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

#define RUNNING_AVERAGE_BIN_MAX   256   //! Maximum number of bins
#define RUNNING_AVERAGE_BIN_MIN   1     //! Minimum number of bins
    
//=============================================================================
//                          Data Structure Declaration
//=============================================================================
    
/**
 * Running average FIR filter object structure.
 */
typedef struct Running_Average {
    uint16_t* bins; //! Array of previous values
    uint16_t size;  //! Size of the array
    uint16_t head;  //! Next value position
} Running_Average_t;

//=============================================================================
//                         Public Function Prototypes
//=============================================================================
    
/**
 * Running average object initalization function. Allocates the specified
 * number of bins into heap space. Note user should call the destroy function
 * to free this space. 
 * @param obj valid pointer to Running_Average object
 * @param num_bins is number of previous values to store. Must follow range.
 * @return true if allocated bins, else false due to failure
 */
bool running_average_init(Running_Average_t* obj, uint16_t num_bins);
/**
 * Running average object update function. Replaces oldest value with the
 * new supplied value.
 * @param obj valid pointer to Running_Average object
 * @param val is value to place into filter
 */
void running_average_update(Running_Average_t* obj, uint16_t val);
/**
 * Running average object result calculation function. Calculates the average
 * of all values in the filter. Note that the result will default to zero
 * if the object is invalid. Additonally, at start of filter, due to lowpass
 * nature, the result will ramp up from zero to the average input value
 * @param obj valid pointer to Running_Average object
 * @return average result from the filter
 */
uint16_t running_average_result(Running_Average_t* obj);
/**
 * Running average clear function will set all bins to zero.
 * @param obj valid pointer to Running_Average object
 */
void running_average_clear(Running_Average_t* obj);
/**
 * Running average destroy function will free heap memory and clear
 * data members
 * @param obj valid pointer to Running_Average object
 */
void running_average_destroy(Running_Average_t* obj);

#endif
    