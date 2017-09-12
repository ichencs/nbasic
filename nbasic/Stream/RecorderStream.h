/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::RecorderStream

Interfaces:
	RecorderStream					£º±¸·ÝÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_RECORDERSTREAM
#define VCZH_STREAM_RECORDERSTREAM

#include "Interfaces.h"

/// <summary>A readable stream that, read from an stream, and write everything that is read to another stream.</summary>
class RecorderStream : public Object, public virtual NIStream
{
	protected:
		NIStream*				in;
		NIStream*				out;
	public:
		/// <summary>Create a stream.</summary>
		/// <param name="_in">The stream to read.</param>
		/// <param name="_out">The stream to write what is read from "_in".</param>
		RecorderStream(NIStream& _in, NIStream& _out);
		~RecorderStream();
		
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