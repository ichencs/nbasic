// #include "..\stdafx.h"
#include "Locale.h"
#include <Windows.h>


#if defined VCZH_MSVC

	extern SYSTEMTIME DateTimeToSystemTime(const DateTime& dateTime);

	BOOL CALLBACK Locale_EnumLocalesProcEx(
		_In_  LPWSTR lpLocaleString,
		_In_  DWORD dwFlags,
		_In_  LPARAM lParam
		)
	{
		((NList<Locale>*)lParam)->Add(Locale(lpLocaleString));
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

	DWORD TranslateNormalization(Locale::Normalization normalization)
	{
		DWORD result=0;
		if(normalization&Locale::IgnoreCase) result|=NORM_IGNORECASE;
		if(normalization&Locale::IgnoreCaseLinguistic) result|=NORM_IGNORECASE | NORM_LINGUISTIC_CASING;
		if(normalization&Locale::IgnoreKanaType) result|=NORM_IGNOREKANATYPE;
		if(normalization&Locale::IgnoreNonSpace) result|=NORM_IGNORENONSPACE;
		if(normalization&Locale::IgnoreSymbol) result|=NORM_IGNORESYMBOLS;
		if(normalization&Locale::IgnoreWidth) result|=NORM_IGNOREWIDTH;
		if(normalization&Locale::DigitsAsNumbers) result|=SORT_DIGITSASNUMBERS;
		if(normalization&Locale::StringSoft) result|=SORT_STRINGSORT;
		return result;
	}

#endif

/***********************************************************************
Locale
***********************************************************************/

	Locale::Locale(const WString& _localeName)
		:localeName(_localeName)
	{
	}

	Locale::~Locale()
	{
	}

	Locale Locale::Invariant()
	{
		return Locale(LOCALE_NAME_INVARIANT);
	}

	Locale Locale::SystemDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH+1]={0};
		GetSystemDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return Locale(buffer);
	}

	Locale Locale::UserDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH+1]={0};
		GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return Locale(buffer);
	}

	void Locale::Enumerate(NList<Locale>& locales)
	{
		EnumSystemLocalesEx(&Locale_EnumLocalesProcEx, LOCALE_ALL, (LPARAM)&locales, NULL);
	}

	const WString& Locale::GetName()const
	{
		return localeName;
	}

	void Locale::GetShortDateFormats(NList<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_SHORTDATE, (LPARAM)&formats);
	}

	void Locale::GetLongDateFormats(NList<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_LONGDATE, (LPARAM)&formats);
	}

	void Locale::GetYearMonthDateFormats(NList<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_YEARMONTH, (LPARAM)&formats);
	}

	void Locale::GetLongTimeFormats(NList<WString>& formats)const
	{
		EnumTimeFormatsEx(&EnumTimeFormatsProcEx, localeName.Buffer(), 0, (LPARAM)&formats);
	}

	void Locale::GetShortTimeFormats(NList<WString>& formats)const
	{
		EnumTimeFormatsEx(&EnumTimeFormatsProcEx, localeName.Buffer(), TIME_NOSECONDS, (LPARAM)&formats);
	}

	WString Locale::FormatDate(const WString& format, DateTime date)const
	{
		SYSTEMTIME st=DateTimeToSystemTime(date);
		int length=GetDateFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), NULL, 0, NULL);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetDateFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), &buffer[0], (int)buffer.Count(), NULL);
		return &buffer[0];
	}

	WString Locale::FormatTime(const WString& format, DateTime time)const
	{
		SYSTEMTIME st=DateTimeToSystemTime(time);
		int length=GetTimeFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), NULL, 0);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetTimeFormatEx(localeName.Buffer(), 0, &st, format.Buffer(),&buffer[0], (int)buffer.Count());
		return &buffer[0];
	}

	WString Locale::FormatNumber(const WString& number)const
	{
		int length=GetNumberFormatEx(localeName.Buffer(), 0, number.Buffer(), NULL, NULL, 0);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetNumberFormatEx(localeName.Buffer(), 0, number.Buffer(), NULL, &buffer[0], (int)buffer.Count());
		return &buffer[0];
	}

	WString Locale::FormatCurrency(const WString& currency)const
	{
		int length=GetCurrencyFormatEx(localeName.Buffer(), 0, currency.Buffer(), NULL, NULL, 0);
		if(length==0) return L"";
		NArray<wchar_t> buffer(length);
		GetCurrencyFormatEx(localeName.Buffer(), 0, currency.Buffer(), NULL, &buffer[0], (int)buffer.Count());
		return &buffer[0];
	}

	WString Locale::GetShortDayOfWeekName(nint dayOfWeek)const
	{
		return FormatDate(L"ddd", DateTime::FromDateTime(2000, 1, 2+dayOfWeek));
	}

	WString Locale::GetLongDayOfWeekName(nint dayOfWeek)const
	{
		return FormatDate(L"dddd", DateTime::FromDateTime(2000, 1, 2+dayOfWeek));
	}

	WString Locale::GetShortMonthName(nint month)const
	{
		return FormatDate(L"MMM", DateTime::FromDateTime(2000, month, 1));
	}

	WString Locale::GetLongMonthName(nint month)const
	{
		return FormatDate(L"MMMM", DateTime::FromDateTime(2000, month, 1));
	}

	WString Locale::ToFullWidth(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_FULLWIDTH);
	}

	WString Locale::ToHalfWidth(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_HALFWIDTH);
	}

	WString Locale::ToHiragana(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_HIRAGANA);
	}

	WString Locale::ToKatagana(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_KATAKANA);
	}

	WString Locale::ToLower(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_LOWERCASE);
	}

	WString Locale::ToUpper(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_UPPERCASE);
	}

	WString Locale::ToLinguisticLower(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING);
	}

	WString Locale::ToLinguisticUpper(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING);
	}

	WString Locale::ToSimplifiedChinese(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_SIMPLIFIED_CHINESE);
	}

	WString Locale::ToTraditionalChinese(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_TRADITIONAL_CHINESE);
	}

	WString Locale::ToTileCase(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_TITLECASE);
	}

	nint Locale::Compare(const WString& s1, const WString& s2, Normalization normalization)const
	{
		switch(CompareStringEx(localeName.Buffer(), TranslateNormalization(normalization), s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), NULL, NULL, NULL))
		{
		case CSTR_LESS_THAN: return -1;
		case CSTR_GREATER_THAN: return 1;
		default: return 0;
		}
	}

	nint Locale::CompareOrdinal(const WString& s1, const WString& s2)const
	{
		switch(CompareStringOrdinal(s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), FALSE))
		{
		case CSTR_LESS_THAN: return -1;
		case CSTR_GREATER_THAN: return 1;
		default: return 0;
		}
	}

	nint Locale::CompareOrdinalIgnoreCase(const WString& s1, const WString& s2)const
	{
		switch(CompareStringOrdinal(s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), TRUE))
		{
		case CSTR_LESS_THAN: return -1;
		case CSTR_GREATER_THAN: return 1;
		default: return 0;
		}
	}

	Pair<nint, nint> Locale::FindFirst(const WString& text, const WString& find, Normalization normalization)const
	{
		int length=0;
		int result=FindNLSStringEx(localeName.Buffer(), FIND_FROMSTART | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), &length, NULL, NULL, NULL);
		return result==-1?Pair<nint, nint>(-1, 0):Pair<nint, nint>(result, length);
	}

	Pair<nint, nint> Locale::FindLast(const WString& text, const WString& find, Normalization normalization)const
	{
		int length=0;
		int result=FindNLSStringEx(localeName.Buffer(), FIND_FROMEND | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), &length, NULL, NULL, NULL);
		return result==-1?Pair<nint, nint>(-1, 0):Pair<nint, nint>(result, length);
	}

	bool Locale::StartsWith(const WString& text, const WString& find, Normalization normalization)const
	{
		int result=FindNLSStringEx(localeName.Buffer(), FIND_STARTSWITH | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), NULL, NULL, NULL, NULL);
		return result!=-1;
	}

	bool Locale::EndsWith(const WString& text, const WString& find, Normalization normalization)const
	{
		int result=FindNLSStringEx(localeName.Buffer(), FIND_ENDSWITH | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), NULL, NULL, NULL, NULL);
		return result!=-1;
	}
