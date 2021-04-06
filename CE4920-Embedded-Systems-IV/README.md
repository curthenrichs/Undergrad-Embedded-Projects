# CE4920 Embedded Systems IV
The system developed for CE 4920, embedded systems IV, is a heartrate monitor and heartbeat
compensated treadmill using a Cypress PSOC 5 processor. This processor provides mixed-signal
customizable circuitry to signal condition and process analog inputs. The processor also provides
numerous prebuilt hardware implementations of common technology such as USB UART and CapSense
capacitive touch. For this application, the user uses a capacitive touch slider to wake the system and to
start exercise profile selection. After profile selection, the system will ramp up the treadmill motor to
the appropriate speed and start compensating the motor’s speed based on the heartrate measured. If
the heartrate rises then the motor is slowed down to not overtax the user, and vis-versa for decrease in
heartbeat. Finally, if the system detects a fault on any critical hardware devices then it will lock the
treadmill to ensure safety. This project aims to provide embedded IV students with a new development
environment and a new design challenge that requires the full embedded systems model with fault
tolerant considerations. The desired result is to achieve familiarity with the Cypress PSOC architecture in
addition to a functional embedded system.

See the [final report here](./Henrichs_final.pdf).


This is project is for archival and reference purposes. Code is not maintained. 
