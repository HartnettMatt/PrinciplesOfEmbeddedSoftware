/*******************************************************************************
 * Copyright (C) 2024 by Matt Hartnett
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Matt Hartnett and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    strcmps.h
 * @brief The c file for take home 1: string comparisons. The goal of this take
 * home assignment is to implment the standard C library function
 * strcmp in two different ways: one with a for-loop, one with a while loop.
 *
 *
 * @author Matt Hartnett
 * @date   9/6/2024
 *
 */
 

/**
 * @brief strcmp using a for loop. Doesn't have null pointer
 * protection, so don't pass a null pointer.
 *
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @return The result of the comparison.
 */
int strcmpf (const char* str1, const char* str2){
 int i;
 int result = 0;
 for(i = 0; str1[i] != '\0'; i++){
  result = str1[i] - str2[i];
  if(result != 0){
   return result;
  }
 }
 if(str2[i] != '\0'){ // Handle str2 being the same but longer than str1
  return -1;
 }
 return 0;
}


/**
 * @brief strcmp using a while loop. Doesn't have null pointer
 * protection, so don't pass a null pointer.
 *
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @return The result of the comparison.
 */
int strcmpw (const char* str1, const char* str2){
 int result = 0;
 while(*(str1) != '\0'){
  result = *str1 - *str2;
  if(result != 0){
   return result;
  }
  str1++;
  str2++;
 }
 if(*str2 != '\0'){ // Handle str2 being the same but longer than str1
  return -1;
 }
 return 0;
}

