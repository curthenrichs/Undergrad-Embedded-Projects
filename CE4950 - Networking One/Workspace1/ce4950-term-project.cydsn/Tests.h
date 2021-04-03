/**
 * Tests
 * @author Curt Henrichs
 * @date 1-11-18
 *
 * Tests module encapsulates the test cases applied to the term project. 
 * Each test case is build with reference to a written test plan corresponding
 * to the enumerated week.
 *
 * Before building the application one should verify the TEST_CASE 
 * configuration parameter is valid. Additionally, ensure all necessary 
 * hardware and system drivers are initialized outside of this module.
 */

#ifndef TESTS_H
#define TESTS_H
    
//=============================================================================
//                         Constant and Macro Definition
//=============================================================================
    
/**
 * Test cases written against the enumerated week's test plan.
 */
typedef enum Test_Cases {
    TEST_CASE_WEEK_3,   //! Channel monitor test
    TEST_CASE_WEEK_5,   //! Transmitter test
    TEST_CASE_WEEK_6,   //! Transmitter + channel monitor test
    TEST_CASE_WEEK_8,   //! Receiver without header
    TEST_CASE_WEEK_9,   //! Data link layer (interoperability part 1)
    TEST_CASE_WEEK_10   //! Final system test (iteroperability part 2)
} Test_Cases_e;
    
//=============================================================================
//                               Test Configuration
//=============================================================================
    
/**
 * Configuration parameter to build for a certain test case. Note only one 
 * case can be run at a time.
 */
#define CURRENT_TEST (TEST_CASE_WEEK_10)
    
//=============================================================================
//                           Public Function Prototypes
//=============================================================================

/**
 * Testing module main function will run selected test week
 * @return --ignore
 */
int tests_main(void);
/** 
 * Test for week 3 is to determine functionality of the channel monitor
 */
void tests_week3(void);
/**
 * Test for week 5 is a state-machine with user driven menu to select between
 * several test cases for transmitter functionality tests.
 */
void tests_week5(void);
/** 
 * Test week 6 will verify correct behavior for interaction between channel
 * monitor and transmission module.
 */
void tests_week6(void);
/**
 * Test week 8 will test the developed receiver module for valid interpretation
 * of characters. Note that header information is not handled.
 */
void tests_week8(void);
/**
 * Test week 9 will prove basic interoperability and compliance to CSMA/CD 
 * protocol using the expected data headers.
 */
void tests_week9(void);
/**
 * Test week 10 will validate final system drivers for full compliance to
 * protocol.
 */
void tests_week10(void);

#endif
    