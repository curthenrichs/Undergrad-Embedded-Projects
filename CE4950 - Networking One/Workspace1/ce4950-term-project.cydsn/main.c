/**
 * Main
 * @author Curt Henrichs
 * @date 1-11-18
 *
 * Main module is application hook in memory. This module will initialize
 * hardware and drivers for selected application.
 *
 * Application ran is demo app for term project unless, in configuration, the 
 * testing build option is selected. If selected then the program will load 
 * the selected test application.
 */

//=============================================================================
//                               Configuration
//=============================================================================

/**
 * Configurable application, select either to run tests or run application
 */
#define TESTING_BUILD (0)

//=============================================================================
//                                 Libraries
//=============================================================================

#include "project.h"

#if TESTING_BUILD
    #include "Tests.h"
#else
    #include "Application.h"
#endif

//=============================================================================
//                                   MAIN
//=============================================================================

/**
 * Initialize hardware and drivers. Run test or demo application.
 * @return --ignore
 */
int main(void){
    
    // run selected main program
#if TESTING_BUILD
    return tests_main();
#else 
    return app_main();
#endif
}
