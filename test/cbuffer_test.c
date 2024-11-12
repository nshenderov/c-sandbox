/*******************************************************************************
* FILENAME : cbuffer.c
*
* DESCRIPTION : Cbuffer unit tests.
*
* AUTHOR : Nick Shenderov
*
* DATE : 01.03.2023
* 
*******************************************************************************/

#include <stdio.h>  /* printf */ 
#include <string.h>  /* strcmp */ 

#include "cbuffer.h"
#include "testing.h"


static void TestCbuffer(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Cbuffer", TestCbuffer},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);
    	
    return (0);
}

static void TestCbuffer(void)
{
    cbuffer_t *buf = NULL;
    char test_data[13] = "Hello world!";
	char test_dest[50] = {'\0'};
	size_t buf_capacity = 20;
	
	buf = CBufferCreate(buf_capacity);

    TH_ASSERT(1 == CBufferIsEmpty(buf));
    TH_ASSERT(20 == CBufferBufSize(buf));
    TH_ASSERT(20 == CBufferFreeSpace(buf));


    TH_ASSERT(13 == CBufferWrite(buf, test_data, 13));
    TH_ASSERT(0 == CBufferIsEmpty(buf));
    TH_ASSERT(7 == CBufferFreeSpace(buf));

    TH_ASSERT(7 == CBufferWrite(buf, test_data, 10));
    TH_ASSERT(0 == CBufferIsEmpty(buf));
    TH_ASSERT(0 == CBufferFreeSpace(buf));

    TH_ASSERT(0 == CBufferWrite(buf, test_data, 13));
    TH_ASSERT(0 == CBufferIsEmpty(buf));
    TH_ASSERT(0 == CBufferFreeSpace(buf));


    TH_ASSERT(13 == CBufferRead(buf, test_dest, 13));
    TH_ASSERT(13 == CBufferFreeSpace(buf));
    TH_ASSERT(0 == CBufferIsEmpty(buf));

    TH_ASSERT(7 == CBufferRead(buf, test_dest, 13));
    TH_ASSERT(20 == CBufferFreeSpace(buf));
    TH_ASSERT(1 == CBufferIsEmpty(buf));

    TH_ASSERT(0 == CBufferRead(buf, test_dest, 13));
    TH_ASSERT(20 == CBufferFreeSpace(buf));
    TH_ASSERT(1 == CBufferIsEmpty(buf));

    TH_ASSERT(0 == strcmp(test_data, test_dest));

    CBufferDestroy(buf);
}