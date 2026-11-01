#ifndef F_CPU
#define F_CPU 4000000ULL  // Adjust this value to match your actual system clock frequency
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// Include the MCC generated system file to gain access to initialization routines
#include "mcc_generated_files/system/system.h" 

// Mapping index to pin (Index 0 = GPIO4, Index 1-6 = GPIO5-10)
void set_random_gpio_low(int index) {
    switch (index) {
        case 0: PORTD.OUTCLR = PIN4_bm; break; // GPIO4 (PD4)
        case 1: PORTD.OUTCLR = PIN5_bm; break; // GPIO5 (PD5)
        case 2: PORTD.OUTCLR = PIN6_bm; break; // GPIO6 (PD6)
        case 3: PORTD.OUTCLR = PIN7_bm; break; // GPIO7 (PD7)
        case 4: PORTF.OUTCLR = PIN0_bm; break; // GPIO8 (PF0)
        case 5: PORTF.OUTCLR = PIN1_bm; break; // GPIO9 (PF1)
        case 6: PORTC.OUTCLR = PIN3_bm; break; // GPIO10 (PC3)
    }
}

void set_random_gpio_high(int index) {
    switch (index) {
        case 0: PORTD.OUTSET = PIN4_bm; break; // GPIO4 (PD4)
        case 1: PORTD.OUTSET = PIN5_bm; break; // GPIO5 (PD5)
        case 2: PORTD.OUTSET = PIN6_bm; break; // GPIO6 (PD6)
        case 3: PORTD.OUTSET = PIN7_bm; break; // GPIO7 (PD7)
        case 4: PORTF.OUTSET = PIN0_bm; break; // GPIO8 (PF0)
        case 5: PORTF.OUTSET = PIN1_bm; break; // GPIO9 (PF1)
        case 6: PORTC.OUTSET = PIN3_bm; break; // GPIO10 (PC3)
    }
}

int main(void) {
    // Runs MCC setup to configure your Pull-ups & Pin Directions
    SYSTEM_Initialize(); 

    // Set all GPIO lines to their default inactive HIGH state
    PORTD.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTF.OUTSET = PIN0_bm | PIN1_bm;
    PORTC.OUTSET = PIN3_bm;
    
    srand(0x1234); 

    // Power-up sequence: Pull GPIO0 (PD0) low for 500ms
    PORTD.OUTCLR = PIN0_bm;
    _delay_ms(500);
    PORTD.OUTSET = PIN0_bm;

    int mc1_active = 0;
    int active_random_gpio = -1;

    while (1) {
        // Read the current status of PORTA (MC1 to MC8 inputs)
        uint8_t porta_val = PORTA.IN;

        // Check if MC1 (PA0) is pressed (Logic 0)
        uint8_t mc1_pressed = !(porta_val & PIN0_bm);

        // Check if any other MC lines (MC2 to MC8) are triggered
        uint8_t other_mc_triggered = ((porta_val & 0xFE) != 0xFE);

        // MC1 Logic Handling
        if (mc1_pressed && !other_mc_triggered) {
            if (!mc1_active) {
                // Generates a random number from 1 to 6, limiting choices strictly to GPIO5 to GPIO10
                active_random_gpio = (rand() % 6) + 1;
                set_random_gpio_low(active_random_gpio);
                mc1_active = 1;
            }
        } else {
            if (mc1_active) {
                set_random_gpio_high(active_random_gpio);
                mc1_active = 0;
                active_random_gpio = -1;
            }
        }

        // Check MC2 (PA1) -> Pulses GPIO1 (PD1) low for 500ms
        if (!(porta_val & PIN1_bm)) {
            PORTD.OUTCLR = PIN1_bm;
            _delay_ms(500);
            PORTD.OUTSET = PIN1_bm;
        }

        // Check MC3 (PA2) -> Pulses GPIO2 (PD2) low for 500ms
        if (!(porta_val & PIN2_bm)) {
            PORTD.OUTCLR = PIN2_bm;
            _delay_ms(500);
            PORTD.OUTSET = PIN2_bm;
        }

        // Check MC4 (PA3) -> Pulses GPIO3 (PD3) low for 500ms
        if (!(porta_val & PIN3_bm)) {
            PORTD.OUTCLR = PIN3_bm;
            _delay_ms(500);
            PORTD.OUTSET = PIN3_bm;
        }

        // Check MC5 (PA4) -> Pulses GPIO4 (PD4) low for 500ms
        if (!(porta_val & PIN4_bm)) {
            PORTD.OUTCLR = PIN4_bm;
            _delay_ms(500);
            PORTD.OUTSET = PIN4_bm;
        }
    }
}
