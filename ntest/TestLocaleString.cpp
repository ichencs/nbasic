#include "stdafx.h"
#include "UnitTest.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;

extern WString GetTestResourcePath();
extern WString GetTestOutputPath();

TEST_CASE(TestLocaleFormat)
{
	DateTime dt = DateTime::FromDateTime(2000, 1, 2, 13, 2, 3);
	wchar_t* df = L"yyyy,MM,MMM,MMMM,dd,ddd,dddd";
	wchar_t* ds = L"2000,01,Jan,January,02,Sun,Sunday";
	wchar_t* tf = L"hh,HH,mm,ss,tt";
	wchar_t* ts = L"01,13,02,03,PM";
	TEST_ASSERT(INVLOC.FormatDate(df, dt) == ds);
	TEST_ASSERT(INVLOC.FormatTime(tf, dt) == ts);
}

TEST_CASE(TestLocaleCompare)
{
	TEST_ASSERT(INVLOC.Compare(L"abc", L"abc", Locale::None) == 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"ABC", Locale::None) != 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"abc", Locale::IgnoreCase) == 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"ABC", Locale::IgnoreCase) == 0);
	
	TEST_ASSERT(INVLOC.CompareOrdinal(L"abc", L"abc") == 0);
	TEST_ASSERT(INVLOC.CompareOrdinal(L"abc", L"ABC") != 0);
	TEST_ASSERT(INVLOC.CompareOrdinalIgnoreCase(L"abc", L"abc") == 0);
	TEST_ASSERT(INVLOC.CompareOrdinalIgnoreCase(L"abc", L"ABC") == 0);
}

TEST_CASE(TestLocaleFind)
{
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"vczh", Locale::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"vczh", Locale::None).key == 3);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"Vczh", Locale::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"Vczh", Locale::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"vczh", Locale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"vczh", Locale::IgnoreCase).key == 3);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"Vczh", Locale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"Vczh", Locale::IgnoreCase).key == 3);
	
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"vczh", Locale::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"vczh", Locale::None).key == 10);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"Vczh", Locale::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"Vczh", Locale::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"vczh", Locale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"vczh", Locale::IgnoreCase).key == 10);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"Vczh", Locale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"Vczh", Locale::IgnoreCase).key == 10);
	
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"a", Locale::None) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"A", Locale::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"c", Locale::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"C", Locale::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"a", Locale::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"A", Locale::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"c", Locale::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"C", Locale::IgnoreCase) == false);
	
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"a", Locale::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"A", Locale::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"c", Locale::None) == true);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"C", Locale::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"a", Locale::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"A", Locale::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"c", Locale::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"C", Locale::IgnoreCase) == true);
}

TEST_CASE(TestLocaleCase)
{
	TEST_ASSERT(INVLOC.ToUpper(L"abcABC") == L"ABCABC");
	TEST_ASSERT(INVLOC.ToLower(L"abcABC") == L"abcabc");
	TEST_ASSERT(INVLOC.ToLinguisticUpper(L"abcABC") == L"ABCABC");
	TEST_ASSERT(INVLOC.ToLinguisticLower(L"abcABC") == L"abcabc");
}
