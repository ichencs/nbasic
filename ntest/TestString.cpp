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