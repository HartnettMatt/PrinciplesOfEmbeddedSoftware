[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/HFnLWBYE)

# PES Assignment 2: FIFOs

## Assignment Overview

This repository contains Matt Hartnett's source code and related files for Assignment 2 of the ECEN5813: Programming
Embedded Systems course. The focus of this assignment is to implement two different types of FIFO (First-In, First-Out)
data structures and develop a proper GitHub project with a test harness, documentation, README, and Makefile.
Here’s the content formatted into a single `.md` file:

### FIFO Implementations:

1. **llfifo**: A linked-list based FIFO implementation, suitable for video processing or other high-bandwidth
   applications.
2. **cbfifo**: A circular-buffer based FIFO implementation, suitable for low-latency applications such as UART
   communication.

## Project Structure
- `cbfifo.c`: Circular-buffer FIFO implementation
- `cbfifo.h`: Header file for circular-buffer FIFO interface
- `llfifo.c`: Linked-list FIFO implementation
- `llfifo.h`: Header file for linked-list FIFO interface
- `main.c`: Main entry point of the program, responsible for initializing and running FIFO tests
- `Makefile`: Build script to compile the project and run tests
- `README.md`: Project documentation and instructions
- `test_cbfifo.c`: Unit tests for the circular-buffer FIFO implementation
- `test_cbfifo.h`: Header file for the circular-buffer FIFO tests
- `test_llfifo.c`: Unit tests for the linked-list FIFO implementation
- `test_llfifo.h`: Header file for the linked-list FIFO tests

## Building the Project

To build the project, simply run the following command in the project root:


```bash
make
```
This will compile the FIFO implementations as well as the test code.

## Running Tests

After building the project, you can run the test suite by executing:

```bash
./assgn2
```

This will run the unit tests for both `llfifo` and `cbfifo` implementations and display the results in the terminal.

## Implementations

### 1. **llfifo (Linked-list FIFO)**

- This FIFO is implemented using a dynamic linked-list, which allows it to expand as necessary.
- It is suitable for applications requiring large or dynamically sized buffers, such as video streaming.

### 2. **cbfifo (Circular-buffer FIFO)**

- This FIFO uses a circular buffer that overwrites old data when full, making it efficient for fixed-size buffers.
- It is ideal for UART communication where constant data flow is required with minimal memory usage.

## Usage

- Include the `llfifo.h` or `cbfifo.h` header in your project to utilize the respective FIFO implementations.
- For detailed implementation instructions and function descriptions, refer to the code documentation within the source files.

## Header File Note:
The header files for the linked list and circular buffer (`llfifo.h` and `cbfifo.h`) were modified to add dump functions. These functions were added to the header files with the express permission from Lalit Pandit, given in the class Slack. These functions are only enabled if the makefile is in DEBUG mode (which it is by default.)