// #include "..\stdafx.h"
#include "CharFormat.h"
#include <windows.h>



/***********************************************************************
CharEncoder
***********************************************************************/

		CharEncoder::CharEncoder()
			:stream(0)
			,cacheSize(0)
		{
		}

		void CharEncoder::Setup(NIStream* _stream)
		{
			stream=_stream;
		}

		void CharEncoder::Close()
		{
		}

		nint CharEncoder::Write(void* _buffer, nint _size)
		{
			const nint all=cacheSize+_size;
			const nint chars=all/sizeof(wchar_t);
			const nint bytes=chars*sizeof(wchar_t);
			wchar_t* unicode= NULL;
			bool needToFree=false;
			nint result=0;

			if(chars)
			{
				if(cacheSize>0)
				{
					unicode=new wchar_t[chars];
					memcpy(unicode, cacheBuffer, cacheSize);
					memcpy(((nuint8_t*)unicode)+cacheSize, _buffer, bytes-cacheSize);
					needToFree=true;
				}
				else
				{
					unicode=(wchar_t*)_buffer;
				}
				result=WriteString(unicode, chars)*sizeof(wchar_t)-cacheSize;
				cacheSize=0;
			}

			if(needToFree)
			{
				delete[] unicode;
			}
			if(all-bytes>0)
			{
				cacheSize=all-bytes;
				memcpy(cacheBuffer, (nuint8_t*)_buffer+_size-cacheSize, cacheSize);
				result+=cacheSize;
			}
			return result;
		}

/***********************************************************************
CharDecoder
***********************************************************************/

		CharDecoder::CharDecoder()
			:stream(0)
			,cacheSize(0)
		{
		}

		void CharDecoder::Setup(NIStream* _stream)
		{
			stream=_stream;
		}

		void CharDecoder::Close()
		{
		}

		nint CharDecoder::Read(void* _buffer, nint _size)
		{
			nuint8_t* unicode=(nuint8_t*)_buffer;
			nint result=0;
			{
				nint index=0;
				while(cacheSize>0 && _size>0)
				{
					*unicode++=cacheBuffer[index]++;
					cacheSize--;
					_size--;
					result++;
				}
			}

			const nint chars=_size/sizeof(wchar_t);
			nint bytes=ReadString((wchar_t*)unicode, chars)*sizeof(wchar_t);
			result+=bytes;
			_size-=bytes;
			unicode+=bytes;

			if(_size>0)
			{
				wchar_t c;
				if(ReadString(&c, 1)==1)
				{
					cacheSize=sizeof(wchar_t)-_size;
					memcpy(unicode, &c, _size);
					memcpy(cacheBuffer, (nuint8_t*)&c+_size, cacheSize);
					result+=_size;
				}
			}
			return result;
		}

/***********************************************************************
Mbcs
***********************************************************************/

		nint MbcsEncoder::WriteString(wchar_t* _buffer, nint chars)
		{
			nint length=WideCharToMultiByte(CP_THREAD_ACP, 0, _buffer, (int)chars, NULL, NULL, NULL, NULL);
			char* mbcs=new char[length];
			WideCharToMultiByte(CP_THREAD_ACP, 0, _buffer, (int)chars, mbcs, (int)length, NULL, NULL);
			nint result=stream->Write(mbcs, length);
			delete[] mbcs;
			if(result==length)
			{
				return chars;
			}
			else
			{
				Close();
				return 0;
			}
		}

		nint MbcsDecoder::ReadString(wchar_t* _buffer, nint chars)
		{
			char* source=new char[chars*2];
			char* reading=source;
			nint readed=0;
			while(readed<chars)
			{
				if(stream->Read(reading, 1)!=1)
				{
					break;
				}
				if(IsDBCSLeadByte(*reading))
				{
					if(stream->Read(reading+1, 1)!=1)
					{
						break;
					}
					reading+=2;
				}
				else
				{
					reading++;
				}
				readed++;
			}
			MultiByteToWideChar(CP_THREAD_ACP, 0, source, (int)(reading-source), _buffer, (int)chars);
			delete[] source;
			return readed;
		}

/***********************************************************************
Utf-16
***********************************************************************/

		nint Utf16Encoder::WriteString(wchar_t* _buffer, nint chars)
		{
			return stream->Write(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
		}

		nint Utf16Decoder::ReadString(wchar_t* _buffer, nint chars)
		{
			return stream->Read(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
		}

/***********************************************************************
Utf-16-be
***********************************************************************/

		nint Utf16BEEncoder::WriteString(wchar_t* _buffer, nint chars)
		{
			nint writed=0;
			while(writed<chars)
			{
				if(stream->Write(((unsigned char*)_buffer)+1, 1)!=1)
				{
					break;
				}
				if(stream->Write(_buffer, 1)!=1)
				{
					break;
				}
				_buffer++;
				writed++;
			}
			if(writed!=chars)
			{
				Close();
			}
			return writed;
		}

		nint Utf16BEDecoder::ReadString(wchar_t* _buffer, nint chars)
		{
			chars=stream->Read(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
			unsigned char* unicode=(unsigned char*)_buffer;
			for(nint i=0;i<chars;i++)
			{
				unsigned char t=unicode[0];
				unicode[0]=unicode[1];
				unicode[1]=t;
				unicode++;
			}
			return chars;
		}

/***********************************************************************
Utf8
***********************************************************************/

		nint Utf8Encoder::WriteString(wchar_t* _buffer, nint chars)
		{
			nint length=WideCharToMultiByte(CP_UTF8, 0, _buffer, (int)chars, NULL, NULL, NULL, NULL);
			char* mbcs=new char[length];
			WideCharToMultiByte(CP_UTF8, 0, _buffer, (int)chars, mbcs, (int)length, NULL, NULL);
			nint result=stream->Write(mbcs, length);
			delete[] mbcs;
			if(result==length)
			{
				return chars;
			}
			else
			{
				Close();
				return 0;
			}
		}

		Utf8Decoder::Utf8Decoder()
			:cache(0)
			,cacheAvailable(false)
		{
		}

		nint Utf8Decoder::ReadString(wchar_t* _buffer, nint chars)
		{
			nuint8_t source[4];
			wchar_t target[2];
			wchar_t* writing=_buffer;
			nint readed=0;
			nint sourceCount=0;

			while(readed<chars)
			{
				if(cacheAvailable)
				{
					*writing++=cache;
					cache=0;
					cacheAvailable=false;
				}
				else
				{
					if(stream->Read(source, 1)!=1)
					{
						break;
					}
					if((*source & 0xF0) == 0xF0)
					{
						if(stream->Read(source+1, 3)!=3)
						{
							break;
						}
						sourceCount=4;
					}
					else if((*source & 0xE0) == 0xE0)
					{
						if(stream->Read(source+1, 2)!=2)
						{
							break;
						}
						sourceCount=3;
					}
					else if((*source & 0xC0) == 0xC0)
					{
						if(stream->Read(source+1, 1)!=1)
						{
							break;
						}
						sourceCount=2;
					}
					else
					{
						sourceCount=1;
					}
					int targetCount=MultiByteToWideChar(CP_UTF8, 0, (char*)source, (int)sourceCount, target, 2);
					if(targetCount==1)
					{
						*writing++=target[0];
					}
					else if(targetCount==2)
					{
						*writing++=target[0];
						cache=target[1];
						cacheAvailable=true;
					}
					else
					{
						break;
					}
				}
				readed++;
			}
			return readed;
		}

/***********************************************************************
BomEncoder
***********************************************************************/

		BomEncoder::BomEncoder(Encoding _encoding)
			:encoding(_encoding)
			,encoder(NULL)
		{
			switch(encoding)
			{
			case Mbcs:
				encoder=new MbcsEncoder;
				break;
			case Utf8:
				encoder=new Utf8Encoder;
				break;
			case Utf16:
				encoder=new Utf16Encoder;
				break;
			case Utf16BE:
				encoder=new Utf16BEEncoder;
				break;
			}
		}

		BomEncoder::~BomEncoder()
		{
			Close();
		}

		void BomEncoder::Setup(NIStream* _stream)
		{
			switch(encoding)
			{
			case Mbcs:
				break;
			case Utf8:
				_stream->Write((void*)"\xEF\xBB\xBF", 3);
				break;
			case Utf16:
				_stream->Write((void*)"\xFF\xFE", 2);
				break;
			case Utf16BE:
				_stream->Write((void*)"\xFE\xFF", 2);
				break;
			}
			encoder->Setup(_stream);
		}

		void BomEncoder::Close()
		{
			if(encoder)
			{
				encoder->Close();
				delete encoder;
				encoder= NULL;
			}
		}

		nint BomEncoder::Write(void* _buffer, nint _size)
		{
			return encoder->Write(_buffer, _size);
		}

/***********************************************************************
BomDecoder
***********************************************************************/

		BomDecoder::BomStream::BomStream(NIStream* _stream, char* _bom, nint _bomLength)
			:stream(_stream)
			,bomPosition(0)
			,bomLength(_bomLength)
		{
			memcpy(bom, _bom, bomLength);
		}

		bool BomDecoder::BomStream::CanRead()const
		{
			return IsAvailable();
		}

		bool BomDecoder::BomStream::CanWrite()const
		{
			return false;
		}

		bool BomDecoder::BomStream::CanSeek()const
		{
			return false;
		}

		bool BomDecoder::BomStream::CanPeek()const
		{
			return false;
		}

		bool BomDecoder::BomStream::IsLimited()const
		{
			return stream!= NULL && stream->IsLimited();
		}

		bool BomDecoder::BomStream::IsAvailable()const
		{
			return stream!= NULL && stream->IsAvailable();
		}

		void BomDecoder::BomStream::Close()
		{
			stream= NULL;
		}

		pos_t BomDecoder::BomStream::Position()const
		{
			return IsAvailable()?bomPosition+stream->Position():-1;
		}

		pos_t BomDecoder::BomStream::Size()const
		{
			return -1;
		}

		void BomDecoder::BomStream::Seek(pos_t _size)
		{
			CHECK_FAIL(L"BomDecoder::BomStream::Seek(pos_t)#Operation not supported.");
		}

		void BomDecoder::BomStream::SeekFromBegin(pos_t _size)
		{
			CHECK_FAIL(L"BomDecoder::BomStream::SeekFromBegin(pos_t)#Operation not supported.");
		}

		void BomDecoder::BomStream::SeekFromEnd(pos_t _size)
		{
			CHECK_FAIL(L"BomDecoder::BomStream::SeekFromEnd(pos_t)#Operation not supported.");
		}

		nint BomDecoder::BomStream::Read(void* _buffer, nint _size)
		{
			nint result=0;
			unsigned char* buffer=(unsigned char*)_buffer;
			if(bomPosition<bomLength)
			{
				nint remain=bomLength-bomPosition;
				result=remain<_size?remain:_size;
				memcpy(buffer, bom+bomPosition, result);
				buffer+=result;
				bomPosition+=result;
				_size-=result;
			}
			if(_size)
			{
				result+=stream->Read(buffer, _size);
			}
			return result;
		}

		nint BomDecoder::BomStream::Write(void* _buffer, nint _size)
		{
			CHECK_FAIL(L"BomDecoder::BomStream::Write(void*, nint)#Operation not supported.");
		}

		nint BomDecoder::BomStream::Peek(void* _buffer, nint _size)
		{
			CHECK_FAIL(L"BomDecoder::BomStream::Peek(void*, nint)#Operation not supported.");
		}

		BomDecoder::BomDecoder()
			:decoder(NULL)
		{
		}

		BomDecoder::~BomDecoder()
		{
			Close();
		}

		void BomDecoder::Setup(NIStream* _stream)
		{
			char bom[3]={0};
			nint length=_stream->Read(bom, sizeof(bom));
			if(strncmp(bom, "\xEF\xBB\xBF", 3)==0)
			{
				decoder=new Utf8Decoder;
				stream=new BomStream(_stream, bom+3, 0);
			}
			else if(strncmp(bom, "\xFF\xFE", 2)==0)
			{
				decoder=new Utf16Decoder;
				stream=new BomStream(_stream, bom+2, 1);
			}
			else if(strncmp(bom, "\xFE\xFF", 2)==0)
			{
				decoder=new Utf16BEDecoder;
				stream=new BomStream(_stream, bom+2, 1);
			}
			else
			{
				decoder=new MbcsDecoder;
				stream=new BomStream(_stream, bom, 3);
			}
			decoder->Setup(stream);
		}

		void BomDecoder::Close()
		{
			if(decoder)
			{
				decoder->Close();
				delete decoder;
				decoder= NULL;
				stream->Close();
				delete stream;
				stream= NULL;
			}
		}

		nint BomDecoder::Read(void* _buffer, nint _size)
		{
			return decoder->Read(_buffer, _size);
		}

/***********************************************************************
CharEncoder
***********************************************************************/

		bool CanBeMbcs(unsigned char* buffer, nint size)
		{
			for(nint i=0;i<size;i++)
			{
				if(buffer[i]==0) return false;
			}
			return true;
		}

		bool CanBeUtf8(unsigned char* buffer, nint size)
		{
			for(nint i=0;i<size;i++)
			{
				unsigned char c=(unsigned char)buffer[i];
				if(c==0)
				{
					return false;
				}
				else
				{
					nint count10xxxxxx=0;
					if((c&0x80)==0x00) /* 0x0xxxxxxx */ count10xxxxxx=0;
					else if((c&0xE0)==0xC0) /* 0x110xxxxx */ count10xxxxxx=1;
					else if((c&0xF0)==0xE0) /* 0x1110xxxx */ count10xxxxxx=2;
					else if((c&0xF8)==0xF0) /* 0x11110xxx */ count10xxxxxx=3;
					else if((c&0xFC)==0xF8) /* 0x111110xx */ count10xxxxxx=4;
					else if((c&0xFE)==0xFC) /* 0x1111110x */ count10xxxxxx=5;

					if(size<=i+count10xxxxxx)
					{
						return false;
					}
					else
					{
						for(nint j=0;j<count10xxxxxx;j++)
						{
							c=(unsigned char)buffer[i+j+1];
							if((c&0xC0)!=0x80) /* 0x10xxxxxx */ return false;
						}
					}
					i+=count10xxxxxx;
				}
			}
			return true;
		}

		bool CanBeUtf16(unsigned char* buffer, nint size, bool& hitSurrogatePairs)
		{
			hitSurrogatePairs = false;
			if (size % 2 != 0) return false;
			bool needTrail = false;
			for (nint i = 0; i < size; i += 2)
			{
				nuint16_t c = buffer[i] + (buffer[i + 1] << 8);
				if (c == 0) return false;
				nint type = 0;
				if (0xD800 <= c && c <= 0xDBFF) type = 1;
				else if (0xDC00 <= c && c <= 0xDFFF) type = 2;
				if (needTrail)
				{
					if (type == 2)
					{
						needTrail = false;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if (type == 1)
					{
						needTrail = true;
						hitSurrogatePairs = true;
					}
					else if (type != 0)
					{
						return false;
					}
				}
			}
			return !needTrail;
		}

		bool CanBeUtf16BE(unsigned char* buffer, nint size, bool& hitSurrogatePairs)
		{
			hitSurrogatePairs = false;
			if (size % 2 != 0) return false;
			bool needTrail = false;
			for (nint i = 0; i < size; i += 2)
			{
				nuint16_t c = buffer[i + 1] + (buffer[i] << 8);
				if (c == 0) return false;
				nint type = 0;
				if (0xD800 <= c && c <= 0xDBFF) type = 1;
				else if (0xDC00 <= c && c <= 0xDFFF) type = 2;
				if (needTrail)
				{
					if (type == 2)
					{
						needTrail = false;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if (type == 1)
					{
						needTrail = true;
						hitSurrogatePairs = true;
					}
					else if (type != 0)
					{
						return false;
					}
				}
			}
			return !needTrail;
		}

		template<nint Count>
		bool GetEncodingResult(int(&tests)[Count], bool(&results)[Count], int test)
		{
			for (nint i = 0; i < Count; i++)
			{
				if (tests[i] & test)
				{
					if (results[i]) return true;
				}
			}
			return false;
		}

		void TestEncoding(unsigned char* buffer, nint size, BomEncoder::Encoding& encoding, bool& containsBom)
		{
			if (size >= 3 && strncmp((char*)buffer, "\xEF\xBB\xBF", 3) == 0)
			{
				encoding = BomEncoder::Utf8;
				containsBom = true;
			}
			else if (size >= 2 && strncmp((char*)buffer, "\xFF\xFE", 2) == 0)
			{
				encoding = BomEncoder::Utf16;
				containsBom = true;
			}
			else if (size >= 2 && strncmp((char*)buffer, "\xFE\xFF", 2) == 0)
			{
				encoding = BomEncoder::Utf16BE;
				containsBom = true;
			}
			else
			{
				encoding = BomEncoder::Mbcs;
				containsBom = false;

				bool utf16HitSurrogatePairs = false;
				bool utf16BEHitSurrogatePairs = false;
				bool roughMbcs = CanBeMbcs(buffer, size);
				bool roughUtf8 = CanBeUtf8(buffer, size);
				bool roughUtf16 = CanBeUtf16(buffer, size, utf16HitSurrogatePairs);
				bool roughUtf16BE = CanBeUtf16BE(buffer, size, utf16BEHitSurrogatePairs);

				nint roughCount = (roughMbcs ? 1 : 0) + (roughUtf8 ? 1 : 0) + (roughUtf16 ? 1 : 0) + (roughUtf16BE ? 1 : 0);
				if (roughCount == 1)
				{
					if (roughUtf8) encoding = BomEncoder::Utf8;
					else if (roughUtf16) encoding = BomEncoder::Utf16;
					else if (roughUtf16BE) encoding = BomEncoder::Utf16BE;
				}
				else if (roughCount > 1)
				{
					int tests[] =
					{
						IS_TEXT_UNICODE_REVERSE_ASCII16,
						IS_TEXT_UNICODE_REVERSE_STATISTICS,
						IS_TEXT_UNICODE_REVERSE_CONTROLS,

						IS_TEXT_UNICODE_ASCII16,
						IS_TEXT_UNICODE_STATISTICS,
						IS_TEXT_UNICODE_CONTROLS,

						IS_TEXT_UNICODE_ILLEGAL_CHARS,
						IS_TEXT_UNICODE_ODD_LENGTH,
						IS_TEXT_UNICODE_NULL_BYTES,
					};

					const nint TestCount = sizeof(tests) / sizeof(*tests);
					bool results[TestCount];
					for (nint i = 0; i < TestCount; i++)
					{
						int test = tests[i];
						results[i] = IsTextUnicode(buffer, (int)size, &test) != 0;
					}

					if (size % 2 == 0
						&& !GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_ASCII16)
						&& !GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_STATISTICS)
						&& !GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_CONTROLS)
						)
					{
						for (nint i = 0; i < size; i += 2)
						{
							unsigned char c = buffer[i];
							buffer[i] = buffer[i + 1];
							buffer[i + 1] = c;
						}
						// 3 = (count of reverse group) = (count of unicode group)
						for (nint i = 0; i < 3; i++)
						{
							int test = tests[i + 3];
							results[i] = IsTextUnicode(buffer, (int)size, &test) != 0;
						}
						for (nint i = 0; i < size; i += 2)
						{
							unsigned char c = buffer[i];
							buffer[i] = buffer[i + 1];
							buffer[i + 1] = c;
						}
					}

					if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_NOT_UNICODE_MASK))
					{
						if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_NOT_ASCII_MASK))
						{
							encoding = BomEncoder::Utf8;
						}
						else if (roughUtf8 || !roughMbcs)
						{
							encoding = BomEncoder::Utf8;
						}
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_ASCII16))
					{
						encoding = BomEncoder::Utf16;
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_ASCII16))
					{
						encoding = BomEncoder::Utf16BE;
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_CONTROLS))
					{
						encoding = BomEncoder::Utf16;
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_CONTROLS))
					{
						encoding = BomEncoder::Utf16BE;
					}
					else
					{
						if (!roughUtf8)
						{
							if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_STATISTICS))
							{
								encoding = BomEncoder::Utf16;
							}
							else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_STATISTICS))
							{
								encoding = BomEncoder::Utf16BE;
							}
						}
						else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_NOT_UNICODE_MASK))
						{
							encoding = BomEncoder::Utf8;
						}
						else if (roughUtf8 || !roughMbcs)
						{
							encoding = BomEncoder::Utf8;
						}
					}

				}
			}
		}
