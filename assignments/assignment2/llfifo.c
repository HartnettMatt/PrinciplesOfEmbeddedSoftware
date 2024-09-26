/*******************************************************************************
 * Copyright (C) 2023 by Matt Hartnett
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Matt Hartnett and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    llfifo.c
 * @brief   The implmentation file for the linked
 *  list FIFO.
 *
 *
 * @author  Matt Hartnett
 * @date    09/11/2024
 *
 */
#include "llfifo.h"
#include <stdio.h> // ONLY INCLUDED FOR DUMP FUNCTIONS, NOT USED IN ANY PRODUCTION CODE
#include <stdlib.h>

struct llfifo_s {
    int capacity;
    int max_capacity;
    int length;
    // The head follows the oldest value in the buffer
    struct llfifo_node *head;
    // The tail is not the last allocated node, but the last node that is in use
    struct llfifo_node *tail;
};

struct llfifo_node {
    void *data;
    struct llfifo_node *link;
};

llfifo_t *llfifo_create(int capacity, int max_capacity) {
    // Handle negative capacity and max_capacity values
    if (capacity < 0 || max_capacity <= 0 || capacity > max_capacity) {
        return NULL;
    }
    // Allocate memory for the FIFO structure
    struct llfifo_s *fifo = (struct llfifo_s *)malloc(sizeof(struct llfifo_s));
    if (fifo == NULL) {
        return NULL; // Return NULL if malloc fails
    }
    // Initialize fifo values
    fifo->max_capacity = max_capacity;
    fifo->capacity = capacity;
    fifo->length = 0;
    fifo->head = NULL;
    fifo->tail = NULL;

    // Create and initialize the nodes of the linked list
    struct llfifo_node *prev_node = NULL;
    for (int i = 0; i < capacity; i++) {
        // Allocate memory for each node
        struct llfifo_node *new_node = (struct llfifo_node *)malloc(sizeof(struct llfifo_node));
        if (new_node == NULL) {
            // If malloc fails, free any already allocated memory
            llfifo_destroy(fifo);
            return NULL; // Return NULL if memory allocation fails
        }
        new_node->data = NULL; // Initialize the data field to NULL
        new_node->link = NULL; // Initialize the next pointer to NULL

        // If this is the first node, it becomes the head of the list
        if (i == 0) {
            fifo->head = new_node;
        } else {
            prev_node->link = new_node; // Link the previous node to the new node
        }

        prev_node = new_node; // Move the previous node pointer

        // If this is the last node, the link is set to head
        if (i == capacity - 1) {
            new_node->link = fifo->head;
        }
    }

    return fifo;
}

int llfifo_enqueue(llfifo_t *fifo, void *element) {
    if (element == NULL || fifo == NULL) {
        return -1; // Error out if given null pointer
    }
    // First node enqueued
    if (fifo->tail == NULL) {
        if (fifo->head == NULL) {
            // malloc a new node
            struct llfifo_node *new_node = (struct llfifo_node *)malloc(sizeof(struct llfifo_node));
            if (new_node == NULL) {
                // If malloc fails, free any already allocated memory
                llfifo_destroy(fifo);
                return -1; // Return negative 1 if memory allocation fails
            }
            new_node->link = new_node; // Create the circle in the buffer
            fifo->head = new_node;     // Add the new node to the fifo
            fifo->capacity = 1;        // This is always true since this is the first node
        }
        fifo->tail = fifo->head;
        fifo->tail->data = element; // Add data into fifo node
        fifo->length = 1;           // This is always true since this is the first node
    } else {
        // This tests if you have reached the end of your buffer:
        if (fifo->tail->link == fifo->head) {
            // If you're at max capacity, advance head, create new, free old head
            if (fifo->capacity == fifo->max_capacity) {
                struct llfifo_node *tmp = fifo->head;
                // Allocate a new node
                struct llfifo_node *new_node = (struct llfifo_node *)malloc(sizeof(struct llfifo_node));
                if (new_node == NULL) {
                    // If malloc fails, free any already allocated memory
                    llfifo_destroy(fifo);
                    return -1; // Return negative 1 if memory allocation fails
                }
                fifo->head = fifo->head->link; // Advance the head
                fifo->tail->link = new_node;   // Make the tail point to new node
                fifo->tail = new_node;         // Tie tail to new node
                new_node->link = fifo->head;   // Tie new_node link to head
                new_node->data = element;      // Write in data
                free(tmp);                     // Delete old node
                // No need to increase length since we are overwriting
            } else {
                // Allocate a new node
                struct llfifo_node *new_node = (struct llfifo_node *)malloc(sizeof(struct llfifo_node));
                if (new_node == NULL) {
                    // If malloc fails, free any already allocated memory
                    llfifo_destroy(fifo);
                    return -1; // Return negative 1 if memory allocation fails
                }
                fifo->capacity++;              // Increase capacity
                fifo->length++;                // Increase length
                fifo->tail->link = new_node;   // Set current tail to link to new tail
                fifo->tail = new_node;         // Set tail to new value
                fifo->tail->data = element;    // Write in data
                fifo->tail->link = fifo->head; // Rebuild circle
            }
        } else {
            fifo->tail = fifo->tail->link; // Advance tail
            fifo->tail->data = element;    // Write in data
            fifo->length++;                // Increase length
        }
    }
    return fifo->length;
}

void *llfifo_dequeue(llfifo_t *fifo) {
    if (fifo == NULL || fifo->length == 0) {
        return NULL;
    }

    struct llfifo_node *old_head = fifo->head; // Store current head

    if (fifo->head == fifo->tail) {
        // Case when there's only one element in the FIFO
        fifo->tail = NULL; // Reset the tail
        fifo->length = 0;  // Reset the length
    } else {
        // Multiple elements, move head to the next node
        fifo->head = fifo->head->link;
        fifo->length--; // Decrease the length
    }

    return old_head->data; // Return the dequeued element's data
}


void *llfifo_peek(llfifo_t *fifo) {
    if (fifo == NULL || fifo->length == 0) {
        return NULL;
    }
    return fifo->head->data;
}

int llfifo_length(llfifo_t *fifo) {
    if (fifo == NULL) {
        return -1;
    }
    return fifo->length;
}

int llfifo_capacity(llfifo_t *fifo) {
    if (fifo == NULL) {
        return -1;
    }
    return fifo->capacity;
}

int llfifo_max_capacity(llfifo_t *fifo) {
    if (fifo == NULL) {
        return -1;
    }
    return fifo->max_capacity;
}

void llfifo_destroy(llfifo_t *fifo) {
    if (fifo == NULL) {
        return;
    }
    if (fifo->head == NULL) {
        free(fifo);
        return;
    }
    struct llfifo_node *tmp = fifo->head;
    struct llfifo_node *next = NULL;
    do {
        next = tmp->link;
        free(tmp);
        tmp = next;
    } while (tmp != fifo->head);
    free(fifo);
    return;
}

//#ifdef DEBUG
void llfifo_state_dump(llfifo_t *fifo) {
    if (fifo == NULL) {
        printf("NULL FIFO pointer\n");
        return;
    }
    printf("FIFO STATS:\nLength: %d \nCapacity: %d \nMax Capacity: %d \n", fifo->length, fifo->capacity, fifo->max_capacity);
}

void llfifo_data_dump(llfifo_t *fifo) {
    if (fifo == NULL) {
        printf("NULL FIFO pointer\n");
        return;
    }
    struct llfifo_node *tmp = fifo->head;
    struct llfifo_node *next = NULL;
    printf("FIFO DATA:\n");
    do {
        next = tmp->link;
        printf("%d -> ", *(int *)(tmp->data));
        tmp = next;
    } while (tmp != fifo->head);
    printf("\n");
    return;
}
//#endif // DEBUG
