#include "CompressionStream.h"


/***********************************************************************
LzwBase
***********************************************************************/

		void LzwBase::UpdateIndexBits()
		{
			if (nextIndex >=2 && (nextIndex & (nextIndex - 1)) == 0)
			{
				indexBits++;
			}
		}

		NCode* LzwBase::CreateCode(NCode* prefix, nuint8_t byte)
		{
			if (nextIndex < MaxDictionarySize)
			{
				NCode* code = codeAllocator.Create();
				code->byte = byte;
				code->code = nextIndex;
				code->parent = prefix;
				code->size = prefix->size + 1;
				prefix->children.Set(byte, code, mapAllocator);
				nextIndex++;

				return code;
			}
			else
			{
				return 0;
			}
		}

		LzwBase::LzwBase()
			:codeAllocator(65536)
			, mapAllocator(1048576)
		{
			root = codeAllocator.Create();

			for (nint i = 0; i < 256; i++)
			{
				UpdateIndexBits();
				CreateCode(root, (nuint8_t)i);
			}
		}

		LzwBase::LzwBase(bool (&existingBytes)[256])
		{
			root = codeAllocator.Create();
			for (nint i = 0; i < 256; i++)
			{
				if (existingBytes[i])
				{
					UpdateIndexBits();
					CreateCode(root, (nuint8_t)i);
				}
			}

			if (indexBits < 8)
			{
				eofIndex = nextIndex++;
			}
		}

		LzwBase::~LzwBase()
		{
		}

/***********************************************************************
LzwEncoder
***********************************************************************/

		void LzwEncoder::Flush()
		{
			nint written = 0;
			nint bufferUsedSize = bufferUsedBits / 8;
			if (bufferUsedBits % 8 != 0)
			{
				bufferUsedSize++;
			}
			while (written < bufferUsedSize)
			{
				nint size = stream->Write(buffer + written, bufferUsedSize - written);
				CHECK_ERROR(size != 0, L"LzwEncoder::Flush()#Failed to flush the lzw buffer.");
				written += size;
			}
			bufferUsedBits = 0;
		}

		nuint8_t highMarks[9] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
		nuint8_t lowMarks[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

		void LzwEncoder::WriteNumber(nint number, nint bitSize)
		{
			nint bitStart = 0;
			nint bitStep = 8 - bufferUsedBits % 8;
			if (bitStep > bitSize)
			{
				bitStep = bitSize;
			}
			while (bitStart < bitSize)
			{
				if(bufferUsedBits == BufferSize * 8)
				{
					Flush();
				}

				nint writeStart = bufferUsedBits % 8;
				nint byteIndex = bufferUsedBits / 8;
				nuint8_t byte = buffer[byteIndex];
				byte &= highMarks[writeStart];

				nuint8_t content = (nuint8_t)((number >> bitStart)&lowMarks[bitStep]) << (8 - writeStart - bitStep);
				byte |= content;

				buffer[byteIndex] = byte;
				bufferUsedBits += bitStep;

				bitStart += bitStep;
				nint remain = bitSize - bitStart;
				bitStep = remain < 8 ? remain : 8;
			}
		}

		LzwEncoder::LzwEncoder()
		{
			prefix = root;
		}

		LzwEncoder::LzwEncoder(bool (&existingBytes)[256])
			:LzwBase(existingBytes)
		{
			prefix = root;
		}

		LzwEncoder::~LzwEncoder()
		{
		}

		void LzwEncoder::Setup(NIStream* _stream)
		{
			stream = _stream;
		}

		void LzwEncoder::Close()
		{
			if (prefix != root)
			{
				WriteNumber(prefix->code, indexBits);
				prefix = root;
			}

			nint remain = 8 - bufferUsedBits % 8;
			if (remain != 8 && remain >= indexBits)
			{
				CHECK_ERROR(eofIndex != -1, L"LzwEncoder::Close()#Internal error.");
				WriteNumber(eofIndex, indexBits);
			}
			Flush();
		}

		nint LzwEncoder::Write(void* _buffer, nint _size)
		{
			nuint8_t* bytes = (nuint8_t*)_buffer;
			for (nint i = 0; i < _size; i++)
			{
				nuint8_t byte = bytes[i];
				NCode* next = prefix->children.Get(byte);
				if (next)
				{
					prefix = next;
				}
				else
				{
					WriteNumber(prefix->code, indexBits);

					if (nextIndex < MaxDictionarySize)
					{
						UpdateIndexBits();
						CreateCode(prefix, byte);
					}
					prefix = root->children.Get(byte);
				}
			}
			return _size;
		}

/***********************************************************************
LzwDecoder
***********************************************************************/

		bool LzwDecoder::ReadNumber(nint& number, nint bitSize)
		{
			number = 0;
			if (inputBufferSize == -1)
			{
				return false;
			}

			nint remainBits = inputBufferSize * 8 - inputBufferUsedBits;
			nint writtenBits = 0;
			nint bitStep = 8 - inputBufferUsedBits % 8;
			if (bitStep > bitSize)
			{
				bitStep = bitSize;
			}
			while (writtenBits < bitSize)
			{
				if (remainBits == 0)
				{
					inputBufferSize = stream->Read(inputBuffer, BufferSize);
					if (inputBufferSize == 0)
					{
						inputBufferSize = -1;
						return false;
					}
					remainBits = inputBufferSize * 8;
					inputBufferUsedBits = 0;
				}

				nuint8_t byte = inputBuffer[inputBufferUsedBits / 8];
				byte >>= (8 - inputBufferUsedBits % 8 - bitStep);
				byte &= lowMarks[bitStep];
				number |= byte << writtenBits;

				inputBufferUsedBits += bitStep;
				remainBits -= bitStep;
				writtenBits += bitStep;
				nint remain = bitSize - writtenBits;
				bitStep = remain < 8 ? remain : 8;
			}

			return true;
		}

		void LzwDecoder::PrepareOutputBuffer(nint size)
		{
			if (outputBuffer.Count() < size)
			{
				outputBuffer.Resize(size);
			}
			outputBufferSize = size;
		}

		void LzwDecoder::ExpandCodeToOutputBuffer(NCode* code)
		{
			nuint8_t* outputByte = &outputBuffer[0] + code->size;
			NCode* current = code;
			while (current != root)
			{
				*(--outputByte) = current->byte;
				current = current->parent;
			}
			outputBufferUsedBytes = 0;
		}

		LzwDecoder::LzwDecoder()
		{
			for (nint i = 0; i < 256; i++)
			{
				dictionary.Add(root->children.Get((nuint8_t)i));
			}
		}

		LzwDecoder::LzwDecoder(bool (&existingBytes)[256])
			:LzwBase(existingBytes)
		{
			for (nint i = 0; i < 256; i++)
			{
				if (existingBytes[i])
				{
					dictionary.Add(root->children.Get((nuint8_t)i));
				}
			}
			if (eofIndex != -1)
			{
				dictionary.Add(0);
			}
		}

		LzwDecoder::~LzwDecoder()
		{
		}

		void LzwDecoder::Setup(NIStream* _stream)
		{
			stream = _stream;
		}

		void LzwDecoder::Close()
		{
		}

		nint LzwDecoder::Read(void* _buffer, nint _size)
		{
			nint written = 0;
			nuint8_t* bytes = (nuint8_t*)_buffer;
			while (written < _size)
			{
				nint expect = _size - written;
				nint remain = outputBufferSize - outputBufferUsedBytes;
				if (remain == 0)
				{
					nint index = 0;
					if (!ReadNumber(index, indexBits) || index == eofIndex)
					{
						break;
					}

					NCode* prefix = 0;
					if (index == dictionary.Count())
					{
						prefix = lastCode;
						PrepareOutputBuffer(prefix->size + 1);
						ExpandCodeToOutputBuffer(prefix);
						outputBuffer[outputBufferSize - 1] = outputBuffer[0];
					}
					else
					{
						prefix = dictionary[index];
						PrepareOutputBuffer(prefix->size);
						ExpandCodeToOutputBuffer(prefix);
					}
					
					if (nextIndex < MaxDictionarySize)
					{
						if (lastCode)
						{
 							dictionary.Add(CreateCode(lastCode, outputBuffer[0]));
						}
						UpdateIndexBits();
					}
					lastCode = dictionary[index];
				}
				else
				{
					if (remain > expect)
					{
						remain = expect;
					}
					memcpy(bytes + written, &outputBuffer[outputBufferUsedBytes], remain);

					outputBufferUsedBytes += remain;
					written += remain;
				}
			}
			return written;
		}
