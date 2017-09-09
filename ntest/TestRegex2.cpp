#include "stdafx.h"
#include "UnitTest.h"

using namespace regex;
using namespace regex_internal;



CharRange cr1 = CharRange(L'a', L'z');
CharRange cr2 = CharRange(L'A', L'Z');
CharRange cr3 = CharRange( L'1', L'9');

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

}

TEST_CASE(TestRegexLexer1)
{
	NList<WString> codes;
	codes.Add(L"/d+");
	codes.Add(L"/s+");
	codes.Add(L"[a-zA-Z_]/w*");
	// 	RegexLexer lexer(codes);
	//
	// 	{
	// 		NList<RegexToken> tokens;
	// 		CopyFrom(tokens, lexer.Parse(L"vczh is$$a&&genius  1234"));
	// 		// 		TestRegexLexer1Validation(tokens);
	// 	}
	// 	{
	// 		NList<RegexToken> tokens;
	// 		lexer.Parse(L"vczh is$$a&&genius  1234").ReadToEnd(tokens);
	// 		// 		TestRegexLexer1Validation(tokens);
	// 	}
}

