# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\henrichsc\Desktop\School\Senior_year\fall\CE4920\lab\Milestone_1.cydsn\Milestone_1.cyprj
# Date: Thu, 14 Sep 2017 14:01:23 GMT
#set_units -time ns
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {CapSense_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 3 5} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_clock -name {CapSense_IntClock(fixed-function)} -period 83.333333333333329 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_0}]
create_generated_clock -name {PWM_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 47 95} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_clock -name {PWM_CLK(fixed-function)} -period 1958.3333333333333 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_1}]
create_generated_clock -name {Tone_F_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 601 1201} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {Tone_D_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 12001 24001} [list [get_pins {ClockBlock/dclk_glb_3}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\henrichsc\Desktop\School\Senior_year\fall\CE4920\lab\Milestone_1.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\henrichsc\Desktop\School\Senior_year\fall\CE4920\lab\Milestone_1.cydsn\Milestone_1.cyprj
# Date: Thu, 14 Sep 2017 14:01:11 GMT
