#include <string.h>
#include "stdafx.h"
#include "UnitTest.h"


void CheckString(const WString& string, const wchar_t* buffer)
{
	size_t len = wcslen(buffer);
	TEST_ASSERT(string.Length() == len);

	for (size_t i = 0; i < len; i++)
	{
		TEST_ASSERT(string[i] == buffer[i]);
	}

	TEST_ASSERT(wcscmp(string.Buffer(), buffer) == 0);
}

void CheckString(const AString& string, const char* buffer)
{
	size_t len = strlen(buffer);
	TEST_ASSERT(string.Length() == len);

	for (size_t i = 0; i < len; i++)
	{
		TEST_ASSERT(string[i] == buffer[i]);
	}

	TEST_ASSERT(strcmp(string.Buffer(), buffer) == 0);
}

TEST_CASE(StringLeftRightSub)
{
	WString string = L"vczh is genius";
	CheckString(string.Left(4), L"vczh");
	CheckString(string.Sub(5, 2), L"is");
	CheckString(string.Mid(5, 2), L"is");
	CheckString(string.Right(6), L"genius");
	CheckString(string.Sub(4, 4).Left(2), L" i");
	CheckString(string.Sub(4, 4).Sub(1, 2), L"is");
	CheckString(string.Sub(4, 4).Right(2), L"s ");
	CheckString(string.Sub(0, 0), L"");
	CheckString(string.Sub(12, 0), L"");
}

TEST_CASE(TestConcat)
{
	WString string = L"You are very clever";
	CheckString(string.Left(7) + string.Right(7), L"You are clever");
}

TEST_CASE(TestConstFriend)
{
	WString string = L"You are very clever";
	CheckString(L"You are" + string.Right(7), L"You are clever");
}

TEST_CASE(TestStringIsEmpty)
{
	WString string;
	string = L"";
	TEST_ASSERT(string.IsEmpty());

}

TEST_CASE(TestStringTrim)
{
	WString string = L"nice to meet you!!!";
	TEST_ASSERT(string.Trim(L'!') == L"nice to meet you");
	TEST_ASSERT(string.TrimRight(L'!') == L"nice to meet you");
	TEST_ASSERT(string.TrimLeft(L'n') == L"ice to meet you!!!");

	TEST_ASSERT(string.TrimRight(L'!').TrimRight(L'u').TrimRight(L'o').TrimRight(L'y') == L"nice to meet ");
	TEST_ASSERT(string.TrimLeft(L'n').TrimLeft(L'i').TrimLeft(L'c') == L"e to meet you!!!");

	string = L"nice";
	TEST_ASSERT(string.TrimRight(L'e').TrimRight(L'c').TrimRight(L'i').TrimRight(L'n') == L"");
	TEST_ASSERT(string.TrimLeft(L'n').TrimLeft(L'i').TrimLeft(L'c').TrimLeft(L'e') == L"");
	string = L"aaaAaaaa";
	TEST_ASSERT(string.TrimRight('a') == L"aaaA");
	TEST_ASSERT(string.TrimLeft('a') == L"Aaaaa");
	TEST_ASSERT(string.Trim('a') == L"A");
	string = L"abcde";
	TEST_ASSERT(string.TrimLeft(L'a').TrimLeft(L'b').TrimLeft(L'c').TrimLeft(L'd').TrimLeft(L'e') == L"");

//	WString wstrTemp = string.TrimLeft(L'a').TrimRight(L'a');
 	TEST_ASSERT(string.Trim('a') == string.TrimLeft(L'a').TrimRight(L'a'));
	TEST_ASSERT(string.Mid(3, 1) == L"d");
	TEST_ASSERT(string.TrimLeft(L"ab") == L"cde");
	TEST_ASSERT(string.TrimLeft(L"ab").TrimLeft(L"cd") == L"e");
	TEST_ASSERT(string.TrimLeft(L"ab").TrimLeft(L"cd").TrimLeft(L"e") == L"");

	TEST_ASSERT(string.TrimLeft(L"ba") == L"cde");
	TEST_ASSERT(string.TrimLeft(L"ba").TrimLeft(L"cd") == L"e");
	TEST_ASSERT(string.TrimLeft(L"ba").TrimLeft(L"cde") == L"");
	
	TEST_ASSERT(string.TrimRight(L"de") == L"abc");
	TEST_ASSERT(string.TrimRight(L"ed").TrimLeft(L"bca") == L"");
	TEST_ASSERT(string.TrimRight(L"cde").TrimLeft(L"ba") == L"");
	TEST_ASSERT(string.TrimLeft(L"ba").TrimRight(L"cde") == L"");
	TEST_ASSERT(string.TrimLeft(L"ba").TrimRight(L"de") == L"c");
	TEST_ASSERT(string.TrimRight(L"ed").TrimRight(L"bc") == L"a");

}

TEST_CASE(TestDoouble)
{
	double d = 3.141592653578951234567521;
	WString wstr = ftow(d,5);
	TEST_ASSERT(wstr == L"3.1416");

	
}

<<<<<<< HEAD
TEST_CASE(TestSplit)
{
	NList<WString> strList;
	WString wstr = L".1.2.3.4.5.6.";
	SplitString(wstr, strList, L'.');
	
	for (nint i = 0; i < strList.Count(); i++)
	{
		Console::WriteLine(strList.Get(i));
	}
}
=======
>>>>>>> parent of 3eea6d6... string-split
