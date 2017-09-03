// #include "stdafx.h"
#include <string.h>
#include "MemoryStream.h"

/***********************************************************************
MemoryStream
***********************************************************************/

		void MemoryStream::PrepareSpace(nint totalSpace)
		{
			if(totalSpace>capacity)
			{
				totalSpace=(totalSpace/block+1)*block;
				char* newBuffer=new char[totalSpace];
				if(buffer)
				{
					memcpy(newBuffer, buffer, size);
					delete[] buffer;
				}
				buffer=newBuffer;
				capacity=totalSpace;
			}
		}

		MemoryStream::MemoryStream(nint _block)
			:block(_block)
			,buffer(0)
			,size(0)
			,position(0)
			,capacity(0)
		{
			if(block<=0)
			{
				block=65536;
			}
		}

		MemoryStream::~MemoryStream()
		{
			Close();
		}

		bool MemoryStream::CanRead()const
		{
			return block!=0;
		}

		bool MemoryStream::CanWrite()const
		{
			return block!=0;
		}

		bool MemoryStream::CanSeek()const
		{
			return block!=0;
		}

		bool MemoryStream::CanPeek()const
		{
			return block!=0;
		}

		bool MemoryStream::IsLimited()const
		{
			return false;
		}

		bool MemoryStream::IsAvailable()const
		{
			return block!=0;
		}

		void MemoryStream::Close()
		{
			if(buffer)
			{
				delete[] buffer;
			}
			block=0;
			buffer=0;
			size=-1;
			position=-1;
			capacity=0;
		}

		pos_t MemoryStream::Position()const
		{
			return position;
		}

		pos_t MemoryStream::Size()const
		{
			return size;
		}

		void MemoryStream::Seek(pos_t _size)
		{
			SeekFromBegin(position+_size);
		}

		void MemoryStream::SeekFromBegin(pos_t _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::SeekFromBegin(pos_t)#Stream is closed, cannot perform this operation.");
			nint expected=(nint)_size;
			if(expected<0)
			{
				position=0;
			}
			else if(expected>=size)
			{
				position=size;
			}
			else
			{
				position=expected;
			}
		}

		void MemoryStream::SeekFromEnd(pos_t _size)
		{
			SeekFromBegin(size-_size);
		}

		nint MemoryStream::Read(void* _buffer, nint _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::Read(pos_t)#Stream is closed, cannot perform this operation.");
			CHECK_ERROR(_size>=0, L"MemoryStream::Read(void*, nint)#Argument size cannot be negative.");
			nint max=size-position;
			if(_size>max)
			{
				_size=max;
			}
			memmove(_buffer, buffer+position, _size);
			position+=_size;
			return _size;
		}

		nint MemoryStream::Write(void* _buffer, nint _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::Write(pos_t)#Stream is closed, cannot perform this operation.");
			CHECK_ERROR(_size>=0, L"MemoryStream::Write(void*, nint)#Argument size cannot be negative.");
			PrepareSpace(size+_size);
			memmove(buffer+position, _buffer, _size);
			position+=_size;
			if(size<position)
			{
				size=position;
			}
			return _size;
		}

		nint MemoryStream::Peek(void* _buffer, nint _size)
		{
			CHECK_ERROR(block!=0, L"MemoryStream::Peek(pos_t)#Stream is closed, cannot perform this operation.");
			CHECK_ERROR(_size>=0, L"MemoryStream::Peek(void*, nint)#Argument size cannot be negative.");
			nint max=size-position;
			if(_size>max)
			{
				_size=max;
			}
			memmove(_buffer, buffer+position, _size);
			return _size;
		}

		void* MemoryStream::GetInternalBuffer()
		{
			return buffer;
		}