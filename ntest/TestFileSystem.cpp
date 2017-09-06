#include "stdafx.h"
#include "UnitTest.h"

extern WString GetTestOutputPath();
extern WString GetTestResourcePath();

void ClearTestFolders()
{
	NFilePath path = GetTestOutputPath();
	TEST_ASSERT(path.IsFolder());

	NFolder folder(GetTestOutputPath() + L"FileSystem");

	WString folderPath = folder.GetFilePath().GetFullPath();
	TEST_ASSERT(folderPath[1] == L':');

	TEST_ASSERT(INVLOC.FindLast(folderPath, L"\\FileSystem", NLocale::None).key == folderPath.Length() - 11);

	if (folder.Exists())
	{
		TEST_ASSERT(folder.Delete(true) == true);
	}

	TEST_ASSERT(folder.Create(false) == true);
	TEST_ASSERT(folder.Exists() == true);
	TEST_ASSERT(folder.Create(false) == false);
}


TEST_CASE(TestFilePath)
{
	ClearTestFolders();
	{
		NFilePath p;
		TEST_ASSERT(p.IsFile() == false);
		TEST_ASSERT(p.IsFolder() == false);
		TEST_ASSERT(p.IsRoot() == true);
		TEST_ASSERT(p.GetFullPath() == L"");
		TEST_ASSERT(p.GetName() == L"");
		TEST_ASSERT(p.GetFolder().GetFullPath() == L"");
		p = GetTestOutputPath();
		TEST_ASSERT(p.IsFolder() == true);
		Console::WriteLine(L"²âÊÔÎÄ¼þ¼ÐÄ¿Â¼£º" + p.GetFullPath());
	}




}
