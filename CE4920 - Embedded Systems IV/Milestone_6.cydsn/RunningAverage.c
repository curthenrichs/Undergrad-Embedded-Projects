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

//=============================================================================
//                                  Libraries
//=============================================================================

#include "RunningAverage.h"
#include <stdlib.h>

//=============================================================================
//                         Public Function Implementation
//=============================================================================
 
/**
 * Running average object initalization function. Allocates the specified
 * number of bins into heap space. Note user should call the destroy function
 * to free this space. 
 * @param obj valid pointer to Running_Average object
 * @param num_bins is number of previous values to store. Must follow range.
 * @return true if allocated bins, else false due to failure
 */
bool running_average_init(Running_Average_t* obj, uint16_t num_bins){
    bool retVal = false;
    if(obj != NULL && num_bins < RUNNING_AVERAGE_BIN_MAX 
            && num_bins >= RUNNING_AVERAGE_BIN_MIN){
        obj->bins = (uint16_t*)(malloc(num_bins * sizeof(uint16_t)));
        if(obj->bins != NULL){
            retVal = true;
            obj->head = 0;
            obj->size = num_bins;
            for(uint16_t i=0; i<obj->size; ++i){
                obj->bins[i] = 0;   
            }
        }
    }
    return retVal;
}

/**
 * Running average object update function. Replaces oldest value with the
 * new supplied value.
 * @param obj valid pointer to Running_Average object
 * @param val is value to place into filter
 */
void running_average_update(Running_Average_t* obj, uint16_t val){
    if(obj != NULL && obj->bins != NULL && obj->head < obj->size
            && obj->size >= RUNNING_AVERAGE_BIN_MIN){
        obj->bins[obj->head] = val;
        obj->head = (++obj->head) % obj->size;
    }
}

/**
 * Running average object result calculation function. Calculates the average
 * of all values in the filter. Note that the result will default to zero
 * if the object is invalid. Additonally, at start of filter, due to lowpass
 * nature, the result will ramp up from zero to the average input value
 * @param obj valid pointer to Running_Average object
 * @return average result from the filter
 */
uint16_t running_average_result(Running_Average_t* obj){
    uint16_t retVal = 0;
    if(obj != NULL && obj->bins != NULL 
            && obj->size >= RUNNING_AVERAGE_BIN_MIN){
        float sum = 0;
        for(uint16_t i=0; i<obj->size; ++i){
            sum += obj->bins[i];   
        }
        retVal = (uint16_t)(sum / obj->size);
    }
    return retVal;
}

/**
 * Running average clear function will set all bins to zero.
 * @param obj valid pointer to Running_Average object
 */
void running_average_clear(Running_Average_t* obj){
    if(obj != NULL && obj->bins != NULL){
        for(uint16_t i=0; i<obj->size; ++i){
            obj->bins[i] = 0;   
        }
    }
}

/**
 * Running average destroy function will free heap memory and clear
 * data members
 * @param obj valid pointer to Running_Average object
 */
void running_average_destroy(Running_Average_t* obj){
    if(obj != NULL && obj->bins != NULL){
        free(obj->bins);
        obj->bins = NULL;
        obj->head = 0;
        obj->size = 0;
    }
}