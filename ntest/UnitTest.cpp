#include "stdafx.h"
#include "UnitTest.h"
using namespace vl::console;


void UnitTest::PrintMessage(const WString& string)
{
	Console::SetColor(false, true, false, true);
	Console::WriteLine(string);
	Console::SetColor(true, true, true, false);
}

void UnitTest::PrintInfo(const WString& string)
{
	Console::SetColor(true, true, true, true);
	Console::WriteLine(string);
	Console::SetColor(true, true, true, false);
}

void UnitTest::PrintError(const WString& string)
{
	Console::SetColor(true, false, false, true);
	Console::WriteLine(string);
	Console::SetColor(true, true, true, false);
}

struct UnitTestLink
{
	UnitTestLink(): testProc(NULL), next(NULL)
	{};
	UnitTest::TestProc			testProc /*= nullptr*/;
	UnitTestLink*				next/* = nullptr*/;
};
UnitTestLink*					testHead = NULL;
UnitTestLink**					testTail = &testHead;

/// <summary>
/// 编译的过程中通过宏来自动生成链表（初始化时在内存生成链表）！
/// </summary>
/// <param name="testProc"></param>
/// <returns></returns>
void UnitTest::PushTest(UnitTest::TestProc testProc)
{
	UnitTestLink* link = new UnitTestLink;
	link->testProc = testProc;
	*testTail = link;
	testTail = &link->next;
}

void UnitTest::RunAndDisposeTests()
{
	UnitTestLink* current = testHead;
	testHead = NULL;
	testTail = NULL;
	
	while (current)
	{
		current->testProc();
		UnitTestLink* temp = current;
		current = current->next;
		delete temp;
	}
}
