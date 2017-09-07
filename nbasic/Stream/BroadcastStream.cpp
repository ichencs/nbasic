// #include "..\stdafx.h"
#include <string.h>
#include "BroadcastStream.h"

 
	namespace stream
	{
/***********************************************************************
BroadcastStream
***********************************************************************/

		BroadcastStream::BroadcastStream()
			:closed(false)
			,position(0)
		{
		}

		BroadcastStream::~BroadcastStream()
		{
		}

		BroadcastStream::StreamList& BroadcastStream::Targets()
		{
			return streams;
		}

		bool BroadcastStream::CanRead()const
		{
			return false;
		}

		bool BroadcastStream::CanWrite()const
		{
			return !closed;
		}

		bool BroadcastStream::CanSeek()const
		{
			return false;
		}

		bool BroadcastStream::CanPeek()const
		{
			return false;
		}

		bool BroadcastStream::IsLimited()const
		{
			return false;
		}

		bool BroadcastStream::IsAvailable()const
		{
			return !closed;
		}

		void BroadcastStream::Close()
		{
			closed=true;
			position=-1;
		}

		pos_t BroadcastStream::Position()const
		{
			return position;
		}

		pos_t BroadcastStream::Size()const
		{
			return position;
		}

		void BroadcastStream::Seek(pos_t _size)
		{
			CHECK_FAIL(L"BroadcastStream::Seek(pos_t)#Operation not supported.");
		}

		void BroadcastStream::SeekFromBegin(pos_t _size)
		{
			CHECK_FAIL(L"BroadcastStream::SeekFromBegin(pos_t)#Operation not supported.");
		}

		void BroadcastStream::SeekFromEnd(pos_t _size)
		{
			CHECK_FAIL(L"BroadcastStream::SeekFromEnd(pos_t)#Operation not supported.");
		}

		nint BroadcastStream::Read(void* _buffer, nint _size)
		{
			CHECK_FAIL(L"BroadcastStream::Read(void*, nint)#Operation not supported.");
		}

		nint BroadcastStream::Write(void* _buffer, nint _size)
		{
			for(nint i=0;i<streams.Count();i++)
			{
				streams[i]->Write(_buffer, _size);
			}
			position+=_size;
			return _size;
		}

		nint BroadcastStream::Peek(void* _buffer, nint _size)
		{
			CHECK_FAIL(L"BroadcastStream::Peek(void*, nint)#Operation not supported.");
		}
	}
