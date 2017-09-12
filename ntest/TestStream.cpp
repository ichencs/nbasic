﻿#include "stdafx.h"
#include <string.h>
#include "UnitTest.h"
// #include "../../Source/UnitTest/UnitTest.h"
// #include "../../Source/Console.h"
// #include "../../Source/Stream/Interfaces.h"
// #include "../../Source/Stream/MemoryWrapperStream.h"
// #include "../../Source/Stream/MemoryStream.h"
// #include "../../Source/Stream/FileStream.h"
// #include "../../Source/Stream/RecorderStream.h"
// #include "../../Source/Stream/BroadcastStream.h"
// #include "../../Source/Stream/CacheStream.h"
// #include "../../Source/Stream/Accessor.h"
// #include "../../Source/Stream/CharFormat.h"
// #include "../../Source/Stream/CompressionStream.h"
// #include "../../Source/Pointer.h"
// #include "../../Source/Locale.h"

// using namespace vl;
// using namespace vl::stream;
// using namespace vl::collections;

extern WString GetTestResourcePath();
extern WString GetTestOutputPath();
const nint BUFFER_SIZE = 1024;

/***********************************************************************
通用测试方法
***********************************************************************/

void TestClosedProperty(NIStream& stream)
{
	TEST_ASSERT(stream.CanRead()==false);
	TEST_ASSERT(stream.CanWrite()==false);
	TEST_ASSERT(stream.CanPeek()==false);
	TEST_ASSERT(stream.CanSeek()==false);
	TEST_ASSERT(stream.IsLimited()==false);
	TEST_ASSERT(stream.IsAvailable()==false);
	TEST_ASSERT(stream.Position()==-1);
	TEST_ASSERT(stream.Size()==-1);
}

void TestLimitedProperty(NIStream& stream, pos_t position, pos_t size)
{
	TEST_ASSERT(stream.CanRead()==true);
	TEST_ASSERT(stream.CanWrite()==true);
	TEST_ASSERT(stream.CanPeek()==true);
	TEST_ASSERT(stream.CanSeek()==true);
	TEST_ASSERT(stream.IsLimited()==true);
	TEST_ASSERT(stream.IsAvailable()==true);
	TEST_ASSERT(stream.Position()==position);
	TEST_ASSERT(stream.Size()==size);
}

void TestBidirectionalLimitedStreamWithSize15(NIStream& stream)
{
	char buffer[BUFFER_SIZE];

	TestLimitedProperty(stream, 0, 15);
	TEST_ASSERT(stream.Write((void*)"vczh", 4)==4);
	TestLimitedProperty(stream, 4, 15);
	stream.Seek(-4);
	TestLimitedProperty(stream, 0, 15);
	TEST_ASSERT(stream.Peek(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, "vczh", 4)==0);
	TestLimitedProperty(stream, 0, 15);
	memset(buffer, 0, sizeof(buffer));
	TEST_ASSERT(stream.Read(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, "vczh", 4)==0);
	TestLimitedProperty(stream, 4, 15);

	TEST_ASSERT(stream.Write((void*)" is genius!0123456789", 21)==11);
	TestLimitedProperty(stream, 15, 15);
	stream.SeekFromEnd(7);
	TestLimitedProperty(stream, 8, 15);
	TEST_ASSERT(stream.Read(buffer, 100)==7);
	TEST_ASSERT(strncmp(buffer, "genius!", 7)==0);
	TestLimitedProperty(stream, 15, 15);

	stream.SeekFromBegin(100);
	TestLimitedProperty(stream, 15, 15);
	stream.SeekFromEnd(100);
	TestLimitedProperty(stream, 0, 15);
	stream.Seek(100);
	TestLimitedProperty(stream, 15, 15);
	stream.Seek(-100);
	TestLimitedProperty(stream, 0, 15);

	stream.Seek(4);
	TEST_ASSERT(stream.Read(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, " is ", 4)==0);
}

void TestUnlimitedProperty(NIStream& stream, pos_t position, pos_t size)
{
	TEST_ASSERT(stream.CanRead()==true);
	TEST_ASSERT(stream.CanWrite()==true);
	TEST_ASSERT(stream.CanPeek()==true);
	TEST_ASSERT(stream.CanSeek()==true);
	TEST_ASSERT(stream.IsLimited()==false);
	TEST_ASSERT(stream.IsAvailable()==true);
	TEST_ASSERT(stream.Position()==position);
	TEST_ASSERT(stream.Size()==size);
}

void TestBidirectionalUnlimitedStream(NIStream& stream)
{
	char buffer[BUFFER_SIZE];

	TestUnlimitedProperty(stream, 0, 0);
	TEST_ASSERT(stream.Write((void*)"vczh", 4)==4);
	TestUnlimitedProperty(stream, 4, 4);
	stream.Seek(-4);
	TestUnlimitedProperty(stream, 0, 4);
	TEST_ASSERT(stream.Peek(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, "vczh", 4)==0);
	TestUnlimitedProperty(stream, 0, 4);
	memset(buffer, 0, sizeof(buffer));
	TEST_ASSERT(stream.Read(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, "vczh", 4)==0);
	TestUnlimitedProperty(stream, 4, 4);

	TEST_ASSERT(stream.Write((void*)" is genius!0123456789", 11)==11);
	TestUnlimitedProperty(stream, 15, 15);
	stream.SeekFromEnd(7);
	TestUnlimitedProperty(stream, 8, 15);
	TEST_ASSERT(stream.Read(buffer, 100)==7);
	TEST_ASSERT(strncmp(buffer, "genius!", 7)==0);
	TestUnlimitedProperty(stream, 15, 15);

	stream.SeekFromBegin(100);
	TestUnlimitedProperty(stream, 15, 15);
	stream.SeekFromEnd(100);
	TestUnlimitedProperty(stream, 0, 15);
	stream.Seek(100);
	TestUnlimitedProperty(stream, 15, 15);
	stream.Seek(-100);
	TestUnlimitedProperty(stream, 0, 15);

	stream.Seek(4);
	TEST_ASSERT(stream.Read(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, " is ", 4)==0);
}

void TestReadonlySeekableProperty(NIStream& stream, pos_t position, pos_t size)
{
	TEST_ASSERT(stream.CanRead()==true);
	TEST_ASSERT(stream.CanWrite()==false);
	TEST_ASSERT(stream.CanPeek()==true);
	TEST_ASSERT(stream.CanSeek()==true);
	TEST_ASSERT(stream.IsLimited()==true);
	TEST_ASSERT(stream.IsAvailable()==true);
	TEST_ASSERT(stream.Position()==position);
	TEST_ASSERT(stream.Size()==size);
}

void TestReadonlylSeekableStreamWithSize15(NIStream& stream)
{
	char buffer[BUFFER_SIZE];

	TestReadonlySeekableProperty(stream, 0, 15);
	stream.SeekFromEnd(7);
	TestReadonlySeekableProperty(stream, 8, 15);
	TEST_ASSERT(stream.Read(buffer, 100)==7);
	TEST_ASSERT(strncmp(buffer, "genius!", 7)==0);
	TestReadonlySeekableProperty(stream, 15, 15);

	stream.SeekFromBegin(100);
	TestReadonlySeekableProperty(stream, 15, 15);
	stream.SeekFromEnd(100);
	TestReadonlySeekableProperty(stream, 0, 15);
	TEST_ASSERT(stream.Read(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, "vczh", 4)==0);
	TestReadonlySeekableProperty(stream, 4, 15);
	stream.Seek(100);
	TestReadonlySeekableProperty(stream, 15, 15);
	stream.Seek(-100);
	TestReadonlySeekableProperty(stream, 0, 15);

	stream.Seek(4);
	TEST_ASSERT(stream.Read(buffer, 4)==4);
	TEST_ASSERT(strncmp(buffer, " is ", 4)==0);
}

void TestWriteonlySeekableProperty(NIStream& stream, pos_t position, pos_t size)
{
	TEST_ASSERT(stream.CanRead()==false);
	TEST_ASSERT(stream.CanWrite()==true);
	TEST_ASSERT(stream.CanPeek()==false);
	TEST_ASSERT(stream.CanSeek()==true);
	TEST_ASSERT(stream.IsLimited()==false);
	TEST_ASSERT(stream.IsAvailable()==true);
	TEST_ASSERT(stream.Position()==position);
	TEST_ASSERT(stream.Size()==size);
}

void TestWriteonlySeekableStream(NIStream& stream)
{
	TestWriteonlySeekableProperty(stream, 0, 0);
	TEST_ASSERT(stream.Write((void*)"genius!", 7)==7);
	TestWriteonlySeekableProperty(stream, 7, 7);
	stream.Seek(-7);
	TestWriteonlySeekableProperty(stream, 0, 7);
	TEST_ASSERT(stream.Write((void*)"vczh is genius!", 15)==15);
	TestWriteonlySeekableProperty(stream, 15, 15);

	stream.SeekFromBegin(100);
	TestWriteonlySeekableProperty(stream, 15, 15);
	stream.SeekFromEnd(100);
	TestWriteonlySeekableProperty(stream, 0, 15);
	stream.Seek(100);
	TestWriteonlySeekableProperty(stream, 15, 15);
	stream.Seek(-100);
	TestWriteonlySeekableProperty(stream, 0, 15);
}

void TestReadonlyUnseekableProperty(NIStream& stream, pos_t position, pos_t size, bool limited)
{
	TEST_ASSERT(stream.CanRead()==true);
	TEST_ASSERT(stream.CanWrite()==false);
	TEST_ASSERT(stream.CanPeek()==false);
	TEST_ASSERT(stream.CanSeek()==false);
	TEST_ASSERT(stream.IsLimited()==limited);
	TEST_ASSERT(stream.IsAvailable()==true);
	TEST_ASSERT(stream.Position()==position);
	TEST_ASSERT(stream.Size()==size);
}

void TestReadonlyUnseekableStreamWithSize15(NIStream& stream, bool limited)
{
	char buffer[BUFFER_SIZE];

	TestReadonlyUnseekableProperty(stream, 0, 15, limited);
	TEST_ASSERT(stream.Read(buffer, 8)==8);
	TEST_ASSERT(strncmp(buffer, "vczh is ", 8)==0);
	TestReadonlyUnseekableProperty(stream, 8, 15, limited);
	TEST_ASSERT(stream.Read(buffer, 100)==7);
	TEST_ASSERT(strncmp(buffer, "genius!", 7)==0);
	TestReadonlyUnseekableProperty(stream, 15, 15, limited);
}

void TestWriteonlyUnseekableProperty(NIStream& stream, pos_t position, pos_t size, bool limited)
{
	TEST_ASSERT(stream.CanRead()==false);
	TEST_ASSERT(stream.CanWrite()==true);
	TEST_ASSERT(stream.CanPeek()==false);
	TEST_ASSERT(stream.CanSeek()==false);
	TEST_ASSERT(stream.IsLimited()==limited);
	TEST_ASSERT(stream.IsAvailable()==true);
	TEST_ASSERT(stream.Position()==position);
	TEST_ASSERT(stream.Size()==size);
}

void TestWriteonlyUnseekableStream(NIStream& stream, bool limited)
{
	TestWriteonlyUnseekableProperty(stream, 0, 0, limited);
	TEST_ASSERT(stream.Write((void*)"vczh is ", 8)==8);
	TestWriteonlyUnseekableProperty(stream, 8, 8, limited);
	TEST_ASSERT(stream.Write((void*)"genius!", 7)==7);
	TestWriteonlyUnseekableProperty(stream, 15, 15, limited);
}

/***********************************************************************
普通流测试
***********************************************************************/

TEST_CASE(TestMemoryWrapperStream)
{
	char buffer[BUFFER_SIZE]={0};
	MemoryWrapperStream stream(buffer, 15);
	TestBidirectionalLimitedStreamWithSize15(stream);
	stream.Close();
	TestClosedProperty(stream);
	TEST_ASSERT(strncmp(buffer, "vczh is genius!", 15)==0);
}

TEST_CASE(TestMemoryStream)
{
	MemoryStream stream;
	TestBidirectionalUnlimitedStream(stream);
	stream.Close();
	TestClosedProperty(stream);
}

TEST_CASE(TestFileStream)
{
	NFileStream destroyer(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::WriteOnly);
	TestWriteonlySeekableProperty(destroyer, 0, 0);
	destroyer.Close();
	TestClosedProperty(destroyer);
<<<<<<< HEAD
	NFileStream tryRead(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::ReadOnly);
	TestReadonlySeekableProperty(tryRead, 0, 0);
	tryRead.Close();
	TestClosedProperty(tryRead);
	NFileStream w(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::WriteOnly);
	TestWriteonlySeekableStream(w);
	w.Close();
	TestClosedProperty(w);
	NFileStream r(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::ReadOnly);
	TestReadonlylSeekableStreamWithSize15(r);
	r.Close();
	TestClosedProperty(r);
	NFileStream rw(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::ReadWrite);
=======

	FileStream tryRead(GetTestOutputPath() + L"TestFile.ReadWrite.txt", FileStream::ReadOnly);
	TestReadonlySeekableProperty(tryRead, 0, 0);
	tryRead.Close();
	TestClosedProperty(tryRead);

	FileStream w(GetTestOutputPath() + L"TestFile.ReadWrite.txt", FileStream::WriteOnly);
	TestWriteonlySeekableStream(w);
	w.Close();
	TestClosedProperty(w);

	FileStream r(GetTestOutputPath() + L"TestFile.ReadWrite.txt", FileStream::ReadOnly);
	TestReadonlylSeekableStreamWithSize15(r);
	r.Close();
	TestClosedProperty(r);

	FileStream rw(GetTestOutputPath() + L"TestFile.ReadWrite.txt", FileStream::ReadWrite);
>>>>>>> parent of 3eea6d6... string-split
	TestBidirectionalUnlimitedStream(rw);
	rw.Close();
	TestClosedProperty(rw);
}

TEST_CASE(TestRecorderStream)
{
	char reading[]="vczh is genius!";
	char writing[BUFFER_SIZE];
	MemoryWrapperStream readingStream(reading, 15);
	MemoryWrapperStream writingStream(writing, 15);
	RecorderStream recorder(readingStream, writingStream);
	TestReadonlyUnseekableStreamWithSize15(recorder, true);
	TEST_ASSERT(strncmp(writing, "vczh is genius!", 15)==0);
	recorder.Close();
	TestClosedProperty(recorder);
}

TEST_CASE(TestBroadcastStream)
{
	char buffer1[BUFFER_SIZE];
	char buffer2[BUFFER_SIZE];
	MemoryWrapperStream target1(buffer1, 15);
	MemoryWrapperStream target2(buffer2, 15);
	BroadcastStream stream;
	stream.Targets().Add(&target1);
	stream.Targets().Add(&target2);
	TestWriteonlyUnseekableStream(stream, false);
	TEST_ASSERT(strncmp(buffer1, "vczh is genius!", 15)==0);
	TEST_ASSERT(strncmp(buffer2, "vczh is genius!", 15)==0);
	stream.Close();
	TestClosedProperty(stream);
}

/***********************************************************************
缓存流测试
***********************************************************************/

TEST_CASE(TestCacheStreamWithReadonlyUnseekableStream)
{
	char reading[]="vczh is genius!";
	char writing[BUFFER_SIZE];
	MemoryWrapperStream readingStream(reading, 15);
	MemoryWrapperStream writingStream(writing, 15);
	RecorderStream recorder(readingStream, writingStream);
	CacheStream cache(recorder, 4);
	TestReadonlyUnseekableStreamWithSize15(cache, true);
	cache.Close();
	TestClosedProperty(cache);
	TEST_ASSERT(strncmp(writing, "vczh is genius!", 15)==0);
}

TEST_CASE(TestCacheStreamWithWriteOnlyUnseekableStream)
{
	char buffer[BUFFER_SIZE];
	MemoryWrapperStream target(buffer, 15);
	BroadcastStream broadcast;
	broadcast.Targets().Add(&target);
	CacheStream cache(broadcast, 4);
	TestWriteonlyUnseekableStream(cache, false);
	cache.Close();
	TestClosedProperty(cache);
	TEST_ASSERT(strncmp(buffer, "vczh is genius!", 15)==0);
}

TEST_CASE(TestCacheStreamWithSeekableStream)
{
	NFileStream w(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::WriteOnly);
	CacheStream cw(w, 4);
	TestWriteonlySeekableStream(cw);
	cw.Close();
	TestClosedProperty(cw);
	w.Close();
	TestClosedProperty(w);
<<<<<<< HEAD
	NFileStream r(GetTestOutputPath() + L"TestFile.ReadWrite.txt", NFileStream::ReadOnly);
=======

	FileStream r(GetTestOutputPath() + L"TestFile.ReadWrite.txt", FileStream::ReadOnly);
>>>>>>> parent of 3eea6d6... string-split
	CacheStream cr(r, 4);
	TestReadonlylSeekableStreamWithSize15(cr);
	cr.Close();
	TestClosedProperty(cr);
	r.Close();
	TestClosedProperty(r);
}

TEST_CASE(TestCacheStreamWithBidirectionalLimitedStream)
{
	char buffer[BUFFER_SIZE];
	MemoryWrapperStream memory(buffer, 15);
	CacheStream cache(memory, 4);
	TestBidirectionalLimitedStreamWithSize15(cache);
	cache.Close();
	TestClosedProperty(cache);
	TEST_ASSERT(strncmp(buffer, "vczh is genius!", 15)==0);
}

TEST_CASE(TestCacheStreamWithBidirectionalUnlimitedStream)
{
	MemoryStream memory;
	CacheStream cache(memory, 4);
	TestBidirectionalUnlimitedStream(cache);
	cache.Close();
	TestClosedProperty(cache);
}

TEST_CASE(TestCacheStream)
{
	char buffer[BUFFER_SIZE];

	MemoryStream memory;
	CacheStream cache(memory, 4);
	TestUnlimitedProperty(cache, 0, 0);

	TEST_ASSERT(cache.Write((void*)"vcz", 3)==3);
	TestUnlimitedProperty(cache, 3, 3);
	cache.Seek(-2);
	TestUnlimitedProperty(cache, 1, 3);
	TEST_ASSERT(cache.Read(buffer, 4)==2);
	TEST_ASSERT(strncmp(buffer, "cz", 2)==0);
	TestUnlimitedProperty(cache, 3, 3);

	TEST_ASSERT(cache.Write((void*)"h ", 2)==2);
	TestUnlimitedProperty(cache, 5, 5);
	cache.Seek(-5);
	TestUnlimitedProperty(cache, 0, 5);
	TEST_ASSERT(cache.Write((void*)"V", 1)==1);
	TestUnlimitedProperty(cache, 1, 5);
	cache.SeekFromEnd(1);
	TestUnlimitedProperty(cache, 4, 5);
	TEST_ASSERT(cache.Read(buffer, 4)==1);
	TEST_ASSERT(strncmp(buffer, " ", 1)==0);
	TestUnlimitedProperty(cache, 5, 5);

	TEST_ASSERT(cache.Write((void*)"is", 2)==2);
	TestUnlimitedProperty(cache, 7, 7);
	TEST_ASSERT(cache.Write((void*)" genius!", 8)==8);
	TestUnlimitedProperty(cache, 15, 15);
	cache.Seek(-8);
	TEST_ASSERT(cache.Read(buffer, 1)==1);
	TEST_ASSERT(cache.Read(buffer+1, 1)==1);
	TEST_ASSERT(cache.Read(buffer+2, 1)==1);
	TEST_ASSERT(cache.Read(buffer+3, 1)==1);
	TEST_ASSERT(strncmp(buffer, " gen", 4)==0);

	cache.SeekFromBegin(0);
	TestUnlimitedProperty(cache, 0, 15);
	TEST_ASSERT(cache.Read(buffer, 14)==14);
	TEST_ASSERT(strncmp(buffer, "Vczh is genius", 14)==0);

	cache.Close();
	TestClosedProperty(cache);
	memory.SeekFromBegin(0);
	TEST_ASSERT(memory.Read(buffer, 15)==15);
	TEST_ASSERT(strncmp(buffer, "Vczh is genius!", 15)==0);
}

/***********************************************************************
流控制器测试(StringReader)
***********************************************************************/

TEST_CASE(TestStringReader)
{
	const wchar_t text[]=L"1:Vczh is genius!\r\n2:Vczh is genius!\r\n3:Vczh is genius!\r\n4:Vczh is genius!";
	StringReader reader(text);

	TEST_ASSERT(reader.ReadChar()==L'1');
	TEST_ASSERT(reader.ReadString(5)==L":Vczh");
	TEST_ASSERT(reader.ReadLine()==L" is genius!");
	TEST_ASSERT(reader.ReadLine()==L"2:Vczh is genius!");
	TEST_ASSERT(reader.ReadToEnd()==L"3:Vczh is genius!\r\n4:Vczh is genius!");
}

TEST_CASE(TestStringReaderWithCrLf)
{
	const wchar_t text[]=L"1:Vczh is genius!\r\n2:Vczh is genius!!\r\n3:Vczh is genius!!!\r\n4:Vczh is genius!!!!\r\n";
	const wchar_t* lines[]={L"1:Vczh is genius!", L"2:Vczh is genius!!", L"3:Vczh is genius!!!", L"4:Vczh is genius!!!!",L""};
	StringReader reader(text);
	nint index=0;

	while(index<sizeof(lines)/sizeof(*lines))
	{
		TEST_ASSERT(reader.IsEnd()==false);
		TEST_ASSERT(reader.ReadLine()==lines[index++]);
	}
	TEST_ASSERT(reader.IsEnd()==true);
}

TEST_CASE(TestStringReaderWithoutCrLf)
{
	const wchar_t text[]=L"1:Vczh is genius!\r\n2:Vczh is genius!!\r\n3:Vczh is genius!!!\r\n4:Vczh is genius!!!!";
	const wchar_t* lines[]={L"1:Vczh is genius!", L"2:Vczh is genius!!", L"3:Vczh is genius!!!", L"4:Vczh is genius!!!!"};
	StringReader reader(text);
	nint index=0;

	while(index<sizeof(lines)/sizeof(*lines))
	{
		TEST_ASSERT(reader.IsEnd()==false);
		TEST_ASSERT(reader.ReadLine()==lines[index++]);
	}
	TEST_ASSERT(reader.IsEnd()==true);
}

/***********************************************************************
流控制器测试(StreamReader/StreamWriter)
***********************************************************************/

TEST_CASE(TestStreamReader)
{
	wchar_t text[]=L"1:Vczh is genius!\r\n2:Vczh is genius!\r\n3:Vczh is genius!\r\n4:Vczh is genius!";
	MemoryWrapperStream stream(text, sizeof(text)-sizeof(*text));
	StreamReader reader(stream);

	TEST_ASSERT(reader.ReadChar()==L'1');
	TEST_ASSERT(reader.ReadString(5)==L":Vczh");
	TEST_ASSERT(reader.ReadLine()==L" is genius!");
	TEST_ASSERT(reader.ReadLine()==L"2:Vczh is genius!");
	TEST_ASSERT(reader.ReadToEnd()==L"3:Vczh is genius!\r\n4:Vczh is genius!");
}

TEST_CASE(TestStreamReaderWithCrLf)
{
	wchar_t text[]=L"1:Vczh is genius!\r\n2:Vczh is genius!!\r\n3:Vczh is genius!!!\r\n4:Vczh is genius!!!!\r\n";
	const wchar_t* lines[]={L"1:Vczh is genius!", L"2:Vczh is genius!!", L"3:Vczh is genius!!!", L"4:Vczh is genius!!!!",L""};
	MemoryWrapperStream stream(text, sizeof(text)-sizeof(*text));
	StreamReader reader(stream);
	nint index=0;

	while(index<sizeof(lines)/sizeof(*lines))
	{
		TEST_ASSERT(reader.IsEnd()==false);
		TEST_ASSERT(reader.ReadLine()==lines[index++]);
	}
	TEST_ASSERT(reader.IsEnd()==true);
}

TEST_CASE(TestStreamReaderWithoutCrLf)
{
	wchar_t text[]=L"1:Vczh is genius!\r\n2:Vczh is genius!!\r\n3:Vczh is genius!!!\r\n4:Vczh is genius!!!!";
	const wchar_t* lines[]={L"1:Vczh is genius!", L"2:Vczh is genius!!", L"3:Vczh is genius!!!", L"4:Vczh is genius!!!!"};
	MemoryWrapperStream stream(text, sizeof(text)-sizeof(*text));
	StreamReader reader(stream);
	nint index=0;

	while(index<sizeof(lines)/sizeof(*lines))
	{
		TEST_ASSERT(reader.IsEnd()==false);
		TEST_ASSERT(reader.ReadLine()==lines[index++]);
	}
	TEST_ASSERT(reader.IsEnd()==true);
}

TEST_CASE(TestStreamWriter)
{
	wchar_t text[BUFFER_SIZE]={0};
	MemoryWrapperStream stream(text, sizeof(text)-sizeof(*text));
	StreamWriter writer(stream);

	writer.WriteChar(L'1');
	writer.WriteChar(L':');
	writer.WriteChar(L'V');
	writer.WriteChar(L'c');
	writer.WriteChar(L'z');
	writer.WriteChar(L'h');
	writer.WriteChar(L' ');
	writer.WriteChar(L'i');
	writer.WriteChar(L's');
	writer.WriteChar(L' ');
	writer.WriteChar(L'g');
	writer.WriteChar(L'e');
	writer.WriteChar(L'n');
	writer.WriteChar(L'i');
	writer.WriteChar(L'u');
	writer.WriteChar(L's');
	writer.WriteChar(L'!');
	writer.WriteString(L"");
	writer.WriteString(L"\r\n2:Vczh is genius!");
	writer.WriteString(WString(L""));
	writer.WriteLine(L"");
	writer.WriteLine(L"3:Vczh is genius!");
	writer.WriteLine(WString(L"4:Vczh is genius!"));

	wchar_t baseline[]=L"1:Vczh is genius!\r\n2:Vczh is genius!\r\n3:Vczh is genius!\r\n4:Vczh is genius!\r\n";
	TEST_ASSERT(wcscmp(text, baseline)==0);
}

/***********************************************************************
编码测试
***********************************************************************/

void TestEncodingInternal(IEncoder& encoder, IDecoder& decoder, BomEncoder::Encoding encoding, bool containsBom)
{
	const wchar_t* text=L"𩰪㦲𦰗𠀼 𣂕𣴑𣱳𦁚 Vczh is genius!@我是天才";
	MemoryStream memoryStream;
	{
		EncoderStream encoderStream(memoryStream, encoder);
		StreamWriter writer(encoderStream);
		writer.WriteString(text);
	}
	memoryStream.SeekFromBegin(0);
	NArray<nuint8_t> buffer;
	buffer.Resize((nint)memoryStream.Size());
	memoryStream.Read(&buffer[0], buffer.Count());
	{
		WString output;
		for(nint i=0; i<buffer.Count(); i++)
		{
			nuint8_t byte=buffer[i];
			output += L"0123456789ABCDEF"[byte/16];	
			output += L"0123456789ABCDEF"[byte%16];
			output += L' ';
		}
		TEST_PRINT(L"\tEncoded: "+output);
	}

	BomEncoder::Encoding resultEncoding;
	bool resultContainsBom;
	TestEncoding(&buffer[0], buffer.Count(), resultEncoding, resultContainsBom);
	TEST_ASSERT(encoding==resultEncoding);
	TEST_ASSERT(containsBom==resultContainsBom);

	if(encoding!=BomEncoder::Mbcs)
	{
		memoryStream.SeekFromBegin(0);
		DecoderStream decoderStream(memoryStream, decoder);
		StreamReader reader(decoderStream);
		WString read=reader.ReadToEnd();
		TEST_ASSERT(read==text);
	}
}

TEST_CASE(TestEncoding)
{
	if (NLocale::SystemDefault().GetName() == L"zh-CN")
	{
		TEST_PRINT(L"<MBCS, NO-BOM>");
		MbcsEncoder encoder;
		MbcsDecoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Mbcs, false);
	}
	{
		TEST_PRINT(L"<UTF8, NO-BOM>");
		Utf8Encoder encoder;
		Utf8Decoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Utf8, false);
	}
	{
		TEST_PRINT(L"<UTF16, NO-BOM>");
		Utf16Encoder encoder;
		Utf16Decoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Utf16, false);
	}
	{
		TEST_PRINT(L"<UTF16_BE, NO-BOM>");
		Utf16BEEncoder encoder;
		Utf16BEDecoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Utf16BE, false);
	}
	if (NLocale::SystemDefault().GetName() == L"zh-CN")
	{
		TEST_PRINT(L"<MBCS, BOM>");
		BomEncoder encoder(BomEncoder::Mbcs);
		BomDecoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Mbcs, false);
	}
	{
		TEST_PRINT(L"<UTF8, BOM>");
		BomEncoder encoder(BomEncoder::Utf8);
		BomDecoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Utf8, true);
	}
	{
		TEST_PRINT(L"<UTF16, BOM>");
		BomEncoder encoder(BomEncoder::Utf16);
		BomDecoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Utf16, true);
	}
	{
		TEST_PRINT(L"<UTF16_BE, BOM>");
		BomEncoder encoder(BomEncoder::Utf16BE);
		BomDecoder decoder;
		TestEncodingInternal(encoder, decoder, BomEncoder::Utf16BE, true);
	}
}

/***********************************************************************
压缩测试
***********************************************************************/

void TestLzwEncodingWithEncoderAndDecoder(const char* input, LzwEncoder& encoder, LzwDecoder& decoder)
{
	MemoryStream stream;
	nint size = strlen(input);
	{
		EncoderStream encoderStream(stream, encoder);
		nint size = strlen(input);
		TEST_ASSERT(encoderStream.Write((void*)input, size) == size);
	}
	stream.SeekFromBegin(0);
	UnitTest::PrintInfo(L"    [" + atow(input) + L"]");
	UnitTest::PrintInfo(L"    " + itow(size) + L" -> " + i64tow(stream.Size()));
	{
		NArray<char> output(size + 1);
		DecoderStream decoderStream(stream, decoder);
		TEST_ASSERT(decoderStream.Read(&output[0], size) == size);
		TEST_ASSERT(decoderStream.Read(&output[0], size) == 0);
		output[size] = 0;
		TEST_ASSERT(strcmp(input, &output[0]) == 0);
	}
}

void TestLzwEncodingDefault(const char* input)
{
	LzwEncoder encoder;
	LzwDecoder decoder;
	TestLzwEncodingWithEncoderAndDecoder(input, encoder, decoder);
}

void TestLzwEncodingPrepared(const char* input)
{
	bool existingBytes[256] = { 0 };
	const char* current = input;
	while (nuint8_t c = (nuint8_t)*current++)
	{
		existingBytes[c] = true;
	}

	LzwEncoder encoder(existingBytes);
	LzwDecoder decoder(existingBytes);
	TestLzwEncodingWithEncoderAndDecoder(input, encoder, decoder);
}

TEST_CASE(TestLzwEncoding)
{
	const char* buffer[] =
	{
		"",
		"0000000000000000000000000000000000000000",
		"Vczh is genius!Vczh is genius!Vczh is genius!",
	};

	for (nint i = 0; i < sizeof(buffer) / sizeof(*buffer); i++)
	{
		TestLzwEncodingDefault(buffer[i]);
		TestLzwEncodingPrepared(buffer[i]);
	}
}

#if defined NDEBUG

namespace lzw_helper
{
	void Copy(NIStream& dst, NIStream& src, NArray<nuint8_t>& buffer, nint totalSize)
	{
		nint BufferSize = buffer.Count();
		while (true)
		{
			nint size = src.Read(&buffer[0], BufferSize);
			if (size == 0)
			{
				break;
			}
			dst.Write(&buffer[0], size);
		}
	}
}
using namespace lzw_helper;

TEST_CASE(TestLzwSpeed)
{
	const nint BufferSize = 33554432;
	NArray<nuint8_t> buffer(BufferSize);
	MemoryStream compressedStream(BufferSize), decompressedStream(BufferSize);
	UnitTest::PrintInfo(L"    Reading UnitTest.pdb ...");
	{
		NFileStream fileStream(GetTestOutputPath() + L"../UnitTest/Release/UnitTest.pdb", NFileStream::ReadOnly);
		Copy(decompressedStream, fileStream, buffer, (nint)fileStream.Size());
	}

	decompressedStream.SeekFromBegin(0);
	nint totalSize = (nint)decompressedStream.Size();
	
	UnitTest::PrintInfo(L"    Compressing UnitTest.pdb ...");
	{
		DateTime begin = DateTime::LocalTime();

		LzwEncoder encoder;
		EncoderStream encoderStream(compressedStream, encoder);
		Copy(encoderStream, decompressedStream, buffer, totalSize);

		DateTime end = DateTime::LocalTime();

		double time = (end.totalMilliseconds - begin.totalMilliseconds) / 1000.0;
		UnitTest::PrintInfo(L"    Time elasped: " + ftow(time) + L" seconds");
		UnitTest::PrintInfo(L"    Performance: " + ftow(totalSize / time / (1 << 20)) + L" MB/s");
	}

	compressedStream.SeekFromBegin(0);
	UnitTest::PrintInfo(L"    " + i64tow(totalSize) + L" -> " + i64tow(compressedStream.Size()));

	UnitTest::PrintInfo(L"    Decompressing UnitTest.pdb ...");
	{
		DateTime begin = DateTime::LocalTime();

		LzwDecoder decoder;
		DecoderStream decoderStream(compressedStream, decoder);
		Copy(decompressedStream, decoderStream, buffer, totalSize);

		DateTime end = DateTime::LocalTime();
		double time = (end.totalMilliseconds - begin.totalMilliseconds) / 1000.0;
		UnitTest::PrintInfo(L"    Time elasped: " + ftow(time) + L" seconds");
		UnitTest::PrintInfo(L"    Performance: " + ftow(totalSize / time / (1 << 20)) + L" MB/s");
	}
}
#endif