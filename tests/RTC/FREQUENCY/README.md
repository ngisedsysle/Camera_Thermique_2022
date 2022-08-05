# TEST 22 : Frequency Check

The purpose of this test (c.f. TEST 22, document de tests unitaires et d'intégration) is to help measuring the RTC cristal frequency.

## How does it work ?

The RTC is first configured to echo back the clock signal to the MFP pin. This is done by sending 3 I2C commands (please refer to the main.c file for more info).
In parallel, TIMER 12 is confgured as "Input Capture direct mode" to record the "time" of each rising edge on pin PB14 (TIM12_CH1) (SPI2_MISO on AARDVARK connector).
Connecting both PB14 and MFP pins together, the program is now able to compute the time between two rising edges of the MFP signal. After a quick conversion, the frequency is printed once on the terminal via UART 7.


## Expected behavior

The output frequency should be close to 32.768 kHz, with 0.6 Hz precision.

## Annexes

- TIM12 clk source = APB1
- APB1 frequency = 40 Mhz
- APB1 clk source = HSE
- HSE frequency = 25 Mhz

- TIM12 PRESCALAR = 40 (starting from 1)(in CubeMx, write 40 - 1 since it starts from 0).
- TIM12 ARR = 0xffffffff (4294967295) 
- => TIM12 clk = APB1 frequency / (TIM12 PRESCALAR * ARR)  = 40 M / (40 * 4294967295) ~= 232 mHz.

TIM12 clk shall be lower than the given frequency precision (0.6 Hz in our case).
