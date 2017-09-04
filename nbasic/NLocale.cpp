// #include "..\stdafx.h"
#include "NLocale.h"
#include <Windows.h>


#if defined VCZH_MSVC

	extern SYSTEMTIME DateTimeToSystemTime(const DateTime& dateTime);

	BOOL CALLBACK Locale_EnumLocalesProcEx(
		_In_  LPWSTR lpLocaleString,
		_In_  DWORD dwFlags,
		_In_  LPARAM lParam
		)
	{
		((NList<NLocale>*)lParam)->Add(NLocale(lpLocaleString));
		return TRUE;
	}

	BOOL CALLBACK Locale_EnumDateFormatsProcExEx(
		_In_  LPWSTR lpDateFormatString,
		_In_  CALID CalendarID,
		_In_  LPARAM lParam
	)
	{
		((NList<WString>*)lParam)->Add(lpDateFormatString);
		return TRUE;
	}

	BOOL CALLBACK EnumTimeFormatsProcEx(
		_In_  LPWSTR lpTimeFormatString,
		_In_  LPARAM lParam
	)
	{
		((NList<WString>*)lParam)->Add(lpTimeFormatString);
		return TRUE;
	}

	WString Transform(const WString& localeName, const WString& input, DWORD flag)
	{
		int length=LCMapStringEx(localeName.Buffer(), flag, input.Buffer(), (int)input.Length()+1, NULL, 0, NULL, NULL, NULL);
		NArray<wchar_t> buffer(length);
		LCMapStringEx(localeName.Buffer(), flag, input.Buffer(), (int)input.Length()+1, &buffer[0], (int)buffer.Count(), NULL, NULL, NULL);
		return &buffer[0];
	}

	DWORD TranslateNormalization(NLocale::Normalization normalization)
	{
		DWORD result=0;
		if(normalization&NLocale::IgnoreCase) result|=NORM_IGNORECASE;
		if(normalization&NLocale::IgnoreCaseLinguistic) result|=NORM_IGNORECASE | NORM_LINGUISTIC_CASING;
		if(normalization&NLocale::IgnoreKanaType) result|=NORM_IGNOREKANATYPE;
		if(normalization&NLocale::IgnoreNonSpace) result|=NORM_IGNORENONSPACE;
		if(normalization&NLocale::IgnoreSymbol) result|=NORM_IGNORESYMBOLS;
		if(normalization&NLocale::IgnoreWidth) result|=NORM_IGNOREWIDTH;
// 		if(normalization&Locale::DigitsAsNumbers) result|=SORT_DIGITSASNUMBERS;
		if(normalization&NLocale::StringSoft) result|=SORT_STRINGSORT;
		return result;
	}

#endif

/***********************************************************************
Locale
***********************************************************************/

	NLocale::NLocale(const WString& _localeName)
		:localeName(_localeName)
	{
	}

	NLocale::~NLocale()
	{
	}

	NLocale NLocale::Invariant()
	{
		return NLocale(LOCALE_NAME_INVARIANT);
	}

	NLocale NLocale::SystemDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH+1]={0};
		GetSystemDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return NLocale(buffer);
	}

	NLocale NLocale::UserDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH+1]={0};
		GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return NLocale(buffer);
	}

	void NLocale::Enumerate(NList<NLocale>& locales)
	{
		EnumSystemLocalesEx(&Locale_EnumLocalesProcEx, LOCALE_ALL, (LPARAM)&locales, NULL);
	}

	const WString& NLocale::GetName()const
	{
		return localeName;
	}

	void NLocale::GetShortDateFormats(NList<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_SHORTDATE, (LPARAM)&formats);
	}

	void NLocale::GetLongDateFormats(NList<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_LONGDATE, (LPARAM)&formats);
	}

	void NLocale::GetYearMonthDateFormats(NList<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_YEARMONTH, (LPARAM)&formats);
	}

	void NLocale::GetLongTimeFormats(NList<WString>& formats)const
	{
		EnumTimeFormatsEx(&EnumTimeFormatsProcEx, localeName.Buffer(), 0, (LPARAM)&formats);
	}

	void NLocale::GetShortTimeFormats(NList<WString>& formats)const
	{
		EnumTimeFormatsEx(&EnumTimeFormatsProcEx, localeName.Buffer(), TIME_NOSECONDS, (LPARAM)&formats);
	}

	WString NLocale::FormatDate(const WString& format, DateTime date)const
	{
		SYSTEMTIME st=DateTimeToSystemTime(date);
		int length=GetDateFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), NULL, 0, NULL);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetDateFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), &buffer[0], (int)buffer.Count(), NULL);
		return &buffer[0];
	}

	WString NLocale::FormatTime(const WString& format, DateTime time)const
	{
		SYSTEMTIME st=DateTimeToSystemTime(time);
		int length=GetTimeFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), NULL, 0);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetTimeFormatEx(localeName.Buffer(), 0, &st, format.Buffer(),&buffer[0], (int)buffer.Count());
		return &buffer[0];
	}

	WString NLocale::FormatNumber(const WString& number)const
	{
		int length=GetNumberFormatEx(localeName.Buffer(), 0, number.Buffer(), NULL, NULL, 0);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetNumberFormatEx(localeName.Buffer(), 0, number.Buffer(), NULL, &buffer[0], (int)buffer.Count());
		return &buffer[0];
	}

	WString NLocale::FormatCurrency(const WString& currency)const
	{
		int length=GetCurrencyFormatEx(localeName.Buffer(), 0, currency.Buffer(), NULL, NULL, 0);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetCurrencyFormatEx(localeName.Buffer(), 0, currency.Buffer(), NULL, &buffer[0], (int)buffer.Count());
		return &buffer[0];
	}

	WString NLocale::GetShortDayOfWeekName(nint dayOfWeek)const
	{
		return FormatDate(L"ddd", DateTime::FromDateTime(2000, 1, 2+dayOfWeek));
	}

	WString NLocale::GetLongDayOfWeekName(nint dayOfWeek)const
	{
		return FormatDate(L"dddd", DateTime::FromDateTime(2000, 1, 2+dayOfWeek));
	}

	WString NLocale::GetShortMonthName(nint month)const
	{
		return FormatDate(L"MMM", DateTime::FromDateTime(2000, month, 1));
	}

	WString NLocale::GetLongMonthName(nint month)const
	{
		return FormatDate(L"MMMM", DateTime::FromDateTime(2000, month, 1));
	}

	WString NLocale::ToFullWidth(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_FULLWIDTH);
	}

	WString NLocale::ToHalfWidth(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_HALFWIDTH);
	}

	WString NLocale::ToHiragana(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_HIRAGANA);
	}

	WString NLocale::ToKatagana(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_KATAKANA);
	}

	WString NLocale::ToLower(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_LOWERCASE);
	}

	WString NLocale::ToUpper(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_UPPERCASE);
	}

	WString NLocale::ToLinguisticLower(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING);
	}

	WString NLocale::ToLinguisticUpper(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING);
	}

	WString NLocale::ToSimplifiedChinese(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_SIMPLIFIED_CHINESE);
	}

	WString NLocale::ToTraditionalChinese(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_TRADITIONAL_CHINESE);
	}

// #if (WINVER >= _WIN32_WINNT_WIN7)
// 	WString Locale::ToTileCase(const WString& str)const
// 	{
// 		return Transform(localeName, str, LCMAP_TITLECASE);
// 	}
// #endif

	nint NLocale::Compare(const WString& s1, const WString& s2, Normalization normalization)const
	{
		switch(CompareStringEx(localeName.Buffer(), TranslateNormalization(normalization), s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), NULL, NULL, NULL))
		{
		case CSTR_LESS_THAN: return -1;
		case CSTR_GREATER_THAN: return 1;
		default: return 0;
		}
	}

	nint NLocale::CompareOrdinal(const WString& s1, const WString& s2)const
	{
		switch(CompareStringOrdinal(s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), FALSE))
		{
		case CSTR_LESS_THAN: return -1;
		case CSTR_GREATER_THAN: return 1;
		default: return 0;
		}
	}

	nint NLocale::CompareOrdinalIgnoreCase(const WString& s1, const WString& s2)const
	{
		switch(CompareStringOrdinal(s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), TRUE))
		{
		case CSTR_LESS_THAN: return -1;
		case CSTR_GREATER_THAN: return 1;
		default: return 0;
		}
	}

	NPair<nint, nint> NLocale::FindFirst(const WString& text, const WString& find, Normalization normalization)const
	{
		int length=0;
		int result=FindNLSStringEx(localeName.Buffer(), FIND_FROMSTART | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), &length, NULL, NULL, NULL);
		return result==-1?NPair<nint, nint>(-1, 0):NPair<nint, nint>(result, length);
	}

	NPair<nint, nint> NLocale::FindLast(const WString& text, const WString& find, Normalization normalization)const
	{
		int length=0;
		int result=FindNLSStringEx(localeName.Buffer(), FIND_FROMEND | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), &length, NULL, NULL, NULL);
		return result==-1?NPair<nint, nint>(-1, 0):NPair<nint, nint>(result, length);
	}

	bool NLocale::StartsWith(const WString& text, const WString& find, Normalization normalization)const
	{
		int result=FindNLSStringEx(localeName.Buffer(), FIND_STARTSWITH | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), NULL, NULL, NULL, NULL);
		return result!=-1;
	}

	bool NLocale::EndsWith(const WString& text, const WString& find, Normalization normalization)const
	{
		int result=FindNLSStringEx(localeName.Buffer(), FIND_ENDSWITH | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), NULL, NULL, NULL, NULL);
		return result!=-1;
	}
