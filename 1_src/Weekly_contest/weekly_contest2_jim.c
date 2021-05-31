/*******************************************************************************
* File name    : weekly_contest2_jim.c
* Purpose      : Answer the question of https://leetcode.com/problems/longest-substring-without-repeating-characters/description/
* Author       : ZhangJing
* Time         : 2018-03-14

*******************************************************************************/

/************************** Include header file *******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**************************** Macro definition ********************************/
#define TRUE 1
#define FALSE 0

/******************************** Function ************************************/
/**
 * Arithmetic: Brute force.
 */
int lengthOfLongestSubstring(char* s) {
    int i, j, k, len;
    int max_length = 1; // The max length of longest substring.
    int tmp_length; // The temp length of longest substring.
    int break_flag; // The flag that exit double "for" loop.
    
	/* 1. Check parameter. */
    if (s == NULL){
        return -1;
    }
    
    len = strlen(s);
    if (len <= 0) {
        return 0;
    }
    
    /* 2. Brute force */
    for (i = 0; i < len; i++) { // Start of once compare.
        for (j = i + 1; j < len; j++) { // End of once compare.
            tmp_length = 1;
            break_flag = FALSE;
            
            for (k = i; k < j; k++) { // Once comparison.
                if (s[k] != s[j]) {
                    tmp_length++;          
                } else {
                    break_flag = TRUE;
                    break;
                }
            }

            if (break_flag) {
                break;
            } else { // Completion of a comparison.
                if (tmp_length > max_length) {
                    max_length = tmp_length;
                }
            }
        }
    }
    
    return max_length;
}

/******************************** File End ************************************/

