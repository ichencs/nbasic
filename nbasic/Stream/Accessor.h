/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Stream::Accessor

Classes:
	TextReader						���ַ����Ķ���
	TextWriter						���ַ�����д��
	StreamReader					�����Ķ���
	StreamWriter					������д��
	EncoderStream					��������
	DecoderStream					��������
***********************************************************************/

#ifndef VCZH_STREAM_ACCESSOR
#define VCZH_STREAM_ACCESSOR

#include "Interfaces.h"
#include "../Collections/NList.h"
#include "../String.h"


/***********************************************************************
��������
***********************************************************************/

/// <summary>Text reader. All line breaks are normalized to CRLF regardless the format in the source.</summary>
class TextReader : public Object, private NotCopyable
{
	public:
		/// <summary>Test does the reader reach the end or not.</summary>
		/// <returns>Returns true if the reader reaches the end.</returns>
		virtual bool				IsEnd() = 0;
		/// <summary>Read a single character.</summary>
		/// <returns>The character.</returns>
		virtual wchar_t				ReadChar() = 0;
		/// <summary>Read a string of a specified size.</summary>
		/// <returns>The string.</returns>
		/// <param name="length">Expected length of the string to read.</param>
		virtual WString				ReadString(nint length);
		/// <summary>Read a string until a line breaks is reached.</summary>
		/// <returns>The string. It does not contain the line break.</returns>
		virtual WString				ReadLine();
		/// <summary>Read everying remain.</summary>
		/// <returns>The string.</returns>
		virtual WString				ReadToEnd();
};

/// <summary>Text writer.</summary>
class TextWriter : public Object, private NotCopyable
{
	public:
		/// <summary>Write a single character.</summary>
		/// <param name="c">The character to write.</param>
		virtual void				WriteChar(wchar_t c) = 0;
		/// <summary>Write a string.</summary>
		/// <param name="string">Buffer to the string to write.</param>
		/// <param name="charCount">Size of the string in characters not including the zero terminator.</param>
		virtual void				WriteString(const wchar_t* string, nint charCount);
		/// <summary>Write a string.</summary>
		/// <param name="string">Buffer to the zero terminated string to write.</param>
		virtual void				WriteString(const wchar_t* string);
		/// <summary>Write a string.</summary>
		/// <param name="string">The string to write.</param>
		virtual void				WriteString(const WString& string);
		/// <summary>Write a string with a CRLF.</summary>
		/// <param name="string">Buffer to the string to write.</param>
		/// <param name="charCount">Size of the string in characters not including the zero terminator.</param>
		virtual void				WriteLine(const wchar_t* string, nint charCount);
		/// <summary>Write a string with a CRLF.</summary>
		/// <param name="string">Buffer to the zero terminated string to write.</param>
		virtual void				WriteLine(const wchar_t* string);
		/// <summary>Write a string with a CRLF.</summary>
		/// <param name="string">The string to write.</param>
		virtual void				WriteLine(const WString& string);
		
		virtual void				WriteMonospacedEnglishTable(NArray<WString>& tableByRow, nint rows, nint columns);
};

/// <summary>Text reader from a string.</summary>
class StringReader : public TextReader
{
	protected:
		WString						string;
		nint						current;
		bool						lastCallIsReadLine;
		
		void						PrepareIfLastCallIsReadLine();
	public:
		/// <summary>Create a text reader.</summary>
		/// <param name="_string">The string to read.</param>
		StringReader(const WString& _string);
		
		bool						IsEnd();
		wchar_t						ReadChar();
		WString						ReadString(nint length);
		WString						ReadLine();
		WString						ReadToEnd();
};

/// <summary>Text reader from a stream.</summary>
class StreamReader : public TextReader
{
	protected:
		NIStream*					stream;
	public:
		/// <summary>Create a text reader.</summary>
		/// <param name="_stream">The stream to read.</param>
		StreamReader(NIStream& _stream);
		
		bool						IsEnd();
		wchar_t						ReadChar();
};

/// <summary>Text writer to a stream.</summary>
class StreamWriter : public TextWriter
{
	protected:
		NIStream*					stream;
	public:
		/// <summary>Create a text writer.</summary>
		/// <param name="_stream">The stream to write.</param>
		StreamWriter(NIStream& _stream);
		using TextWriter::WriteString;
		
		void						WriteChar(wchar_t c);
		void						WriteString(const wchar_t* string, nint charCount);
};

/***********************************************************************
�������
***********************************************************************/

/// <summary>Encoder stream, a writable stream using an [T:vl.stream.IEncoder] to transform content.</summary>
class EncoderStream : public virtual NIStream
{
	protected:
		NIStream*					stream;
		IEncoder*					encoder;
		pos_t						position;
		
	public:
		/// <summary>Create a stream.</summary>
		/// <param name="_stream">The target stream to write.</param>
		/// <param name="_encoder">The encoder to transform content.</param>
		EncoderStream(NIStream& _stream, IEncoder& _encoder);
		~EncoderStream();
		
		bool						CanRead()const;
		bool						CanWrite()const;
		bool						CanSeek()const;
		bool						CanPeek()const;
		bool						IsLimited()const;
		bool						IsAvailable()const;
		void						Close();
		pos_t						Position()const;
		pos_t						Size()const;
		void						Seek(pos_t _size);
		void						SeekFromBegin(pos_t _size);
		void						SeekFromEnd(pos_t _size);
		nint						Read(void* _buffer, nint _size);
		nint						Write(void* _buffer, nint _size);
		nint						Peek(void* _buffer, nint _size);
};

/// <summary>Decoder stream, a readable stream using an [T:vl.stream.IDecoder] to transform content.</summary>
class DecoderStream : public virtual NIStream
{
	protected:
		NIStream*					stream;
		IDecoder*					decoder;
		pos_t						position;
		
	public:
		/// <summary>Create a stream.</summary>
		/// <param name="_stream">The target stream to read.</param>
		/// <param name="_decoder">The decoder to transform content.</param>
		DecoderStream(NIStream& _stream, IDecoder& _decoder);
		~DecoderStream();
		
		bool						CanRead()const;
		bool						CanWrite()const;
		bool						CanSeek()const;
		bool						CanPeek()const;
		bool						IsLimited()const;
		bool						IsAvailable()const;
		void						Close();
		pos_t						Position()const;
		pos_t						Size()const;
		void						Seek(pos_t _size);
		void						SeekFromBegin(pos_t _size);
		void						SeekFromEnd(pos_t _size);
		nint						Read(void* _buffer, nint _size);
		nint						Write(void* _buffer, nint _size);
		nint						Peek(void* _buffer, nint _size);
};

#endif