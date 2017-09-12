// #include "stdafx.h"
#include "NFileStream.h"



/***********************************************************************
FileStream
***********************************************************************/

NFileStream::NFileStream(const WString& fileName, AccessRight _accessRight)
	: accessRight(_accessRight)
{
	const wchar_t* mode = L"rb";
	
	switch (accessRight)
	{
		case ReadOnly:
			mode = L"rb";
			break;
			
		case WriteOnly:
			mode = L"wb";
			break;
			
		case ReadWrite:
			mode = L"w+b";
			break;
	}
	
	if (_wfopen_s(&file, fileName.Buffer(), mode) != 0)
	{
		file = NULL;
	}
}

NFileStream::~NFileStream()
{
	Close();
}

bool NFileStream::CanRead()const
{
	return file != NULL && (accessRight == ReadOnly || accessRight == ReadWrite);
}

bool NFileStream::CanWrite()const
{
	return file != NULL && (accessRight == WriteOnly || accessRight == ReadWrite);
}

bool NFileStream::CanSeek()const
{
	return file != NULL;
}

bool NFileStream::CanPeek()const
{
	return file != NULL && (accessRight == ReadOnly || accessRight == ReadWrite);
}

bool NFileStream::IsLimited()const
{
	return file != NULL && accessRight == ReadOnly;
}

bool NFileStream::IsAvailable()const
{
	return file != NULL;
}

void NFileStream::Close()
{
	if (file != NULL)
	{
		fclose(file);
		file = NULL;
	}
}

pos_t NFileStream::Position()const
{
	if (file != NULL)
	{
		fpos_t position = 0;
		
		if (fgetpos(file, &position) == 0)
		{
			return position;
		}
	}
	
	return -1;
}

pos_t NFileStream::Size()const
{
	if (file != NULL)
	{
		fpos_t position = 0;
		
		if (fgetpos(file, &position) == 0)
		{
			if (fseek(file, 0, SEEK_END) == 0)
			{
				pos_t size = Position();
				
				if (fsetpos(file, &position) == 0)
				{
					return size;
				}
			}
		}
	}
	
	return -1;
}

void NFileStream::Seek(pos_t _size)
{
	if (Position() + _size > Size())
	{
		_fseeki64(file, 0, SEEK_END);
	}
	else
		if (Position() + _size < 0)
		{
			_fseeki64(file, 0, SEEK_SET);
		}
		else
		{
			_fseeki64(file, _size, SEEK_CUR);
		}
}

void NFileStream::SeekFromBegin(pos_t _size)
{
	if (_size > Size())
	{
		_fseeki64(file, 0, SEEK_END);
	}
	else
		if (_size < 0)
		{
			_fseeki64(file, 0, SEEK_SET);
		}
		else
		{
			_fseeki64(file, _size, SEEK_SET);
		}
}

void NFileStream::SeekFromEnd(pos_t _size)
{
	if (_size < 0)
	{
		_fseeki64(file, 0, SEEK_END);
	}
	else
		if (_size > Size())
		{
			_fseeki64(file, 0, SEEK_SET);
		}
		else
		{
			_fseeki64(file, -_size, SEEK_END);
		}
}

nint NFileStream::Read(void* _buffer, nint _size)
{
	CHECK_ERROR(file != NULL, L"FileStream::Read(pos_t)#Stream is closed, cannot perform this operation.");
	CHECK_ERROR(_size >= 0, L"FileStream::Read(void*, nint)#Argument size cannot be negative.");
	return fread(_buffer, 1, _size, file);
}

nint NFileStream::Write(void* _buffer, nint _size)
{
	CHECK_ERROR(file != NULL, L"FileStream::Write(pos_t)#Stream is closed, cannot perform this operation.");
	CHECK_ERROR(_size >= 0, L"FileStream::Write(void*, nint)#Argument size cannot be negative.");
	return fwrite(_buffer, 1, _size, file);
}

nint NFileStream::Peek(void* _buffer, nint _size)
{
	CHECK_ERROR(file != NULL, L"FileStream::Peek(pos_t)#Stream is closed, cannot perform this operation.");
	CHECK_ERROR(_size >= 0, L"FileStream::Peek(void*, nint)#Argument size cannot be negative.");
	fpos_t position = 0;
	
	if (fgetpos(file, &position) == 0)
	{
		size_t count = fread(_buffer, 1, _size, file);
		
		if (fsetpos(file, &position) == 0)
		{
			return count;
		}
	}
	
	return -1;
}
