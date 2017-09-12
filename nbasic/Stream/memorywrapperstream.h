/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::MemoryWrapperStream

Interfaces:
	MemoryWrapperStream				：内存代理流
***********************************************************************/

#ifndef VCZH_STREAM_MEMORYWRAPPERSTREAM
#define VCZH_STREAM_MEMORYWRAPPERSTREAM

#include "Interfaces.h"

/// <summary>A readable, writable, seekable and peekable stream that operates a buffer.</summary>
class MemoryWrapperStream : public Object, public virtual NIStream
{
	protected:
		char*					buffer;
		nint						size;
		nint						position;
	public:
		/// <summary>Create a stream.</summary>
		/// <param name="_buffer">The buffer to operate.</param>
		/// <param name="_size">Size of the buffer.</param>
		MemoryWrapperStream(void* _buffer, nint _size);
		~MemoryWrapperStream();
		
		bool					CanRead()const;
		bool					CanWrite()const;
		bool					CanSeek()const;
		bool					CanPeek()const;
		bool					IsLimited()const;
		bool					IsAvailable()const;
		void					Close();
		pos_t					Position()const;
		pos_t					Size()const;
		void					Seek(pos_t _size);
		void					SeekFromBegin(pos_t _size);
		void					SeekFromEnd(pos_t _size);
		nint					Read(void* _buffer, nint _size);
		nint					Write(void* _buffer, nint _size);
		nint					Peek(void* _buffer, nint _size);
};

#endif