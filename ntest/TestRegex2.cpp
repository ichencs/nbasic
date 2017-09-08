#include "stdafx.h"
#include "UnitTest.h"


using namespace regex_internal;



CharRange cr1 = {L'a',L'z'};
CharRange cr2 = {L'A',L'Z'};
CharRange cr3 = { L'1',L'9' };

TEST_CASE(TestCharRange)
{
	CharRange::List chlist;
	chlist.Add(cr1);
	chlist.Add(cr3);
	chlist.Add(cr2);

	TEST_ASSERT(chlist[0] == CharRange(L'1',L'9'));
	TEST_ASSERT(chlist[1] == CharRange(L'A', L'Z'));
	TEST_ASSERT(chlist[2] == CharRange(L'a', L'z'));

	FOREACH(CharRange,i,chlist)
	{
		WString wstr = L"ʼ��";
		wstr += i.begin;
		wstr += L"ĩ��";
		wstr += i.end;
		Console::WriteLine(wstr);
	}
	


}
