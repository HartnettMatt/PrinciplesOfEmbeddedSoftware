/*
 * log.h
 *
 *  Created on: Oct 1, 2024
 *      Author: Matt Hartnett
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifdef DEBUG
#define LOG printf
#else
#define LOG(...)
#endif

#endif /* LOG_H_ */
