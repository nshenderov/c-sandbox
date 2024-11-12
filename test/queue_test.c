/*******************************************************************************
*
* FILENAME : queue_test.h
*
* DESCRIPTION : Queue unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 15.04.2023
* 
*******************************************************************************/

#include <stdio.h> /* printf */

#include "queue.h"
#include "testing.h"


static void TestQueue(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Queue", TestQueue},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestQueue(void)
{
	int t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5, t6= 6;

	queue_t *queue = QueueCreate();
	queue_t *queue2 = QueueCreate();

	QueueEnqueue(queue, &t1);
	QueueEnqueue(queue, &t2);
	QueueEnqueue(queue, &t3);

	QueueEnqueue(queue2, &t4);
	QueueEnqueue(queue2, &t5);
	QueueEnqueue(queue2, &t6);

    TH_ASSERT(1 == *(int *) QueuePeek(queue));
    TH_ASSERT(0 == QueueIsEmpty(queue));
    TH_ASSERT(3 == QueueSize(queue));

    TH_ASSERT(4 == *(int *) QueuePeek(queue2));
    TH_ASSERT(0 == QueueIsEmpty(queue2));
    TH_ASSERT(3 == QueueSize(queue2));

	QueueAppend(queue, queue2);

    TH_ASSERT(6 == QueueSize(queue));
    TH_ASSERT(1 == QueueIsEmpty(queue2));

	QueueDequeue(queue);
	QueueDequeue(queue);
	QueueDequeue(queue);
	QueueDequeue(queue);

    TH_ASSERT(5 == *(int *) QueuePeek(queue));

	QueueDestroy(queue);
	QueueDestroy(queue2);
}