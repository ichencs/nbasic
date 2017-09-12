#include "stdafx.h"
#include "UnitTest.h"

struct ThreadData2
{


};


void SimpleThreadProcedure(Thread*, void*)
{

}

TEST_CASE(TestSimpleThread2)
{
	ThreadData2 data;
	Ptr<Thread> thread = Thread::CreateAndStart(SimpleThreadProcedure, &data, false);
}