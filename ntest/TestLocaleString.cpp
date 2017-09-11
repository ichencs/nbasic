#include "stdafx.h"
#include "UnitTest.h"

// #include "../../Source/UnitTest/UnitTest.h"
// #include "../../Source/Locale.h"
// #include "../../Source/Stream/Accessor.h"
// #include "../../Source/Stream/CharFormat.h"
// #include "../../Source/Stream/FileStream.h"
// #include "../../Source/Collections/OperationForEach.h"
//
// using namespace vl;
// using namespace vl::collections;
// using namespace vl::stream;

extern WString GetTestResourcePath();
extern WString GetTestOutputPath();

TEST_CASE(TestLocaleFormat)
{
	DateTime dt = DateTime::FromDateTime(2000, 1, 2, 13, 2, 3);
	WString df = L"yyyy,MM,MMM,MMMM,dd,ddd,dddd";
	WString ds = L"2000,01,Jan,January,02,Sun,Sunday";
	WString tf = L"hh,HH,mm,ss,tt";
	WString ts = L"01,13,02,03,PM";
	TEST_ASSERT(INVLOC.FormatDate(df, dt) == ds);
	TEST_ASSERT(INVLOC.FormatTime(tf, dt) == ts);
}

TEST_CASE(TestLocaleCompare)
{
	TEST_ASSERT(INVLOC.Compare(L"abc", L"abc", NLocale::None) == 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"ABC", NLocale::None) != 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"abc", NLocale::IgnoreCase) == 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"ABC", NLocale::IgnoreCase) == 0);

	TEST_ASSERT(INVLOC.CompareOrdinal(L"abc", L"abc") == 0);
	TEST_ASSERT(INVLOC.CompareOrdinal(L"abc", L"ABC") != 0);
	TEST_ASSERT(INVLOC.CompareOrdinalIgnoreCase(L"abc", L"abc") == 0);
	TEST_ASSERT(INVLOC.CompareOrdinalIgnoreCase(L"abc", L"ABC") == 0);
}

TEST_CASE(TestLocaleFind)
{
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"vczh", NLocale::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"vczh", NLocale::None).key == 3);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"Vczh", NLocale::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"Vczh", NLocale::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"vczh", NLocale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"vczh", NLocale::IgnoreCase).key == 3);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"Vczh", NLocale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"Vczh", NLocale::IgnoreCase).key == 3);

	TEST_ASSERT(INVLOC.FindLast(L"abc", L"vczh", NLocale::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"vczh", NLocale::None).key == 10);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"Vczh", NLocale::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"Vczh", NLocale::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"vczh", NLocale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"vczh", NLocale::IgnoreCase).key == 10);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"Vczh", NLocale::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"Vczh", NLocale::IgnoreCase).key == 10);

	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"a", NLocale::None) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"A", NLocale::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"c", NLocale::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"C", NLocale::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"a", NLocale::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"A", NLocale::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"c", NLocale::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"C", NLocale::IgnoreCase) == false);

	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"a", NLocale::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"A", NLocale::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"c", NLocale::None) == true);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"C", NLocale::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"a", NLocale::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"A", NLocale::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"c", NLocale::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"C", NLocale::IgnoreCase) == true);
}

TEST_CASE(TestLocaleCase)
{
	TEST_ASSERT(INVLOC.ToUpper(L"abcABC") == L"ABCABC");
	TEST_ASSERT(INVLOC.ToLower(L"abcABC") == L"abcabc");
	TEST_ASSERT(INVLOC.ToLinguisticUpper(L"abcABC") == L"ABCABC");
	TEST_ASSERT(INVLOC.ToLinguisticLower(L"abcABC") == L"abcabc");
}
