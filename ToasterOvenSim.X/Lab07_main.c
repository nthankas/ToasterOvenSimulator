// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stddef.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Oled.h"
#include "Ascii.h"
#include "Leds.h"
#include "OledDriver.h"
#include "Buttons.h"
#include "Adc.h"    


// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define LONG_PRESS 1


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
} OvenState;

typedef struct {
    OvenState state;
    //add more members to this struct
    int cookingTimeLeft;
    int initialCookTime;
    int temperature;
    int buttonPressTime;
    int cookmode;
    int settings_selector;
} OvenData;

// **** Declare any datatypes here ****

typedef enum {
    TIME, TEMP
} settings;

typedef enum {
    ADC_CHANGED = 13, TIMER_TICK = 14
} newevents;

// **** Define any module-level, global, or external variables here ****
int runningTime;
int startTime;
int minutes;
int seconds;
int elapsedTime;
uint8_t ledCount = 0xFF;
OvenData initial1;
uint8_t event;
uint8_t ADCevent;
uint16_t ADCreading;
uint8_t ADCreadingU;

// **** Put any helper functions here ****


/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData){
    //update OLED here
    char buffer[200] = "";
    minutes = ovenData.cookingTimeLeft / 60;
    seconds = ovenData.cookingTimeLeft % 60;
    if (ovenData.cookmode == 0) {
        if (ovenData.settings_selector == TIME) {
            if (seconds >= 10) {
                sprintf(buffer, "Mode: Bake \n>Time: %d:%d\n Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
            else {
                sprintf(buffer, "Mode: Bake \n>Time: %d:0%d\n Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
        }
        else if (ovenData.settings_selector == TEMP) {
            if (seconds >= 10) {
                sprintf(buffer, "Mode: Bake \n Time: %d:%d\n>Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
            else {
                sprintf(buffer, "Mode: Bake \n Time: %d:0%d\n>Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
        }
    }
    else if (ovenData.cookmode == 1) {
        if (seconds >= 10) {
            sprintf(buffer, "Mode: Toast \n Time: %d:%d\n            ", minutes, seconds);
        }
        else {
            sprintf(buffer, "Mode: Toast \n Time: %d:0%d\n            ", minutes, seconds);
        }
    }
    else if (ovenData.cookmode == 2) {
        ovenData.temperature = 500;
        if (seconds >= 10) {
            sprintf(buffer, "Mode: Broil \n Time: %d:%d\n Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
        }
        else {
            sprintf(buffer, "Mode: Broil \n Time: %d:0%d\n Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
    }
    }
    OledDrawString(buffer);
    OledUpdate();
    }

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    //write your SM logic here.
    switch (initial1.state) {
        case SETUP: {
            if (initial1.settings_selector == TEMP) {
                ADCreadingU = ADCreading >> 2;
                initial1.temperature = ADCreadingU;
                initial1.temperature += 300;
            }
            else if (initial1.settings_selector == TIME) {
                ADCreadingU = ADCreading >> 2;
                initial1.initialCookTime = ADCreadingU;
                initial1.initialCookTime += 1;
            }
            initial1.cookingTimeLeft = initial1.initialCookTime;
            updateOvenOLED(initial1);
            break;
        }
        case SELECTOR_CHANGE_PENDING: {
            elapsedTime = runningTime - startTime;
            if (elapsedTime < LONG_PRESS * 5) {
                if (initial1.cookmode == 2) {
                    initial1.cookmode = 0;
                }
                else {
                    initial1.cookmode += 1;
                }
                updateOvenOLED(initial1);
            }
            else {
                if (initial1.cookmode == 0) {
                    if (initial1.settings_selector == TIME) {
                        initial1.settings_selector = TEMP;
                    }
                    else if (initial1.settings_selector == TEMP) {
                        initial1.settings_selector = TIME;
                    }
                }
                updateOvenOLED(initial1);
            }
            break;
        }

        case RESET_PENDING: {
            elapsedTime = runningTime - startTime;
            if (elapsedTime >= LONG_PRESS * 5) {
                initial1.state = SETUP;
                updateOvenOLED(initial1);
            }
            else {
                updateOvenOLED(initial1);
            }
            break;
        }

        case COOKING: {
            elapsedTime = runningTime - startTime;
            initial1.cookingTimeLeft = initial1.initialCookTime - (elapsedTime / 5);
//            double x8 = (double)(initial1.initialCookTime);
//            double x7 = (7.0/8.0) * (double)(initial1.initialCookTime);
//            double x6 = (6.0/8.0) * (double)(initial1.initialCookTime);
//            double x5 = (5.0/8.0) * (double)(initial1.initialCookTime);
//            double x4 = (4.0/8.0) * (double)(initial1.initialCookTime);
//            double x3 = (3.0/8.0) * (double)(initial1.initialCookTime);
//            double x2 = (2.0/8.0) * (double)(initial1.initialCookTime);
//            double x1 = (1.0/8.0) * (double)(initial1.initialCookTime);
//            double x0 = (0.0) * (double)(initial1.initialCookTime);
//            
            if (initial1.cookingTimeLeft > 0) {
//                if ((initial1.cookingTimeLeft / 5) >= x8) {
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x7) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x6) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x5) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x4) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x3) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x2) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) >= x1) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                }
//                else if ((initial1.cookingTimeLeft / 5) == x0) {
//                    ledCount = ledCount << 1;
//                    LEDS_SET(ledCount);
//                } 
                if (((elapsedTime * 5 * 5 * 5) % 8) == 0) {
                    ledCount = ledCount >> 1;
                    LEDS_SET(ledCount);
                }
            updateOvenOLED(initial1);  
            }
            else {
                initial1.state = SETUP;
                initial1.cookingTimeLeft = initial1.initialCookTime;
                ledCount = 0xFF;
                updateOvenOLED(initial1);
            }
            break;
        }

    }
}


int main()
{
    BOARD_Init();
    OledInit();
    ButtonsInit();
    AdcInit();
    LEDS_INIT();
     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
    
   
    printf("Welcome to CRUZID's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    initial1.state = SETUP;
    initial1.temperature = 350;
    initial1.initialCookTime = 1;
    initial1.settings_selector = 0;
    initial1.cookmode = 0;
    updateOvenOLED(initial1);   

    while (1){
        // Add main loop code here:
        // check for event;
        // on event, run runOvenSM()
        // clear event flags
        switch (event) {
            case ADC_CHANGED: {
                initial1.state = SETUP;
                ADCreading = AdcRead();
                runOvenSM();
                event = FALSE;
                break;
            }
            case BUTTON_EVENT_3DOWN: {
                startTime = runningTime;
                event = FALSE;
                break;
            }
            case BUTTON_EVENT_3UP: {
                initial1.state = SELECTOR_CHANGE_PENDING;
                runOvenSM();
                event = FALSE;
                break;
            }
            case BUTTON_EVENT_4DOWN: {
                startTime = runningTime;
                if (initial1.state == COOKING) {
                    initial1.state = RESET_PENDING;
                    event = FALSE;
                }
                else {
                    initial1.state = COOKING;
                    runOvenSM();
                    event = FALSE;
                }
                break;
            }
            case BUTTON_EVENT_4UP: {
                if (initial1.state == RESET_PENDING) {
                    initial1.state = COOKING;
                    event = FALSE;
                }
                break;
            }
            case TIMER_TICK: {
                if (initial1.state == RESET_PENDING) {
                    runOvenSM();
                    event = FALSE;
                }
                if (initial1.state == COOKING) {
                    initial1.state = COOKING;
                    runOvenSM();
                    event = FALSE;
                }
                break;
            }
        };
    }
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    //add event-checking code here
    runningTime += 1;
    event = TIMER_TICK;
    
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
    event = ButtonsCheckEvents();
    if (AdcChanged() == TRUE){
        event = ADC_CHANGED;
    }
}