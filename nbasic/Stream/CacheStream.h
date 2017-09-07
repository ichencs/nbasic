/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::CacheStream

Interfaces:
	CacheStream						£º»º³åÁ÷
***********************************************************************/

#ifndef VCZH_STREAM_CACHESTREAM
#define VCZH_STREAM_CACHESTREAM

#include "Interfaces.h"

 
	namespace stream
	{
		/// <summary>
		/// A cache stream. Features (readable, writable, seekable, peekable) are enabled according to the target stream.
		/// When you read from the cache strema, it will read a specified size of content from the target stream first and cache, reducing the numbers of operations on the target stream.
		/// When you write to the cache strema, it will save them to a buffer, and write to the target stream until the buffer reaches a specified size, reducing the numbers of operations on the target stream.
		/// </summary>
		class CacheStream : public Object, public virtual NIStream
		{
		protected:
			NIStream*				target;
			nint					block;
			pos_t					start;
			pos_t					position;

			char*					buffer;
			nint					dirtyStart;
			nint					dirtyLength;
			nint					availableLength;
			pos_t					operatedSize;

			void					Flush();
			void					Load(pos_t _position);
			nint					InternalRead(void* _buffer, nint _size);
			nint					InternalWrite(void* _buffer, nint _size);
		public:
			/// <summary>Create a cache stream using a target stream.</summary>
			/// <param name="_target">The target stream.</param>
			/// <param name="_block">Size of the cache.</param>
			CacheStream(NIStream& _target, nint _block=65536);
			~CacheStream();

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
	}

#endif