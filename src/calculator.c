/*******************************************************************************
*
* FILENAME : calculator.h
*
* DESCRIPTION : Calculator implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 01.06.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "stack.h"
#include "calculator.h"

#define ASCII_SIZE (256)
#define NULL_TERMINATOR ('\0')
#define CALC_STRUCT_SIZE (sizeof(struct calc))

#define PEEK_VAL_OPERANDS_STACK(CALC) (*(double *) StackPeek(CALC->operands))
#define PEEK_VAL_OPERATORS_STACK(CALC) (*(char *) StackPeek(CALC->operators))
#define PEEK_PTR_OPERANDS_STACK(CALC) ((double *) StackPeek(CALC->operands))

#define GET_HANDLER_ENTITY(CALC, STATE, SYMBOL) \
(CALC->states_lut[STATE][SYMBOL])
#define HANDLER_GET_HANDLER(HANDLER) \
(HANDLER.handler)
#define HANDLER_GET_NEXT_STATE(HANDLER) \
(HANDLER.next_state)

#define GET_OPERATOR_ENTITY(CALC, OPERATOR) \
(CALC->operations_lut[OPERATOR])

#define UPDATE_STATES_LUT(STATE, SYMBOL, HANDLER, NEXT_STATE)                   \
do                                                                              \
{                                                                               \
    calc->states_lut[STATE][SYMBOL].handler = HANDLER;                          \
    calc->states_lut[STATE][SYMBOL].next_state = NEXT_STATE;                    \
}                                                                               \
while (0)

#define UPDATE_OPERATIONS_LUT(OPERATOR, OPERATION, PRIORITY)                    \
do                                                                              \
{                                                                               \
    calc->operations_lut[OPERATOR].operation = OPERATION;                       \
    calc->operations_lut[OPERATOR].priority = PRIORITY;                         \
}                                                                               \
while (0)

typedef struct handler_entry handler_entry_t;
typedef struct operator_entry operator_entry_t;
typedef calc_status_t (*handler_t)(calc_t *calc, char **data);
typedef calc_status_t (*operation_t)(double *num1, double num2);

typedef enum states
{
    WAIT_OPERATOR,
    WAIT_OPERAND,
    ERROR,
    NUM_OF_STATES
} state_t;

typedef enum priority
{
    DUMMY_OP = -1,
    OPEN_PAR = 0,
    PLUS = 1,
    MINUS = 1,
    MULT = 2,
    DIV = 2,
    CLOSE_PAR = 3
} priority_t;

struct handler_entry
{
    handler_t handler;
    state_t next_state;
};

struct operator_entry
{
    operation_t operation;
    priority_t priority;
};

struct calc
{
    stack_t *operands;
    stack_t *operators;
    handler_entry_t states_lut[NUM_OF_STATES][ASCII_SIZE];
    operator_entry_t operations_lut[ASCII_SIZE];
};


static calc_status_t Compute(calc_t *calc);
static void CleanStacks(calc_t *calc);

static void FillUpWithDummyStatesLut(calc_t *calc);
static void FIllUpHandlers(calc_t *calc);
static void FillUpOperations(calc_t *calc);

static calc_status_t DummyHandler(calc_t *calc, char **data);
static calc_status_t NullTerminatorHandler(calc_t *calc, char **data);
static calc_status_t NumberHandler(calc_t *calc, char **data);
static calc_status_t OperatorHandler(calc_t *calc, char **data);
static calc_status_t OpenParHandler(calc_t *calc, char **data);
static calc_status_t CloseParHandler(calc_t *calc, char **data);

static calc_status_t NullTermFailOperation(double *num1, double num2);
static calc_status_t AdditionOperation(double *num1, double num2);
static calc_status_t SubtractionOperation(double *num1, double num2);
static calc_status_t MultiplicationOperation(double *num1, double num2);
static calc_status_t DivisionOperation(double *num1, double num2);


/* general */
/*----------------------------------------------------------------------------*/

calc_t *CalcCreate(size_t max_len)
{
    stack_t *operands = NULL;
    stack_t *operators = NULL;

    calc_t *calculator = (calc_t *) malloc(CALC_STRUCT_SIZE);
    if (NULL == calculator)
    {
        return (NULL);
    }

    operands = StackCreate(max_len, sizeof(double));
    if (NULL == operands)
    {
        free(calculator);
        calculator = NULL;

        return (NULL);
    }

    operators = StackCreate(max_len, sizeof(char));
    if (NULL == operators)
    {
        free(calculator);
        calculator = NULL;

        free(operands);
        operands = NULL;

        return (NULL);
    }

    calculator->operands = operands;
    calculator->operators = operators;

    FillUpWithDummyStatesLut(calculator);

    FIllUpHandlers(calculator);
    FillUpOperations(calculator);

    return (calculator);
}

double Calculate(const char *str, calc_status_t *calc_status, calc_t *calc)
{
    handler_entry_t handler_entry = {0};
    calc_status_t current_status = SUCCESS;
    state_t current_state = WAIT_OPERAND;
    double result = 0.0;
    char dummy_operator = NULL_TERMINATOR;

    assert(NULL != str);
    assert(NULL != calc);
    assert(NULL != calc_status);

    StackPush(calc->operators, &dummy_operator);

    while(ERROR != current_state && SUCCESS == current_status)
    {
        handler_entry = GET_HANDLER_ENTITY(calc, current_state, (int) *str);

        current_status = HANDLER_GET_HANDLER(handler_entry)(calc,(char **)&str);

        current_state = HANDLER_GET_NEXT_STATE(handler_entry);
    }

    *calc_status = current_status;

    result = PEEK_VAL_OPERANDS_STACK(calc);

    CleanStacks(calc);

    return (result);
}

void CalcDestroy(calc_t *calc)
{
    assert(NULL != calc);
    assert(NULL != calc->operands);
    assert(NULL != calc->operators);

    StackDestroy(calc->operands);
    calc->operands = NULL;

    StackDestroy(calc->operators);
    calc->operators = NULL;

    free(calc);
    calc = NULL;
}

/* helpers*/
/*----------------------------------------------------------------------------*/

static calc_status_t Compute(calc_t *calc)
{
    double *num1 = NULL;
    double num2 = 0.0;
    int last_operator_char = NULL_TERMINATOR;
    operator_entry_t last_operator = {0};
    calc_status_t status = SUCCESS;

    assert(NULL != calc);

    last_operator_char = PEEK_VAL_OPERATORS_STACK(calc);
    last_operator = calc->operations_lut[last_operator_char];
    StackPop(calc->operators);

    num2 = PEEK_VAL_OPERANDS_STACK(calc);
    StackPop(calc->operands);

    num1 = PEEK_PTR_OPERANDS_STACK(calc);

    status = last_operator.operation(num1, num2);

    return (status);
}

static void CleanStacks(calc_t *calc)
{
    assert(NULL != calc);
    assert(NULL != calc->operands);
    assert(NULL != calc->operators);

    while(!StackIsEmpty(calc->operators))
    {
        StackPop(calc->operators);
    }

    while(!StackIsEmpty(calc->operands))
    {
        StackPop(calc->operands);
    }
}

/* filling */
/*----------------------------------------------------------------------------*/

static void FillUpWithDummyStatesLut(calc_t *calc)
{
    int i = 0;
    int q = 0;

    for (; i < NUM_OF_STATES; ++i)
    {
        for (q = 0; q < ASCII_SIZE; ++q)
        {
            UPDATE_STATES_LUT(i, q, DummyHandler, ERROR);
        }
    }
}

static void FIllUpHandlers(calc_t *calc)
{
    int i = '0';

    for (; i <= '9'; ++i)
    {
        UPDATE_STATES_LUT(WAIT_OPERAND, i, NumberHandler, WAIT_OPERATOR);
    }

    UPDATE_STATES_LUT(WAIT_OPERATOR, '+', OperatorHandler, WAIT_OPERAND);
    UPDATE_STATES_LUT(WAIT_OPERATOR, '-', OperatorHandler, WAIT_OPERAND);
    UPDATE_STATES_LUT(WAIT_OPERATOR, '*', OperatorHandler, WAIT_OPERAND);
    UPDATE_STATES_LUT(WAIT_OPERATOR, '/', OperatorHandler, WAIT_OPERAND);

    UPDATE_STATES_LUT(WAIT_OPERAND, '(', OpenParHandler, WAIT_OPERAND);
    UPDATE_STATES_LUT(WAIT_OPERATOR, ')', CloseParHandler, WAIT_OPERATOR);

    UPDATE_STATES_LUT(WAIT_OPERATOR,NULL_TERMINATOR,NullTerminatorHandler,ERROR);
}

static void FillUpOperations(calc_t *calc)
{
    int i = 0;

    for (; i < ASCII_SIZE; ++i)
    {
        UPDATE_OPERATIONS_LUT(i, NullTermFailOperation, DUMMY_OP);
    }

    UPDATE_OPERATIONS_LUT('+', AdditionOperation, PLUS);
    UPDATE_OPERATIONS_LUT('-', SubtractionOperation, MINUS);
    UPDATE_OPERATIONS_LUT('*', MultiplicationOperation, MULT);
    UPDATE_OPERATIONS_LUT('/', DivisionOperation, DIV);
    UPDATE_OPERATIONS_LUT(NULL_TERMINATOR, NullTermFailOperation, DUMMY_OP);
}

/* handlers */
/*----------------------------------------------------------------------------*/
static calc_status_t DummyHandler(calc_t *calc, char **data)
{
    return (SYNTAX_ERROR);
    (void) calc;
    (void) data;
}

static calc_status_t NullTerminatorHandler(calc_t *calc, char **data)
{
    calc_status_t status = SYNTAX_ERROR;

    char operator = NULL_TERMINATOR;

    assert(NULL != calc);
    assert(NULL != data);

    operator = PEEK_VAL_OPERATORS_STACK(calc);

    while (NULL_TERMINATOR != operator)
    {
        if ('(' == operator || ')' == operator)
        {
            status = SYNTAX_ERROR;
            break;
        }

        status = Compute(calc);
        operator = PEEK_VAL_OPERATORS_STACK(calc);
    }

    return (status);
    (void) calc;
    (void) data;
}

static calc_status_t NumberHandler(calc_t *calc, char **data)
{
    double number = 0.0;

    assert(NULL != calc);
    assert(NULL != data);

    number = strtod(*data, data);

    StackPush(calc->operands, &number);

    return (SUCCESS);
}

static calc_status_t OperatorHandler(calc_t *calc, char **data)
{
    operator_entry_t operator = {0};
    operator_entry_t last_operator = {0};
    calc_status_t status = SUCCESS;
    int last_operator_char = NULL_TERMINATOR;

    assert(NULL != calc);
    assert(NULL != data);

    operator = GET_OPERATOR_ENTITY(calc, (int) **data);

    last_operator_char = PEEK_VAL_OPERATORS_STACK(calc);

    last_operator = GET_OPERATOR_ENTITY(calc, last_operator_char);

    while (last_operator.priority >= operator.priority)
    {
        status = Compute(calc);

        if (SUCCESS != status)
        {
            break;
        }

        last_operator_char = PEEK_VAL_OPERATORS_STACK(calc);
        last_operator = GET_OPERATOR_ENTITY(calc, last_operator_char);
    }

    StackPush(calc->operators, *data);
    *data += 1;

    return (status);
}

static calc_status_t OpenParHandler(calc_t *calc, char **data)
{
    assert(NULL != calc);
    assert(NULL != data);

    StackPush(calc->operators, *data);

    *data += 1;

    return (SUCCESS);
}

static calc_status_t CloseParHandler(calc_t *calc, char **data)
{
    calc_status_t status = SUCCESS;

    char operator = NULL_TERMINATOR;

    assert(NULL != calc);
    assert(NULL != data);

    operator = PEEK_VAL_OPERATORS_STACK(calc);

    while (NULL_TERMINATOR != operator && '(' != operator)
    {
        status = Compute(calc);
        operator = PEEK_VAL_OPERATORS_STACK(calc);
    }

    if (NULL_TERMINATOR == operator)
    {
        status = SYNTAX_ERROR;
    }

    StackPop(calc->operators);

    *data += 1;

    return (status);
}

/* operations */
/*----------------------------------------------------------------------------*/

static calc_status_t NullTermFailOperation(double *num1, double num2)
{
    return (SYNTAX_ERROR);
    (void) num1;
    (void) num2;
}

static calc_status_t AdditionOperation(double *num1, double num2)
{
    assert(NULL != num1);

    *num1 = *num1 + num2;

    return (SUCCESS);
}
static calc_status_t SubtractionOperation(double *num1, double num2)
{
    assert(NULL != num1);

    *num1 = *num1 - num2;

    return (SUCCESS);
}
static calc_status_t MultiplicationOperation(double *num1, double num2)
{
    assert(NULL != num1);

    *num1 = *num1 * num2;

    return (SUCCESS);
}
static calc_status_t DivisionOperation(double *num1, double num2)
{
    assert(NULL != num1);

    if (0 == num2)
    {
        return (MATH_ERROR);
    }

    *num1 = *num1 / num2;

    return (SUCCESS);
}