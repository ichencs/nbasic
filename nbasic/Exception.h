/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Exception

Classes:
	Exception									：异常
	ArgumentException							：预料的的参数错误
***********************************************************************/

#ifndef VCZH_EXCEPTION
#define VCZH_EXCEPTION

#include "String.h"

	/// <summary>Base type of all exceptions.</summary>
	class Exception : public Object
	{
	protected:
		WString						message;

	public:
		Exception(const WString& _message=WString::Empty);

		const WString&				Message()const;
	};

	class ArgumentException : public Exception
	{
	protected:
		WString						function;
		WString						name;

	public:
		ArgumentException(const WString& _message=WString::Empty, const WString& _function=WString::Empty, const WString& _name=WString::Empty);

		const WString&				GetFunction()const;
		const WString&				GetName()const;
	};

	class ParsingException : public Exception
	{
	protected:
		nint							position;
		WString						expression;

	public:
		ParsingException(const WString& _message, const WString& _expression, nint _position);

		const WString&				GetExpression()const;
		nint							GetPosition()const;
	};

#endif