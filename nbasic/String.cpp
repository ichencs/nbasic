#include "stdafx.h"
#include <stdlib.h>
#include "String.h"
#include <Windows.h>


	nint atoi_test(const AString& string, bool& success)
	{
		char* endptr = 0;
		nint result = strtol(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && itoa(result) == string;
		return result;
	}

	nint wtoi_test(const WString& string, bool& success)
	{
		wchar_t* endptr = 0;
		nint result = wcstol(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && itow(result) == string;
		return result;
	}

	nint64_t atoi64_test(const AString& string, bool& success)
	{
		char* endptr = 0;
		nint64_t result = _strtoi64(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && i64toa(result) == string;
		return result;
	}

	nint64_t wtoi64_test(const WString& string, bool& success)
	{
		wchar_t* endptr = 0;
		nint64_t result = _wcstoi64(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && i64tow(result) == string;
		return result;
	}

	nuint atou_test(const AString& string, bool& success)
	{
		char* endptr = 0;
		nuint result = strtoul(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && utoa(result) == string;
		return result;
	}

	nuint wtou_test(const WString& string, bool& success)
	{
		wchar_t* endptr = 0;
		nuint result = wcstoul(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && utow(result) == string;
		return result;
	}

	nuint64_t atou64_test(const AString& string, bool& success)
	{
		char* endptr = 0;
		nuint64_t result = _strtoui64(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && u64toa(result) == string;
		return result;
	}

	nuint64_t wtou64_test(const WString& string, bool& success)
	{
		wchar_t* endptr = 0;
		nuint64_t result = _wcstoui64(string.Buffer(), &endptr, 10);
		success = endptr == string.Buffer() + string.Length() && u64tow(result) == string;
		return result;
	}

	double atof_test(const AString& string, bool& success)
	{
		char* endptr = 0;
		double result = strtod(string.Buffer(), &endptr);
		success = endptr == string.Buffer() + string.Length();
		return result;
	}

	double wtof_test(const WString& string, bool& success)
	{
		wchar_t* endptr = 0;
		double result = wcstod(string.Buffer(), &endptr);
		success = endptr == string.Buffer() + string.Length();
		return result;
	}

	nint atoi(const AString& string)
	{
		bool success = false;
		return atoi_test(string, success);
	}

	nint wtoi(const WString& string)
	{
		bool success = false;
		return wtoi_test(string, success);
	}

	nint64_t atoi64(const AString& string)
	{
		bool success = false;
		return atoi64_test(string, success);
	}

	nint64_t wtoi64(const WString& string)
	{
		bool success = false;
		return wtoi64_test(string, success);
	}

	nuint atou(const AString& string)
	{
		bool success = false;
		return atou_test(string, success);
	}

	nuint wtou(const WString& string)
	{
		bool success = false;
		return wtou_test(string, success);
	}

	nuint64_t atou64(const AString& string)
	{
		bool success = false;
		return atou64_test(string, success);
	}

	nuint64_t wtou64(const WString& string)
	{
		bool success = false;
		return wtou64_test(string, success);
	}

	double atof(const AString& string)
	{
		bool success = false;
		return atof_test(string, success);
	}

	double wtof(const WString& string)
	{
		bool success = false;
		return wtof_test(string, success);
	}

	AString itoa(nint number)
	{
		char buffer[100];
		ITOA_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	WString itow(nint number)
	{
		wchar_t buffer[100];
		ITOW_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	AString i64toa(nint64_t number)
	{
		char buffer[100];
		I64TOA_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	WString i64tow(nint64_t number)
	{
		wchar_t buffer[100];
		I64TOW_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	AString utoa(nuint number)
	{
		char buffer[100];
		UITOA_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	WString utow(nuint number)
	{
		wchar_t buffer[100];
		UITOW_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	AString u64toa(nuint64_t number)
	{
		char buffer[100];
		UI64TOA_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	WString u64tow(nuint64_t number)
	{
		wchar_t buffer[100];
		UI64TOW_S(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
		return buffer;
	}

	AString ftoa(double number)
	{
		char buffer[320];
		_gcvt_s(buffer, 320, number, 30);
		nint len = (nint)strlen(buffer);
		if (buffer[len - 1] == '.')
		{
			buffer[len - 1] = '\0';
		}
		return buffer;
	}

	WString ftow(double number)
	{
		return atow(ftoa(number));
	}

	nint _wtoa(const wchar_t* w, char* a, nint  chars)
	{
		return WideCharToMultiByte(CP_THREAD_ACP, 0, w, -1, a, (int)(a ? chars : 0), 0, 0);
	}

	AString wtoa(const WString& string)
	{
		nint len = _wtoa(string.Buffer(), 0, 0);
		char* buffer = new char[len];
		memset(buffer, 0, len*sizeof(*buffer));
		_wtoa(string.Buffer(), buffer, (int)len);
		AString s = buffer;
		delete[] buffer;
		return s;
	}

	nint _atow(const char* a, wchar_t* w, nint  chars)
	{
		return MultiByteToWideChar(CP_THREAD_ACP, 0, a, -1, w, (int)(w ? chars : 0));
	}

	WString atow(const AString& string)
	{
		nint len = _atow(string.Buffer(), 0, 0);
		wchar_t* buffer = new wchar_t[len];
		memset(buffer, 0, len*sizeof(*buffer));
		_atow(string.Buffer(), buffer, (int)len);
		WString s = buffer;
		delete[] buffer;
		return s;
	}

	AString alower(const AString& string)
	{
		AString result = string.Buffer();
		_strlwr_s((char*)result.Buffer(), result.Length() + 1);
		return result;
	}

	WString wlower(const WString& string)
	{
		WString result = string.Buffer();
		_wcslwr_s((wchar_t*)result.Buffer(), result.Length() + 1);
		return result;
	}

	AString aupper(const AString& string)
	{
		AString result = string.Buffer();
		_strupr_s((char*)result.Buffer(), result.Length() + 1);
		return result;
	}

	WString wupper(const WString& string)
	{
		WString result = string.Buffer();
		_wcsupr_s((wchar_t*)result.Buffer(), result.Length() + 1);
		return result;
	}
