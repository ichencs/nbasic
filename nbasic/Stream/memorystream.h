/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::MemoryStream

Interfaces:
	MemoryStream					£ºÄÚ´æÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_MEMORYSTREAM
#define VCZH_STREAM_MEMORYSTREAM

#include "Interfaces.h"

		/// <summary>A readable, writable, seekable and peekable stream that stores everything in memory.</summary>
		class MemoryStream : public Object, public virtual NIStream
		{
		protected:
			nint					block;
			char*					buffer;
			nint					size;
			nint					position;
			nint					capacity;

			void					PrepareSpace(nint totalSpace);
		public:
			/// <summary>Create a stream.</summary>
			/// <param name="_block">Size for each allocation. The stream will only allocate new memory of size "_block" when it reaches the end and has to extend.</param>
			MemoryStream(nint _block=65536);
			~MemoryStream();

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
			void*					GetInternalBuffer();
		};

#endif