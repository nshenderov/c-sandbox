/*******************************************************************************
*
* FILENAME : calculator_test.c
*
* DESCRIPTION : Calculator unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 01.06.2023
* 
*******************************************************************************/

#include <stdlib.h> /* malloc, free*/
#include <math.h> /* fabs */

#include "calculator.h"
#include "testing.h"


#define EPSILON (0.001)


static void TestCreate(void);
static void TestGeneralBehavior(void);
static void TestReturnedStatus(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Create", TestCreate},
		{"General behavior", TestGeneralBehavior},
		{"Returned status", TestReturnedStatus},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestCreate(void)
{
	calc_t *cal = CalcCreate(80);

	TH_ASSERT(NULL != cal);

	CalcDestroy(cal);
}

static void TestGeneralBehavior(void)
{
	calc_t *cal = CalcCreate(80);

	calc_status_t status = SUCCESS;

	TH_ASSERT(EPSILON > fabs(13.255 - Calculate("5*2+3.255", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(6.000 - Calculate("10+5-9", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(14.000 - Calculate("10-5+9", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(1.200 - Calculate("2/5*3", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(3.333 - Calculate("2*5/3", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(-4.000 - Calculate("10-(5+9)", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(6.000 - Calculate("10+(5-9)", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	TH_ASSERT(EPSILON > fabs(12.000 - Calculate("2*3*2-2/2+2-2/2", &status, cal)));
	TH_ASSERT(SUCCESS == status);

	CalcDestroy(cal);
}

static void TestReturnedStatus(void)
{
    calc_t *cal = CalcCreate(80);
    calc_status_t status = SUCCESS;

    Calculate("2/0", &status, cal);
	TH_ASSERT(MATH_ERROR == status);

	Calculate("(10+5-9", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);
	
    Calculate("10(+5-9", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);

    Calculate("10+(5-9", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);
	
    Calculate("10+5-9(", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);
	
    Calculate(")10+5-9", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);

    Calculate("10)+5-9", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);
	
    Calculate("10+)5-9", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);
	
    Calculate("10+5-9)", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);

    Calculate("2+9+()", &status, cal);
    TH_ASSERT(SYNTAX_ERROR == status);

    CalcDestroy(cal);
}