README
======

  This README discusses issues unique to NuttX configurations for the
  STM32H743VIT-CT2022 board. The STM32H743VIT6 MCU is a 400MHz Cortex-M7 operation with 2MBytes Flash
  memory and 1MByte SRAM. The board features:

  - SPI screen FFC connector
  - USB type B 
  - SWD/JTAG
  - UART connector
  - DCMI FFC connector
  - SPI/I2C camera socket
  - 1 user LED
  - SPI aardvark connector
  - HSE oscillator 25 400MHz
  - external RTC with LSE oscillator 32.768 kHz
  - battery clips
  - 1 push button
  - SPI SD card reader

STATUS
======

  2018-06-16:  This port is still incomplete!  It is lacking the following
    at a minimum:

    - The RCC register definitions must still be updated per the STM32H7x3
      user manual.
    - The RCC-based clock configuration logic must be redesigned to support
      the STM32H7's RCC.
    - There is no U[S]ART support in arch/arm/src/stm32h7.  For the most
      part the U[S]ART is compatible with the STM32 F7, however, updates
      will be required to support:  (1) the new STM32 H7 clock prescaler,
      and (2) the new STM32 H7 Rx and Tx FIFOs.
    - There is a fragmentary NSH here.  However, it does not have any serial
      console selected and is not expected to be usable.

    This logic has set on a branch for some time with little progress.  I
    have decided to bring it into the master but only with the option to
    select the STM32 H7 architecture dependent on CONFIG_EXPERMIMENTAL.
    That should eliminate the possibility of anyone accidentally enabling
    this know incomplete port.

  2018-07-12:  After several commits focused primarily on the H7 RCC and
    serial driver, Mateusz Szafoni states the basic NSH configuration is
    now functional.
  2018-09-16:  I2C Driver added by Mateusz Szafoni.
  2018-10-02:  SPI Driver added by Mateusz Szafoni.
  2019-03-02:  DMA support contributed by Mateusz Szafoni.
