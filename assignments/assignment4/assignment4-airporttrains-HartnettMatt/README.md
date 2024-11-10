[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Q-Urnqos)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=16544104&assignment_repo_type=AssignmentRepo)
# Assignment 4 - Airport_Trains
Author: Matt Hartnett

Target Device: STM32 Nucleo-64 Development Board

Date: 10/15/2024

## Project Overview:
The purpose of this assignment is to develop a train monitoring system for an airport, utilizing the STM32F091RC microcontroller on the Nucleo-64 board. The focus is on implementing a finite state machine to control an external LED that indicates the train's location and movement between different stops—Terminal, Concourse A, Concourse B, and a storage shed—by varying its brightness and blink patterns. Additionally, the assignment involves handling emergency stop functionality and using PWM to control the LED's brightness transitions. The system must operate in both normal and debug modes, with specific timing and logging requirements.

## Project Structure:
assignment4-airporttrains-HartnettMatt/

├── .github                   # GitHub-related configuration files <br>
├── .settings                 # Project settings and configuration files <br>
├── CMSIS                     # ARM CMSIS library files for STM32 <br>
├── Debug                     # Directory for debug build output <br>
├── Inc <br>
│   ├── stm32f0xx.h           # STM32F0xx series microcontroller header file <br>
│   ├── stm32f091xc.h         # STM32F091 microcontroller specific header file <br>
│   ├── system_stm32f0xx.h    # System-level configuration for STM32F0xx series <br>
│   └── utilities.h           # Utility functions and definitions header <br>
├── Lib                       # External libraries (currently empty) <br>
├── Release                   # Directory for release build output <br>
├── Src <br>
│   ├── button.c              # Source file for button handling logic <br>
│   ├── button.h              # Header file for button definitions and prototypes <br>
│   ├── eled.c                # Source file for external LED and PWM handling logic <br>
│   ├── eled.h                # Header file for external LED and PWM control definitions and prototypes <br>
│   ├── fsm.c                 # Source file for finite state machine implementation <br>
│   ├── fsm.h                 # Header file for finite state machine definitions and prototypes <br>
│   ├── log.h                 # Header file for logging functionality <br>
│   ├── main.c                # Main application entry point <br>
│   ├── syscalls.c            # System call stubs for the embedded system <br>
│   ├── sysmem.c              # Memory management functions for the embedded system <br>
│   ├── timer.c               # Source file for systick timer-related functionality <br>
│   ├── timer.h               # Header file for systick timer definitions and prototypes <br>
│   ├── uled.c                # Source file for user LED handling logic <br>
│   └── uled.h                # Header file for user LED control definitions and prototypes <br>
├── Startup                   # Startup files for initializing the microcontroller (currently empty) <br>
├── .cproject                 # Eclipse project configuration file <br>
├── .gitignore                # Git ignore file to exclude certain files from version control <br>
├── .project                  # Eclipse project metadata file <br>
├── assignment4-airporttrains-HartnettMatt Debug.launch    # Debug build configuration for the project <br>
├── assignment4-airporttrains-HartnettMatt Release.launch  # Release build configuration for the project <br>
├── README.md                 # Project README with information about the assignment <br>
├── State Machine.drawio.pdf  # PDF of the state machine diagram for the project <br>
└── STM32F091RCTX_FLASH.ld    # Linker script for STM32F091 microcontroller <br>



Notes:
* ChatGPT was used to create this project structure display. It was not used in any other part of this project.
* Certain sections of code were taken from Alex Dean's github (https://github.com/alexander-g-dean). All relevant sections are marked as such.
* The certain files have been taken and modified from assignment 3. I was the original author then and now. This is the complete list of files:
    * button.h and button.c
    * log.h
    * uled.h and uled.c

## Operation Guide:
In order to run this project, a Nucleo-F091RC board is required, along with the Cube32 IDE. When opening the project in the Cube32 IDE, several options are available to run the project. The two main ways to run it are saved as different run configurations:
### Release:
This run configuration has all of the correct timing, and does not use any printf statements (blocked by #ifdef DEBUG). If you would like to run the project and observe the timing-accurate version, run this configuration.
### Debug:
This run configuration has several printf statements within it to help debug and observe what is occuring. These debug statements do influence the timing of the project, so the delays are not precise with this run configuration. Additionally, the timing of the state diagram is modified for faster debugging (view state machine and assignment documentation). The printf statements can be observed in a serial console, configured to have 9600 baud, 1 stop bit, no parity, and no flow control.
