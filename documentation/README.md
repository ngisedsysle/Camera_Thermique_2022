# TO DO LIST
- [] USB
- [] SD
- [] Visible camera
- [] Coulombmeter & GPIO KILL
- [x] RTC
- [x] IR camera
- [x] Screen
- [x]  UART

# CHANGES   (we tried to put the most relevant here)

## STM32H747I-DISCO board
From Nuttx main repo, several adjustements and changes were made for the STM32H747I-discovery board:
Among them :

###### nuttx/arch/arm/src/stm32h7/hardware/stm32h7x7xx_<driver>.h 
- The exisiting drivers were only present for the stm32h7x3 boards. You could find each code under the name : stm32h7x3xx_<driver's name>. Thus, none of these files were existing for the stm32h7x7 boards. If you wanted to use them, you had to enable CONFIG_ARCH_STM32H7X3XX. To solve this issue, all the missing files were created with the same content.

###### nuttx/boards/arm/stm32h7/stm32h7i-disco/src/stm32_bringup.c
- Only few driver files are created at startup time. If you have enabled the CONFIG option for your driver but forgot to add the file creation step in stm32_bringup.c, you may encounter this error " ERROR : failed to open /dev/...) ".

###### nuttx/boards/arm/stm32h7/stm32h7i-disco/src/stm32h747i-disco.h
- Same thing. Not all driver init functions are present here. If not set, at compilation time you will meet the famous undefined reference.

###### FT80x
- ROM_CHIPID is no longer checked at FT80X init since it was entailing irrelevant errors. 
- There was also some mistakes in the naming : FT80X_INT_ENABLE = 0 when the comment just right above says the opposite... It should be 1 actually. This mistake was also encountered several times on the Internet. Be careful when copying.
- In many examples found for the screen, each time after the display buffer was sent, the  FT80X_REG_CMD_WRITE and FT80X_REG_CMD_READ registers were read continuously until they had the same value. This operation takes half a second to execute, which is way too much for the video quality. As time's running short, we put all these lines under comment :). From what we could see, the image quality wasn't affected while the frame rate/sec soared.

###### nuttx/arch/arm/src/stm32h7/stm32h7x7xx_rcc.c
- the content was replaced by an old commit from the main repo. With the last one (may 2022), an ``` #if 0 ``` was removed which entails failure at boot time (the program was stuck in an infinite loop in this same file). The ``` #if 0 ``` is there to jump over some necessary checks on how the board is powered. Since the mainline didn't provide any relevant support for that, we kept it. However, this issue does not always show up, but mostly when the mcu is already programmed with a diff OS (barematal vs NuttX).




