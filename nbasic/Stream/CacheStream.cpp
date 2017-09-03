// #include "..\stdafx.h"
#include <string.h>
#include "CacheStream.h"

namespace vl
{
	namespace stream
	{
/***********************************************************************
CacheStream
***********************************************************************/

		void CacheStream::Flush()
		{
			if(dirtyLength>0)
			{
				if(target->Position()!=start+dirtyStart)
				{
					target->SeekFromBegin(start+dirtyStart);
				}
				target->Write(buffer+dirtyStart, dirtyLength);
			}
			dirtyStart=0;
			dirtyLength=0;
			availableLength=0;
		}

		void CacheStream::Load(pos_t _position)
		{
			if(target->Position()!=_position)
			{
				target->SeekFromBegin(_position);
			}
			start=_position;
			if(target->CanRead())
			{
				availableLength=target->Read(buffer, block);
			}
		}

		nint CacheStream::InternalRead(void* _buffer, nint _size)
		{
			nint readed=0;
			if(position>=start && position<start+availableLength)
			{
				nint bufferMax=(nint)(start+availableLength-position);
				nint min=bufferMax<_size?bufferMax:_size;
				memcpy(_buffer, buffer+(position-start), min);
				readed+=min;
				_buffer=(char*)_buffer+min;
			}

			if(_size>readed)
			{
				Flush();
				if(_size-readed>=block)
				{
					if(CanSeek())
					{
						target->SeekFromBegin(position+readed);
					}
					nint additional=target->Read(_buffer, _size-readed);
					if(additional!=-1)
					{
						readed+=additional;
					}
				}
				else
				{
					Load(position+readed);
					nint remain=_size-readed;
					nint min=availableLength<remain?availableLength:remain;
					memcpy(_buffer, buffer, min);
					readed+=min;
				}
			}
			return readed;
		}

		nint CacheStream::InternalWrite(void* _buffer, nint _size)
		{
			nint written=0;
			if(position>=start && position<start+block)
			{
				nint bufferMax=(nint)(start+block-position);
				nint writeLength=bufferMax<_size?bufferMax:_size;
				nint writeStart=(nint)(position-start);

				memcpy(buffer+writeStart, _buffer, writeLength);
				written+=writeLength;
				_buffer=(char*)_buffer+writeLength;

				if(dirtyLength==0)
				{
					dirtyStart=writeStart;
					dirtyLength=writeLength;
				}
				else
				{
					dirtyLength=writeStart+writeLength-dirtyStart;
				}

				nint availableOffset=writeStart+writeLength-availableLength;
				if(availableOffset>0)
				{
					availableLength+=availableOffset;
				}
			}
			
			if(_size>written)
			{
				Flush();
				if(_size-written>=block)
				{
					if(CanSeek())
					{
						target->SeekFromBegin(position+written);
					}
					nint additional=target->Write(_buffer, _size-written);
					if(additional!=-1)
					{
						written+=additional;
					}
				}
				else
				{
					Load(position+written);
					dirtyLength=_size-written;
					memcpy(buffer, _buffer, dirtyLength);
					written+=dirtyLength;
				}
			}
			return written;
		}

		CacheStream::CacheStream(NIStream& _target, nint _block)
			:target(&_target)
			,block(_block)
			,start(0)
			,position(0)
			,dirtyStart(0)
			,dirtyLength(0)
			,availableLength(0)
			,operatedSize(0)
		{
			if(block<=0)
			{
				block=65536;
			}
			buffer=new char[block];
		}

		CacheStream::~CacheStream()
		{
			Close();
		}

		bool CacheStream::CanRead()const
		{
			return target!=0 && target->CanRead();
		}

		bool CacheStream::CanWrite()const
		{
			return target!=0 && target->CanWrite();
		}

		bool CacheStream::CanSeek()const
		{
			return target!=0 && target->CanSeek();
		}

		bool CacheStream::CanPeek()const
		{
			return target!=0 && target->CanPeek();
		}

		bool CacheStream::IsLimited()const
		{
			return target!=0 && target->IsLimited();
		}

		bool CacheStream::IsAvailable()const
		{
			return target!=0 && target->IsAvailable();
		}

		void CacheStream::Close()
		{
			Flush();
			target=0;
			delete[] buffer;
			buffer=0;
			position=-1;
			dirtyStart=0;
			dirtyLength=0;
			availableLength=0;
			operatedSize=-1;
		}

		pos_t CacheStream::Position()const
		{
			return position;
		}

		pos_t CacheStream::Size()const
		{
			if(target!=0)
			{
				if(IsLimited())
				{
					return target->Size();
				}
				else
				{
					return operatedSize;
				}
			}
			else
			{
				return -1;
			}
		}

		void CacheStream::Seek(pos_t _size)
		{
			SeekFromBegin(position+_size);
		}

		void CacheStream::SeekFromBegin(pos_t _size)
		{
			if(CanSeek())
			{
				if(_size<0)
				{
					position=0;
				}
				else if(_size>Size())
				{
					position=Size();
				}
				else
				{
					position=_size;
				}
			}
		}

		void CacheStream::SeekFromEnd(pos_t _size)
		{
			SeekFromBegin(Size()-_size);
		}

		nint CacheStream::Read(void* _buffer, nint _size)
		{
			CHECK_ERROR(CanRead(), L"CacheStream::Read(void*, nint)#Stream is closed or operation not supported.");
			CHECK_ERROR(_size>=0, L"CacheStream::Read(void*, nint)#Argument size cannot be negative.");

			_size=InternalRead(_buffer, _size);
			position+=_size;
			if(operatedSize<position)
			{
				operatedSize=position;
			}
			return _size;
		}

		nint CacheStream::Write(void* _buffer, nint _size)
		{
			CHECK_ERROR(CanWrite(), L"CacheStream::Write(void*, nint)#Stream is closed or operation not supported.");
			CHECK_ERROR(_size>=0, L"CacheStream::Read(void*, nint)#Argument size cannot be negative.");

			if(IsLimited())
			{
				pos_t size=Size();
				if(size!=-1)
				{
					nint remain=(nint)(size-(position+_size));
					if(remain<0)
					{
						_size-=remain;
					}
				}
			}

			_size=InternalWrite(_buffer, _size);
			position+=_size;
			if(operatedSize<position)
			{
				operatedSize=position;
			}
			return _size;
		}

		nint CacheStream::Peek(void* _buffer, nint _size)
		{
			CHECK_ERROR(CanPeek(), L"CacheStream::Peek(void*, nint)#Stream is closed or operation not supported.");
			CHECK_ERROR(_size>=0, L"CacheStream::Read(void*, nint)#Argument size cannot be negative.");

			return InternalRead(_buffer, _size);
		}
	}
}