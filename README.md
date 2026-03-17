# Toaster Oven Simulator

Event-driven FSM simulation of a digital toaster oven on PIC32. Three cooking modes (Bake/Toast/Broil) with real-time OLED display, LED countdown bar, and potentiometer-based time/temperature input.

## Architecture

- **FSM states**: SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
- **Inputs**: BTN3 (mode cycle / selector toggle via short/long press), BTN4 (start cooking / cancel via long press), potentiometer (ADC for time or temp)
- **Display**: OLED shows current mode, time remaining (MM:SS), and temperature. 8 LEDs turn off sequentially as cooking progresses (one per 1/8th of total duration)
- **Timers**: Timer2 at 100Hz polls buttons and ADC. Timer3 at 5Hz drives the cooking countdown and state machine ticks

## Cooking Modes

- **Bake**: adjustable time + temperature (300-555F via ADC). Selector toggles between time and temp
- **Toast**: adjustable time only, no temp display
- **Broil**: fixed 500F, adjustable time only

## Target

PIC32MX795F512H. Build with MPLAB X -- open `ToasterOvenSim.X/`.
