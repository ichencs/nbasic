/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Data Structure::String

Classes:
	AString										£ºMbcs×Ö·û´®
	WString										£ºUtf-16×Ö·û´®
***********************************************************************/

#ifndef VCZH_STRING
#define VCZH_STRING

#include <memory.h>
#include "Basic.h"

/// <summary>A type representing a string.</summary>
/// <typeparam name="T">Type of a character.</typeparam>
template<typename T>
class ObjectString : public Object
{
	private:
		static const T				zero;

		mutable T*					buffer;
		mutable volatile nint* 		counter;
		mutable nint				start;
		mutable nint				length;
		mutable nint				realLength;

		static nint  CalculateLength(const T* buffer)
		{
			nint result = 0;

			while (*buffer++)
			{
				result++;
			}

			return result;
		}

		static nint  Compare(const T* bufA, const ObjectString<T>& strB)
		{
			const T* bufB = strB.buffer + strB.start;
			const T* bufAOld = bufA;
			nint length = strB.length;

			while (length-- && *bufA)
			{
				nint diff = *bufA++ - *bufB++;

				if (diff != 0)
				{
					return diff;
				}
			};

			return CalculateLength(bufAOld) - strB.length;
		}

	public:

		static nint  Compare(const ObjectString<T>& strA, const ObjectString<T>& strB)
		{
			const T* bufA = strA.buffer + strA.start;
			const T* bufB = strB.buffer + strB.start;
			nint length = strA.length < strB.length ? strA.length : strB.length;

			while (length--)
			{
				nint diff = *bufA++ -*bufB++;

				if (diff != 0)
				{
					return diff;
				}
			};

			return strA.length - strB.length;
		}

	private:

		void Inc()const
		{
			if (counter)
			{
				INCRC(counter);
			}
		}

		void Dec()const
		{
			if (counter)
			{
				if (DECRC(counter) == 0)
				{
					delete[] buffer;
					delete counter;
				}
			}
		}

		ObjectString(const ObjectString<T>& string, nint  _start, nint  _length)
		{
			if (_length <= 0)
			{
				buffer = (T*)&zero;
				counter = 0;
				start = 0;
				length = 0;
				realLength = 0;
			}
			else
			{
				buffer = string.buffer;
				counter = string.counter;
				start = string.start + _start;
				length = _length;
				realLength = string.realLength;
				Inc();
			}
		}

		/// <summary>
		///
		/// </summary>
		/// <param name="dest"></param>
		/// <param name="source"></param>
		/// <param name="index"></param>
		/// <param name="count"></param>
		/// <returns></returns>
		ObjectString(const ObjectString<T>& dest, const ObjectString<T>& source, nint  index, nint  count)
		{
			if (index == 0 && count == dest.length && source.length == 0)
			{
				buffer = (T*)&zero;
				counter = 0;
				start = 0;
				length = 0;
				realLength = 0;
			}
			else
			{
				counter = new nint (1);
				start = 0;
				length = dest.length - count + source.length;
				realLength = length;
				buffer = new T[length + 1];
				memcpy(buffer, dest.buffer + dest.start, sizeof(T)*index);
				memcpy(buffer + index, source.buffer + source.start, sizeof(T)*source.length);
				memcpy(buffer + index + source.length, (dest.buffer + dest.start + index + count), sizeof(T) * (dest.length - index - count));
				buffer[length] = 0;
			}
		}
	public:
		static ObjectString<T>	Empty;

		/// <summary>Create an empty string.</summary>
		ObjectString()
		{
			buffer = (T*)&zero;
			counter = 0;
			start = 0;
			length = 0;
			realLength = 0;
		}

		/// <summary>Create a string continaing one character.</summary>
		/// <param name="_char">The character.</param>
		ObjectString(const T& _char)
		{
			counter = new nint (1);
			start = 0;
			length = 1;
			buffer = new T[2];
			buffer[0] = _char;
			buffer[1] = 0;
			realLength = length;
		}

		/// <summary>Copy a string.</summary>
		/// <param name="_buffer">Memory to copy. It does not have to contain the zero terminator.</param>
		/// <param name="_length">Size of the content in characters.</param>
		ObjectString(const T* _buffer, nint  _length)
		{
			if (_length <= 0)
			{
				buffer = (T*)&zero;
				counter = 0;
				start = 0;
				length = 0;
				realLength = 0;
			}
			else
			{
				buffer = new T[_length + 1];
				memcpy(buffer, _buffer, _length * sizeof(T));
				buffer[_length] = 0;
				counter = new nint (1);
				start = 0;
				length = _length;
				realLength = _length;
			}
		}

		/// <summary>Copy a string.</summary>
		/// <param name="_buffer">Memory to copy. It should have to contain the zero terminator.</param>
		/// <param name="copy">Set to true to copy the memory. Set to false to use the memory directly.</param>
		ObjectString(const T* _buffer, bool copy = true)
		{
			CHECK_ERROR(_buffer != 0, L"ObjectString<T>::ObjectString(const T*, bool)#Cannot construct a string from nullptr.");

			if (copy)
			{
				counter = new nint (1);
				start = 0;
				length = CalculateLength(_buffer);
				buffer = new T[length + 1];
				memcpy(buffer, _buffer, sizeof(T) * (length + 1));
				realLength = length;
			}
			else
			{
				buffer = (T*)_buffer;
				counter = 0;
				start = 0;
				length = CalculateLength(_buffer);
				realLength = length;
			}
		}

		/// <summary>Copy a string.</summary>
		/// <param name="string">The string to copy.</param>
		ObjectString(const ObjectString<T>& string)
		{
			buffer = string.buffer;
			counter = string.counter;
			start = string.start;
			length = string.length;
			realLength = string.realLength;
			Inc();
		}

		/// <summary>Move a string.</summary>
		/// <param name="string">The string to move.</param>
		// 		ObjectString(ObjectString<T>&& string)
		// 		{
		// 			buffer=string.buffer;
		// 			counter=string.counter;
		// 			start=string.start;
		// 			length=string.length;
		// 			realLength=string.realLength;
		//
		// 			string.buffer=(T*)&zero;
		// 			string.counter=0;
		// 			string.start=0;
		// 			string.length=0;
		// 			string.realLength=0;
		// 		}

		~ObjectString()
		{
			Dec();
		}

		bool IsEmpty()const
		{
			return *this == Empty;
		}


		/// <summary>Get the zero-terminated buffer in the string. Copying parts of a string does not necessarily create a new buffer, so in some situation the string will not actually points to a zero-terminated buffer. In this case, this function will copy the content to a new buffer with a zero terminator and return.</summary>
		/// <returns>Returns the buffer.</returns>
		const T* Buffer()const
		{
			if (start + length != realLength)
			{
				T* newBuffer = new T[length + 1];
				memcpy(newBuffer, buffer + start, sizeof(T)*length);
				newBuffer[length] = 0;
				Dec();
				buffer = newBuffer;
				counter = new nint (1);
				start = 0;
				realLength = length;
			}

			return buffer + start;
		}

		ObjectString<T>& operator=(const ObjectString<T>& string)
		{
			if (this != &string)
			{
				Dec();
				buffer = string.buffer;
				counter = string.counter;
				start = string.start;
				length = string.length;
				realLength = string.realLength;
				Inc();
			}

			return *this;
		}

		ObjectString<T>& operator+=(const ObjectString<T>& string)
		{
			return *this = *this + string;
		}

		ObjectString<T> operator+(const ObjectString<T>& string)const
		{
			return ObjectString<T>(*this, string, length, 0);
		}

		bool operator==(const ObjectString<T>& string)const
		{
			return Compare(*this, string) == 0;
		}

		bool operator!=(const ObjectString<T>& string)const
		{
			return Compare(*this, string) != 0;
		}

		bool operator>(const ObjectString<T>& string)const
		{
			return Compare(*this, string) > 0;
		}

		bool operator>=(const ObjectString<T>& string)const
		{
			return Compare(*this, string) >= 0;
		}

		bool operator<(const ObjectString<T>& string)const
		{
			return Compare(*this, string) < 0;
		}

		bool operator<=(const ObjectString<T>& string)const
		{
			return Compare(*this, string) <= 0;
		}

		bool operator==(const T* buffer)const
		{
			return Compare(buffer, *this) == 0;
		}

		bool operator!=(const T* buffer)const
		{
			return Compare(buffer, *this) != 0;
		}

		bool operator>(const T* buffer)const
		{
			return Compare(buffer, *this) < 0;
		}

		bool operator>=(const T* buffer)const
		{
			return Compare(buffer, *this) <= 0;
		}

		bool operator<(const T* buffer)const
		{
			return Compare(buffer, *this) > 0;
		}

		bool operator<=(const T* buffer)const
		{
			return Compare(buffer, *this) >= 0;
		}

		T operator[](nint index)const
		{
			CHECK_ERROR(index >= 0 && index < length, L"ObjectString:<T>:operator[](nint)#Argument index not in range.");
			return buffer[start + index];
		}

		/// <summary>Get the size of the string in characters.</summary>
		/// <returns>The size.</returns>
		nint Length()const
		{
			return length;
		}

		/// <summary>Find a character.</summary>
		/// <returns>The position of the character. Returns -1 if it doesn not exist.</returns>
		/// <param name="c">The character to find.</param>
		nint IndexOf(T c)const
		{
			const T* reading = buffer + start;

			for (nint i = 0; i < length; i++)
			{
				if (reading[i] == c)
				{
					return i;
				}
			}

			return -1;
		}



		/// <summary>Copy the beginning of the string.</summary>
		/// <returns>The copied string.</returns>
		/// <param name="count">Size of characters to copy.</param>
		ObjectString<T> Left(nint count)const
		{
			CHECK_ERROR(count >= 0 && count <= length, L"ObjectString<T>::Left(nint)#Argument count not in range.");
			return ObjectString<T>(*this, 0, count);
		}

		/// <summary>Copy the ending of the string.</summary>
		/// <returns>The copied string.</returns>
		/// <param name="count">Size of characters to copy.</param>
		ObjectString<T> Right(nint count)const
		{
			CHECK_ERROR(count >= 0 && count <= length, L"ObjectString<T>::Right(nint)#Argument count not in range.");
			return ObjectString<T>(*this, length - count, count);
		}

		/// <summary>Copy the middle of the string.</summary>
		/// <returns>The copied string.</returns>
		/// <param name="index">Position of characters to copy.</param>
		/// <param name="count">Size of characters to copy.</param>
		ObjectString<T> Sub(nint index, nint  count)const
		{
			CHECK_ERROR(index >= 0 && index <= length, L"ObjectString<T>::Sub(nint, nint )#Argument index not in range.");
			CHECK_ERROR(index + count >= 0 && index + count <= length, L"ObjectString<T>::Sub(nint, nint )#Argument count not in range.");
			return ObjectString<T>(*this, index, count);
		}
		ObjectString<T>Mid(nint index, nint  count)const
		{
			CHECK_ERROR(index >= 0 && index <= length, L"ObjectString<T>::Sub(nint, nint )#Argument index not in range.");
			CHECK_ERROR(index + count >= 0 && index + count <= length, L"ObjectString<T>::Sub(nint, nint )#Argument count not in range.");
			return ObjectString<T>(*this, index, count);
		}

		/// <summary>Copy the beginning and the end of the string.</summary>
		/// <returns>The copied string.</returns>
		/// <param name="index">Position of characters NOT to copy.</param>
		/// <param name="count">Size of characters NOT to copy.</param>
		ObjectString<T> Remove(nint index, nint  count)const
		{
			CHECK_ERROR(index >= 0 && index < length, L"ObjectString<T>::Remove(nint, nint )#Argument index not in range.");
			CHECK_ERROR(index + count >= 0 && index + count <= length, L"ObjectString<T>::Remove(nint, nint )#Argument count not in range.");
			return ObjectString<T>(*this, ObjectString<T>(), index, count);
		}

		/// <summary>Make a new string by inserting a string in this string.</summary>
		/// <returns>The copied string.</returns>
		/// <param name="index">Position of characters to insert.</param>
		/// <param name="string">The string to be inserted in this string.</param>
		ObjectString<T> Insert(nint index, const ObjectString<T>& string)const
		{
			CHECK_ERROR(index >= 0 && index <= length, L"ObjectString<T>::Insert(nint)#Argument count not in range.");
			return ObjectString<T>(*this, string, index, 0);
		}

		ObjectString<T> Trim(T wc)
		{
			nint nsatrt = 0;
			nint nend = length;

			while (nend && buffer[nend - 1] == wc)
			{
				nend--;
			}

			while (buffer[nsatrt] == wc && nsatrt < length)
			{
				nsatrt++;
			}

			return ObjectString<T>(*this, nsatrt, nend - nsatrt);
		};
		ObjectString<T> TrimRight(T wc)const
		{
			nint nend = length ;

			while (nend && buffer[nend - 1] == wc)
			{
				nend--;
			}

			return ObjectString<T>(*this, 0, nend);
		};

		ObjectString<T> TrimLeft(T wc)
		{
			nint nstart = 0;

			while (buffer[nstart] == wc && nstart < length)
			{
				nstart++;
			}

			return ObjectString<T>(*this, nstart, length - nstart);
		};



		friend bool operator<(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right) < 0;
		}

		friend bool operator<=(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right) <= 0;
		}

		friend bool operator>(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right) > 0;
		}

		friend bool operator>=(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right) >= 0;
		}

		friend bool operator==(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right) == 0;
		}

		friend bool operator!=(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right) != 0;
		}

		friend ObjectString<T> operator+(const T* left, const ObjectString<T>& right)
		{
			return ObjectString<T>(left, false) + right;
		}
};

template<typename T>
ObjectString<T> ObjectString<T>::Empty = ObjectString<T>();
template<typename T>
const T ObjectString<T>::zero = 0;

/// <summary>Ansi string.</summary>
typedef ObjectString<char>		AString;
/// <summary>Unicode string.</summary>
typedef ObjectString<wchar_t>	WString;

/// <summary>Convert a string to an signed nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nint 					atoi_test(const AString& string, bool& success);
/// <summary>Convert a string to an signed nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nint 					wtoi_test(const WString& string, bool& success);
/// <summary>Convert a string to an signed 64-bits nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nint64_t				atoi64_test(const AString& string, bool& success);
/// <summary>Convert a string to an signed 64-bits nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nint64_t				wtoi64_test(const WString& string, bool& success);
/// <summary>Convert a string to an unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nuint				atou_test(const AString& string, bool& success);
/// <summary>Convert a string to an unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nuint				wtou_test(const WString& string, bool& success);
/// <summary>Convert a string to a 64-bits unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nuint64_t			atou64_test(const AString& string, bool& success);
/// <summary>Convert a string to a 64-bits unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern nuint64_t			wtou64_test(const WString& string, bool& success);
/// <summary>Convert a string to 64-bits floating point number.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern double				atof_test(const AString& string, bool& success);
/// <summary>Convert a string to 64-bits floating point number.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
/// <param name="success">Returns true if this operation succeeded.</param>
extern double				wtof_test(const WString& string, bool& success);

/// <summary>Convert a string to an signed nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nint 					atoi(const AString& string);
/// <summary>Convert a string to an signed nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nint 					wtoi(const WString& string);
/// <summary>Convert a string to an signed 64-bits nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nint64_t				atoi64(const AString& string);
/// <summary>Convert a string to an signed 64-bits nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nint64_t				wtoi64(const WString& string);
/// <summary>Convert a string to an unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nuint				atou(const AString& string);
/// <summary>Convert a string to an unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nuint				wtou(const WString& string);
/// <summary>Convert a string to a 64-bits unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nuint64_t			atou64(const AString& string);
/// <summary>Convert a string to a 64-bits unsigned nint eger.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern nuint64_t			wtou64(const WString& string);
/// <summary>Convert a string to a 64-bits floating point number.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern double				atof(const AString& string);
/// <summary>Convert a string to a 64-bits floating point number.</summary>
/// <returns>The converted number. If the convert failed, the result is undefined.</returns>
/// <param name="string">The string to convert.</param>
extern double				wtof(const WString& string);

/// <summary>Convert a signed nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern AString				itoa(nint number);
/// <summary>Convert a signed nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern WString				itow(nint number);
/// <summary>Convert a 64-bits signed nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern AString				i64toa(nint64_t number);
/// <summary>Convert a 64-bits signed nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern WString				i64tow(nint64_t number);
/// <summary>Convert an unsigned nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern AString				utoa(nuint number);
/// <summary>Convert an unsigned nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern WString				utow(nuint number);
/// <summary>Convert a 64-bits unsigned nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern AString				u64toa(nuint64_t number);
/// <summary>Convert a 64-bits unsigned nint erger to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern WString				u64tow(nuint64_t number);
/// <summary>Convert a 64-bits floating pointer number to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern AString				ftoa(double number);
/// <summary>Convert a 64-bits floating pointer number to a string.</summary>
/// <returns>The converted string.</returns>
/// <param name="number">The number to convert.</param>
extern WString				ftow(double number);

extern nint 					_wtoa(const wchar_t* w, char* a, nint  chars);
/// <summary>Convert an Unicode string to an Ansi string.</summary>
/// <returns>The converted string.</returns>
/// <param name="string">The string to convert.</param>
extern AString				wtoa(const WString& string);
extern nint 					_atow(const char* a, wchar_t* w, nint  chars);
/// <summary>Convert an Ansi string to an Unicode string.</summary>
/// <returns>The converted string.</returns>
/// <param name="string">The string to convert.</param>
extern WString				atow(const AString& string);
/// <summary>Convert all letters to lower case letters.</summary>
/// <returns>The converted string.</returns>
/// <param name="string">The string to convert.</param>
extern AString				alower(const AString& string);
/// <summary>Convert all letters to lower case letters.</summary>
/// <returns>The converted string.</returns>
/// <param name="string">The string to convert.</param>
extern WString				wlower(const WString& string);
/// <summary>Convert all letters to upper case letters.</summary>
/// <returns>The converted string.</returns>
/// <param name="string">The string to convert.</param>
extern AString				aupper(const AString& string);
/// <summary>Convert all letters to upper case letters.</summary>
/// <returns>The converted string.</returns>
/// <param name="string">The string to convert.</param>
extern WString				wupper(const WString& string);


#endif
