#include "stdafx.h"
#include "UnitTest.h"

extern WString GetTestOutputPath();
extern WString GetTestResourcePath();

void ClearTestFolders()
{
	NFilePath path = GetTestOutputPath();
	TEST_ASSERT(path.IsFolder());

	NFolder folder(GetTestOutputPath() + L"FileSyetem");

	WString folderPath = folder.GetFilePath().GetFullPath();
	TEST_ASSERT(folderPath[1] == L':');


}


TEST_CASE(TestFilePath)
{


}
