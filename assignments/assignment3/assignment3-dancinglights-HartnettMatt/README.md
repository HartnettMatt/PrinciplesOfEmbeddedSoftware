[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/3ppgcYRu)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=16174685&assignment_repo_type=AssignmentRepo)
# Assignment 3 - DancingLights
Author: Matt Hartnett

Target Device: STM32 Nucleo-64 Development Board

Date: 09/29/2024

Certain files and sections of code have been take from Alex Dean's Embedded Systems Fundamental GitHub. All code sections that have been taken are marked with in-line or in-file comments that link to the specific part of the GitHub where they are found. The repo as a whole can be found here: https://github.com/alexander-g-dean/ESF

## Project Overview:
The point of this project was to understand how to control and read the GPIOs of the STM32 Nucleo-64 board. We accomplished this by controlling the user LED (ULED) and an external LED (ELED) with the button built onto the development board. Here is a basic rundown of the project:

Upon boot, the processor begins a startup sequence. First, all of the peripherals are configured and intialized, and then a timing sequence tests both of the LEDs. This timing sequence is as follows: ULED  ON for 2000 msec, OFF for 1000 msec, ELED ON for 2000 msec, OFF for 1000 msec, ULED ON for 2000 msec, OFF for 1000 msec. This boot sequence cannot be interrupted by the button.

After boot, the program infinitely runs two different sequences. The sequences can be switched between by pressing the button on the board. Sequence 1 is ELED ON for 1000 msec then OFF for 500 msec. ULED is OFF during this sequence. Sequence 2 is ULED ON for 3000 msec then OFF for 1500 msec. ELED is OFF during this sequence.

The button is polled every 100 milliseconds to ensure a responsive system. The delays are achieved using a NOP loop that delays the processor. I took several measurements to determine the correct conversion rate between the number of NOPs per 1 millisecond of operation (approximately 3200).

## Project Structure:
assignment3-dancinglights-HartnettMatt/
│ <br>
├── .github/ # GitHub configuration files <br>
├── .metadata/ # Project metadata <br>
├── .settings/ # Project settings <br>
├── CMSIS/ # CMSIS library files <br>
├── Debug/ # Debugging-related files <br>
├── Inc/ # Header files <br>
│ ├── button.h # Button handling <br>
│ ├── eled.h # LED control <br>
│ ├── stm32f0xx.h # STM32F0 series specific definitions <br>
│ ├── stm32f091xc.h # STM32F091xC specific definitions <br>
│ ├── system_stm32f0xx.h # STM32 system configuration <br>
│ ├── timer.h # Timer configuration and handling <br>
│ ├── uled.h # Utility LED definitions <br>
│ └── utilities.h # General utility functions <br>
├── Lib/ # External libraries <br>
├── Release/ # Release build files <br>
├── Src/ # Source files <br>
│ ├── button.c # Button handling implementation <br>
│ ├── eled.c # External LED control implementation <br>
│ ├── main.c # Main program file <br>
│ ├── syscalls.c # System call definitions <br>
│ ├── sysmem.c # System memory management <br>
│ ├── timer.c # Timer implementation <br>
│ └── uled.c # User LED functions implementation <br>
├── Startup/ # Startup configuration <br>
├── .cproject # Eclipse C project configuration <br>
├── .gitignore # Git ignore file <br>
├── .project # Eclipse project file
├── README.md # Project documentation (this file) <br>
└── STM32F091RCTX_FLASH.ld # Linker script for STM32F091 <br>

Note: ChatGPT was used to create this project structure display. It was not used in any other part of this project.

## Operation Guide:
In order to run this project, a Nucleo-F091RC board is required, along with the Cube32 IDE. When opening the project in the Cube32 IDE, several options are available to run the project. The two main ways to run it are saved as different run configurations:
### Release:
This run configuration has all of the correct timing, and does not use any printf statements (blocked by #ifdef DEBUG). If you would like to run the project and observe the timing-accurate version, run this configuration.
### Debug:
This run configuration has several printf statements within it to help debug and observe what is occuring. These debug statements do influence the timing of the project, so the delays are not precise with this run configuration. The printf statements can be observed in a serial console, configured to have 9600 baud, 1 stop bit, no parity, and no flow control.

## Compilation Information
As specified in the rubric for this assignment, several key details about the compilation were requested. Here are those pieces of information:

<b>Address of Main Function: </b> 0x080003dc<br>
<b>Size of Delay Function: </b> 112 bytes<br>
<b>Size of Text Segment: </b> 5112 bytes<br>
<b>Size of Data Segment: </b> 104 bytes<br>
<b>Inital Size of Stack: </b> 1024 bytes<br>

The sizes and addresses are as reported by the ARM version of objdump. The size of the stack was found in flash linker script.

# Review Comment 1
* Response
# Review Comment 2
* Response
# Review Comment 3
* Response
# Review Comment 4
* Response
# Review Comment 5
* Response
