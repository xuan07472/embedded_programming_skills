/*******************************************************************************
* File name    : weekly_contest.c
* Purpose      : Answer the question of https://leetcode.com/problems/relative-ranks/description/
* Author       : ZhangJing
* Time         : 2018-02-11

*******************************************************************************/

/************************** Include header file *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
////#include <iostream> // Matching the C++

/**************************** Macro definition ********************************/
////#pragma warning(disable:4996) // Matching the VS2015
#define EMBEDDED_BOARD // To use in embedded software development.
#define MAX_NUMBER_OF_ATHLETES 10000 // N is a positive integer and won't exceed 10,000, All the scores of athletes are guaranteed to be unique.
#define UNIT_SIZE_16	16 // The max bytes of ranks.
#define UNIT_SIZE_8		8 // The max bytes of ranks after third.

/************************ Global variable definition **************************/
static const char* gpMedalsString[3] = { "Gold Medal", "Silver Medal", "Bronze Medal" };
static char** gapReturnedArray; // The ptr.
static char* gpData = NULL; // The Data.

/******************************** Function ************************************/
/**
* Return an array of size *returnSize.
* Note: The returned array must be malloced, assume caller calls free().
*/
#ifdef EMBEDDED_BOARD /* If to use in embedded software development. */
char** findRelativeRanks(int* nums, int numsSize, int* returnSize) {
	int realSize = 0;
	int i = 0, j = 0, tmp = 0;
	char *pData = NULL;
    int *originalArray;

	/* 1. Check parameter. */
	if ((NULL == nums) || (numsSize <= 0) || (numsSize > MAX_NUMBER_OF_ATHLETES)
		|| (NULL == returnSize)) {
		returnSize = 0;
		return NULL;
	}

	/* 2. Malloc memory. */
	// Reduced the malloc()'s used and byte alignment, I Simply think that the first, second and third is 16 bytes, others is 8 bytes.
	if (numsSize <= 3) {
		realSize = numsSize * UNIT_SIZE_16;
	}
	else {
		realSize = (numsSize - 3) * UNIT_SIZE_8 + 3 * UNIT_SIZE_16;
	}

	if (NULL == (gpData = (char *)malloc(realSize))) { // Melloc data memory.
		returnSize = 0;
		return NULL;
	}
	memset(gpData, 0, realSize);
    
	if (NULL == (gapReturnedArray = (char **)malloc(numsSize * sizeof(char *)))) { // Melloc array ptr memory 
		free(gpData); // Free memory is importent!
		returnSize = 0;
		return NULL;
	}
	memset((char *)gapReturnedArray, 0, numsSize * sizeof(char *));
	
	if ( NULL == (originalArray = (int *)malloc(numsSize * sizeof(int)) ) ) {
		free(gapReturnedArray); // Free memory is importent!
		returnSize = 0;
		return NULL;
	}
	for (i = 0; i < numsSize; i++) { // Original order.
	    *(originalArray + i) = i;
	}

	/* 3. Bubble sort. I don't use quick sort achieved by recursion, avoid the stack overflow. */
	for (i = 0; i < numsSize; i++) {
		for (j = 0; j < numsSize - i - 1; j++) {
			if (nums[j] < nums[j + 1]) {
				tmp = nums[j];
				nums[j] = nums[j + 1];
				nums[j + 1] = tmp;
				
				tmp = originalArray[j];
				originalArray[j] = originalArray[j + 1];
				originalArray[j + 1] = tmp;
			}
		}
	}

	/* 4. Changeing to string */
	pData = gpData;
	for (i = 0; (i < numsSize) && (i < 3); i++) { // The 1st, 2nd and 3rd.
		//strcmp(pData, gpMedalsString[i]);
		memcpy(pData, gpMedalsString[i], strlen(gpMedalsString[i]) + 1);
		gapReturnedArray[originalArray[i]] = pData;
		pData += UNIT_SIZE_16;
	}
	for (i = 3; i < numsSize; i++) { // The order after 3rd.
		////itoa(nums[i], pData, 10);
		////_itoa(nums[i], pData, 10); // Matching the VS2015
		sprintf(pData, "%d", i + 1);
		gapReturnedArray[originalArray[i]] = pData;
		pData += UNIT_SIZE_8;
	}
	free(originalArray);

	/* 5. Output. */
	*returnSize = numsSize;
	return gapReturnedArray; // Please remember to free the gapReturnedArray and gpArray!!!
}
#else /* If to use in PC. */
/**
 * We can use quick sort of recurrence and malloc() per order number
 * if application running in PC.
 */
char** findRelativeRanks(int* nums, int numsSize, int* returnSize) {
    /* Omission. */
    return NULL;
}
#endif /* EMBEDDED_BOARD */

/**
 * Main function.
 */
int main(void)
{
	int size, i;
	int num[5] = { 5, 1, 3, 4, 2 };
	char **p;
	

	if ( p = findRelativeRanks(num, 5, &size) )
	{
		printf("Test result:\n");
		for (i = 0; i < size; i++)
		{
		    printf("%s    ", p[i]);
		}
		printf("\n");
	}
	else
	{
	    printf("Test fail!\n");
		free(p);
		free(gpData);
	}

	////while (1); // Matching the VS2015
	return 0;
}

/******************************** File End ************************************/
