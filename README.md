# LED_Exercise
## Test exercise for MikroTik:

Use STM32 or Atmel SAMD20 or Microchip PIC microcontroller to implement runngin light of at least 9 LEDs and one button that can change the dirrection of lights. UART can be used for debugging and LED speed control
Language: C and/or C++
Compiler: any free of charge, should run on Linux
Limitations: no libraries/SDK/HAL etc.
Use github/gitlab/bitbucket/sourceforge to save developement history

## Implementation:
Used will be STM32F030R8T6 microcontroller that is a part of Nucleo-64 developement kit
  -this is the only microcontroller that is available to me at this time

For developement Atollic TrueStudio for STM32 will be used is [here](https://www.st.com/en/development-tools/truestudio.html)  
Installation guide for Linux is [here](https://www.st.com/resource/en/user_manual/truestudio_installation_guide-truestudio-installation-guide-stmicroelectronics.pdf)

## Resources:
STM32F030R8T6 reference manual is [here](https://www.st.com/resource/en/reference_manual/dm00091010-stm32f030x4x6x8xc-and-stm32f070x6xb-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)  
NUCLEO-F030R8 pinout can be found [here](https://os.mbed.com/platforms/ST-Nucleo-F030R8/)

## DEMO:
A bit shaky but demonstration video of how code is running with 10 LEDs wired from NUCLEO board  
[DEMO](https://youtu.be/DdL53ePqL-g)

## Notes
* On NUCLEO board USART2 can't be used without changes in board as these pins are used by the STLink Virtual Comm Port (used for printf for example).
* Currently delay input is limited from 10ms - 999ms as it demonstrates input and sets this delay between LED changes.
* Other charaters can't bet inputed. If tried sollution will default to 500ms delay.
