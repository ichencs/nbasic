// ntest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "UnitTest.h"
#include <windows.h>

WString GetExePath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer) / sizeof(*buffer));
	vint pos = -1;
	vint index = 0;
	
	while (buffer[index])
	{
		if (buffer[index] == L'\\' || buffer[index] == L'/')
		{
			pos = index;
		}
		
		index++;
	}
	
	return WString(buffer, pos + 1);
}

WString GetTestResourcePath()
{
#ifdef _WIN64
	return GetExePath() + L"../../Resources/";
#else
	return GetExePath() + L"../Resources/";
#endif
}

WString GetTestOutputPath()
{
#ifdef _WIN64
	return GetExePath() + L"../../Output/";
#else
	return GetExePath() + L"../Output/";
#endif
}


int main()
{

	{
		using namespace filesystem;
		Folder folder(GetTestOutputPath());
		
		if (!folder.Exists())
		{
			TEST_ASSERT(folder.Create(false) == true);
		}
	}
	UnitTest::RunAndDisposeTests();
	FinalizeGlobalStorage();
	system("pause");
	return 0;
}

