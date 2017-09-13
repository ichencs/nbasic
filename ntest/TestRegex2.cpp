#include "stdafx.h"
#include "UnitTest.h"
using namespace vl::console;
using namespace regex;
using namespace regex_internal;
using namespace vl::collections;


CharRange cr1 = CharRange(L'a', L'z');
CharRange cr2 = CharRange(L'A', L'Z');
CharRange cr3 = CharRange(L'1', L'9');

TEST_CASE(TestCharRange)
{
	CharRange::List chlist;
	chlist.Add(cr1);
	chlist.Add(cr3);
	chlist.Add(cr2);
	
	TEST_ASSERT(chlist[0] == CharRange(L'1', L'9'));
	TEST_ASSERT(chlist[1] == CharRange(L'A', L'Z'));
	TEST_ASSERT(chlist[2] == CharRange(L'a', L'z'));
	
	FOREACH(CharRange, i, chlist)
	{
		WString wstr = L"Ê¼£º";
		wstr += i.begin;
		wstr += L"Ä©£º";
		wstr += i.end;
		Console::WriteLine(wstr);
	}
	
}

TEST_CASE(RegexAutomaton)
{
	// 	TEST_ASSERT(false);
}



