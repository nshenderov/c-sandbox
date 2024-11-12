/*******************************************************************************
*
* FILENAME : calculator.h
*
* DESCRIPTION : Basic calculator, uses state machine model of calculation and
* Dijkstra's 2-stack algorithm for evaluating mathematical expressions.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 01.06.2023
* 
*******************************************************************************/

#ifndef __NSRD_CALCULATOR_H__
#define __NSRD_CALCULATOR_H__

#include <stddef.h> /* size_t */

typedef enum status
{
    SUCCESS,
    SYNTAX_ERROR,
    MATH_ERROR
} calc_status_t;

typedef struct calc calc_t;

/*
DESCRIPTION
    This function creates a calculator and allocates memory for the math
    expression of the predetermined length. The maximum length is specified
    by the 'max_len' parameter. The memory allocation may fail, in which NULL
    is returned. It is the responsibility of the user to destroy calculators.
RETURN
    Pointer to the created calculator on success.
    NULL if memory allocation fails.
INPUT
    max_len: the maximum length of the string containing the math expression.
*/
calc_t *CalcCreate(size_t max_len);

/*
DESCRIPTION
    This function calculates the result of the math expression provided as a
    string. The calculated result is returned, and the status of the
    calculation is written into the 'calc_status' output parameter.
    The user should check the status to verify the result of the calculation.
RETURN
    The result of the calculation.
INPUT
    str: pointer to the string containing the math expression to be calculated.
    calc_status: output parameter to store the status of the calculation.
    calc: pointer to the calculator.
*/
double Calculate(const char *str, calc_status_t *calc_status, calc_t *calc);

/*
DESCRIPTION
    This function destroys the calculator object and frees the allocated
    memory. It is the responsibility of the user to handle any dangling
    pointers after the calculator is destroyed.
RETURN
    This function does not return anything.
INPUT
    calc: pointer to the calculator to be destroyed.
*/
void CalcDestroy(calc_t *calc);

#endif /* __NSRD_CALCULATOR_H__ */