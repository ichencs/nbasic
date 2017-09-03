/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::BroadcastStream

Interfaces:
	BroadcastStream					���㲥��
***********************************************************************/

#ifndef VCZH_STREAM_BROADCASTSTREAM
#define VCZH_STREAM_BROADCASTSTREAM

#include "Interfaces.h"
#include "../Collections/NList.h"

namespace vl
{
	namespace stream
	{
		/// <summary>A writable stream that copy written content to multiple target streams.</summary>
		class BroadcastStream : public Object, public virtual NIStream
		{
			typedef NList<NIStream*>		StreamList;
		protected:
			bool					closed;
			pos_t					position;
			StreamList				streams;
		public:
			/// <summary>Create a strema.</summary>
			BroadcastStream();
			~BroadcastStream();

			/// <summary>Get the list of target streams. You can add streams to this list, or remove streams from this list.</summary>
			/// <returns>The list of target streams.</returns>
			StreamList&				Targets();
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
}

#endif