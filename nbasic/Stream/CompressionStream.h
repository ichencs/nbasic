/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::CharFormat

Classes:
***********************************************************************/

#ifndef VCZH_STREAM_COMPRESSIONSTREAM
#define VCZH_STREAM_COMPRESSIONSTREAM

#include "Interfaces.h"


/***********************************************************************
Compression
***********************************************************************/

	
			static const nint						BufferSize = 1024;
			static const nint						MaxDictionarySize = 1 << 24;

			struct NCode
			{
				NCode() :byte(0), code(-1), parent(0), size(0)
				{
				}
				typedef PushOnlyAllocator<NCode>			CodeAllocator;
				typedef ByteObjectMap<NCode>::Allocator		MapAllocator;

				nuint8_t							byte;
				nint								code;
				NCode*								parent;
				nint								size;
				ByteObjectMap<NCode>	children;
			};
		
		class LzwBase : public Object
		{
		protected:
			NCode::CodeAllocator				codeAllocator;
			NCode::MapAllocator					mapAllocator;
			NCode*								root;
			nint									eofIndex/* = -1*/;
			nint									nextIndex /*= 0*/;
			nint									indexBits /*= 1*/;

			void									UpdateIndexBits();
			NCode*								CreateCode(NCode* parent, nuint8_t byte);

			LzwBase();
			LzwBase(bool (&existingBytes)[256]);
			~LzwBase();
		};

		/// <summary>An encoder to compress using Lzw algorithm.</summary>
		class LzwEncoder : public LzwBase, public IEncoder
		{
		protected:
			NIStream*								stream/* = 0*/;

			nuint8_t								buffer[BufferSize];
			nint									bufferUsedBits/* = 0*/;
			NCode*								prefix;

			void									Flush();
			void									WriteNumber(nint number, nint bitSize);
		public:
			/// <summary>Create an encoder.</summary>
			LzwEncoder();
			/// <summary>Create an encoder and tell it which byte will never appear in the data before compression.</summary>
			/// <param name="existingBytes">An array to tell the encoder which byte will never appear in the data before compression.</param>
			LzwEncoder(bool (&existingBytes)[256]);
			~LzwEncoder();

			void									Setup(NIStream* _stream)override;
			void									Close()override;
			nint									Write(void* _buffer, nint _size)override;
		};
		
		/// <summary>An decoder to decompress using Lzw algorithm.</summary>
		class LzwDecoder :public LzwBase, public IDecoder
		{
		protected:
			NIStream*								stream /*= 0*/;
			NList<NCode*>			dictionary;
			NCode*								lastCode /*= 0*/;

			nuint8_t								inputBuffer[BufferSize];
			nint									inputBufferSize/* = 0*/;
			nint									inputBufferUsedBits/* = 0*/;

			NArray<nuint8_t>			outputBuffer;
			nint									outputBufferSize/* = 0*/;
			nint									outputBufferUsedBytes /*= 0*/;

			bool									ReadNumber(nint& number, nint bitSize);
			void									PrepareOutputBuffer(nint size);
			void									ExpandCodeToOutputBuffer(NCode* code);
		public:
			/// <summary>Create an decoder.</summary>
			LzwDecoder();
			/// <summary>Create an decoder and tell it which byte will never appear in the data before compression.</summary>
			/// <param name="existingBytes">An array to tell the encoder which byte will never appear in the data before compression.</param>
			LzwDecoder(bool (&existingBytes)[256]);
			~LzwDecoder();

			void									Setup(NIStream* _stream)override;
			void									Close()override;
			nint									Read(void* _buffer, nint _size)override;
		};

#endif