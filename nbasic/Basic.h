/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Basic

Classes:
	NotCopyable									：不可复制对象
	Error										：内部错误，检查到了不可出现的变量、参数或状态错误
	Object										：对象基类

Macros:
	CHECK_ERROR(CONDITION,DESCRIPTION)			：检查内部错误
***********************************************************************/

#ifndef VCZH_BASIC
#define VCZH_BASIC


#if defined _WIN64 || __x86_64 || __LP64__
#define VCZH_64
#endif

#if defined _MSC_VER
#define VCZH_MSVC
#else
// #define VCZH_GCC
// #if defined(__APPLE__)
// #define VCZH_APPLE
// #endif
#endif

#if defined VCZH_MSVC
#include <intrin.h>
#endif

#define L_(x) L__(x)
#define L__(x) L ## x


/***********************************************************************
32位/64位兼容
***********************************************************************/

#if defined VCZH_MSVC
	/// <summary>1-byte signed integer.</summary>
	typedef signed __int8			int8_t;
	/// <summary>1-byte unsigned integer.</summary>
	typedef unsigned __int8			vuint8_t;
	/// <summary>2-bytes signed integer.</summary>
	typedef signed __int16			int16_t;
	/// <summary>2-bytes unsigned integer.</summary>
	typedef unsigned __int16		vuint16_t;
	/// <summary>4-bytes signed integer.</summary>
	typedef signed __int32			int32_t;
	/// <summary>4-bytes unsigned integer.</summary>
	typedef unsigned __int32		vuint32_t;
	/// <summary>8-bytes signed integer.</summary>
	typedef signed __int64			int64_t;
	/// <summary>8-bytes unsigned integer.</summary>
	typedef unsigned __int64		vuint64_t;
#endif

	/// <summary>Signed interger representing position.</summary>
	typedef int64_t				pos_t;

#ifdef VCZH_64
#define ITOA_S		_i64toa_s
#define ITOW_S		_i64tow_s
#define I64TOA_S	_i64toa_s
#define I64TOW_S	_i64tow_s
#define UITOA_S		_ui64toa_s
#define UITOW_S		_ui64tow_s
#define UI64TOA_S	_ui64toa_s
#define UI64TOW_S	_ui64tow_s
// #define INCRC(x)	(_InterlockedIncrement64(x))
// #define DECRC(x)	(_InterlockedDecrement64(x))
#else
#define ITOA_S		_itoa_s
#define ITOW_S		_itow_s
#define I64TOA_S	_i64toa_s
#define I64TOW_S	_i64tow_s
#define UITOA_S		_ui64toa_s
#define UITOW_S		_ui64tow_s
#define UI64TOA_S	_ui64toa_s
#define UI64TOW_S	_ui64tow_s
#endif
#define INCRC(x)	(_InterlockedIncrement((volatile long*)(x)))
#define DECRC(x)	(_InterlockedDecrement((volatile long*)(x)))

/***********************************************************************
基础
***********************************************************************/

	class NotCopyable
	{
	private:
		NotCopyable(const NotCopyable&);
		NotCopyable& operator=(const NotCopyable&);
	public:
		NotCopyable();
	};
	
	/// <summary>Base type of all errors. An error is an exception that you are not allowed to catch. Raising it means there is a fatal error in the code.</summary>
	class Error
	{
	private:
		const wchar_t*		description;
	public:
		Error(const wchar_t* _description);

		const wchar_t*		Description()const;
	};

#if defined _DEBUG
	#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)
#elif defined NDEBUG
	#define CHECK_ERROR(CONDITION,DESCRIPTION)
#endif

#define CHECK_FAIL(DESCRIPTION) do{throw Error(DESCRIPTION);}while(0)

#define SCOPE_VARIABLE(TYPE, VARIABLE, VALUE)\
	if(bool __scope_variable_flag__=true)\
		for(TYPE VARIABLE = VALUE;__scope_variable_flag__;__scope_variable_flag__=false)

/***********************************************************************
类型计算
***********************************************************************/
	
	template<typename T>
	struct RemoveReference
	{
		typedef T			Type;
	};

	template<typename T>
	struct RemoveReference<T&>
	{
		typedef T			Type;
	};

	/*template<typename T>
	struct RemoveReference<T&&>
	{
		typedef T			Type;
	};*/

	template<typename T>
	struct RemoveConst
	{
		typedef T			Type;
	};

	template<typename T>
	struct RemoveConst<const T>
	{
		typedef T			Type;
	};

	template<typename T>
	struct RemoveVolatile
	{
		typedef T			Type;
	};

	template<typename T>
	struct RemoveVolatile<volatile T>
	{
		typedef T			Type;
	};

	template<typename T>
	struct RemoveCVR
	{
		typedef T								Type;
	};

	template<typename T>
	struct RemoveCVR<T&>
	{
		typedef typename RemoveCVR<T>::Type		Type;
	};


	template<typename T>
	struct RemoveCVR<const T>
	{
		typedef typename RemoveCVR<T>::Type		Type;
	};

	template<typename T>
	struct RemoveCVR<volatile T>
	{
		typedef typename RemoveCVR<T>::Type		Type;
	};

	

/***********************************************************************
基础
***********************************************************************/

	/// <summary>Base type of all classes.</summary>
	class Object
	{
	public:
		virtual ~Object();
	};
	
	/// <summary>Type for storing a value to wherever requiring a [T:vl.Ptr`1] to [T:vl.Object].</summary>
	/// <typeparam name="T">Type of the value.</typeparam>
	template<typename T>
	class ObjectBox : public Object
	{
	private:
		T					object;
	public:
		/// <summary>Box a value.</summary>
		/// <param name="_object">The value to box.</param>
		ObjectBox(const T& _object)
			:object(_object)
		{
		}
		
		/// <summary>Box a movable value.</summary>
		/// <param name="_object">The value to box.</param>
// 		ObjectBox(T&& _object)
// 			:object(MoveValue(_object))
// 		{
// 		}
		
		/// <summary>Copy a box.</summary>
		/// <param name="value">The box.</param>
		ObjectBox(const ObjectBox<T>& value)
			:object(value.object)
		{
		}
		
		/// <summary>Move a box.</summary>
		/// <param name="value">The box.</param>
// 		ObjectBox(ObjectBox<T>&& value)
// 			:object(MoveValue(value.object))
// 		{
// 		}
		
		/// <summary>Box a value.</summary>
		/// <returns>The boxed value.</returns>
		/// <param name="_object">The value to box.</param>
		ObjectBox<T>& operator=(const T& _object)
		{
			object=_object;
			return *this;
		}
		
		/// <summary>Copy a box.</summary>
		/// <returns>The boxed value.</returns>
		/// <param name="value">The box.</param>
		ObjectBox<T>& operator=(const ObjectBox<T>& value)
		{
			object=value.object;
			return *this;
		}
		

		/// <summary>Unbox the value.</summary>
		/// <returns>The original value.</returns>
		const T& Unbox()
		{
			return object;
		}
	};

	/// <summary>Type for optionally storing a value.</summary>
	/// <typeparam name="T">Type of the value.</typeparam>
	template<typename T>
	class Nullable
	{
	private:
		T*					object;
	public:
		/// <summary>Create a null value.</summary>
		Nullable()
			:object(0)
		{
		}

		/// <summary>Create a non-null value.</summary>
		/// <param name="value">The value to copy.</param>
		Nullable(const T& value)
			:object(new T(value))
		{
		}
		
		/// <summary>Create a non-null value.</summary>
		/// <param name="value">The value to move.</param>
// 		Nullable(T&& value)
// 			:object(new T(MoveValue(value)))
// 		{
// 		}

		/// <summary>Copy a nullable value.</summary>
		/// <param name="nullable">The nullable value to copy.</param>
		Nullable(const Nullable<T>& nullable)
			:object(nullable.object?new T(*nullable.object):0)
		{
		}
		


		~Nullable()
		{
			if(object)
			{
				delete object;
				object=0;
			}
		}
		
		/// <summary>Create a non-null value.</summary>
		/// <returns>The created nullable value.</returns>
		/// <param name="value">The value to copy.</param>
		Nullable<T>& operator=(const T& value)
		{
			if(object)
			{
				delete object;
				object=0;
			}
			object=new T(value);
			return *this;
		}
		
		/// <summary>Copy a nullable value.</summary>
		/// <returns>The created nullable value.</returns>
		/// <param name="nullable">The nullable value to copy.</param>
		Nullable<T>& operator=(const Nullable<T>& nullable)
		{
			if(this!=&nullable)
			{
				if(object)
				{
					delete object;
					object=0;
				}
				if(nullable.object)
				{
					object=new T(*nullable.object);
				}
			}
			return *this;
		}
		

		static bool Equals(const Nullable<T>& a, const Nullable<T>& b)
		{
			return
				a.object
				?b.object
					?*a.object==*b.object
					:false
				:b.object
					?false
					:true;
		}

		static int Compare(const Nullable<T>& a, const Nullable<T>& b)
		{
			return
				a.object
				?b.object
					?(*a.object==*b.object?0:*a.object<*b.object?-1:1)
					:1
				:b.object
					?-1
					:0;
		}

		bool operator==(const Nullable<T>& nullable)const
		{
			return Equals(*this, nullable);
		}

		bool operator!=(const Nullable<T>& nullable)const
		{
			return !Equals(*this, nullable);
		}

		bool operator<(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)<0;
		}

		bool operator<=(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)<=0;
		}

		bool operator>(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)>0;
		}

		bool operator>=(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)>=0;
		}

		/// <summary>Convert the nullable value to a bool value.</summary>
		/// <returns>Returns true if it is not null.</returns>
		operator bool()const
		{
			return object!=0;
		}
		
		/// <summary>Unbox the value. This operation will cause an access violation of it is null.</summary>
		/// <returns>The original value.</returns>
		const T& Value()const
		{
			return *object;
		}
	};

	template<typename T, size_t minSize>
	union BinaryRetriver
	{
		T t;
		char binary[sizeof(T)>minSize?sizeof(T):minSize];
	};

/***********************************************************************
配置
***********************************************************************/

	/// <summary>Get the index type of a value for containers.</summary>
	/// <typeparam name="T">Type of the value.</typeparam>
	template<typename T>
	struct KeyType
	{
	public:
		/// <summary>The index type of a value for containers.</summary>
		typedef T Type;

		/// <summary>Convert a value to its index type.</summary>
		/// <returns>The corresponding index value.</returns>
		/// <param name="value">The value.</param>
		static T GetKeyValue(const T& value)
		{
			return value;
		}
	};

	/// <summary>Test is a type a Plain-Old-Data type for containers.</summary>
	/// <typeparam name="T">The type to test.</typeparam>
	template<typename T>
	struct POD
	{
		/// <summary>Returns true if the type is a Plain-Old-Data type.</summary>
		static const bool Result=false;
	};

	template<>struct POD<bool>{static const bool Result=true;};
	template<>struct POD<int8_t>{static const bool Result=true;};
	template<>struct POD<vuint8_t>{static const bool Result=true;};
	template<>struct POD<int16_t>{static const bool Result=true;};
	template<>struct POD<vuint16_t>{static const bool Result=true;};
	template<>struct POD<int32_t>{static const bool Result=true;};
	template<>struct POD<vuint32_t>{static const bool Result=true;};
	template<>struct POD<int64_t>{static const bool Result=true;};
	template<>struct POD<vuint64_t>{static const bool Result=true;};
	template<>struct POD<char>{static const bool Result=true;};
	template<>struct POD<wchar_t>{static const bool Result=true;};
	template<typename T>struct POD<T*>{static const bool Result=true;};
	template<typename T>struct POD<T&>{static const bool Result=true;};
	template<typename T, typename C>struct POD<T C::*>{static const bool Result=true;};
	template<typename T, int _Size>struct POD<T[_Size]>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<const T>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<volatile T>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<const volatile T>{static const bool Result=POD<T>::Result;};

/***********************************************************************
时间
***********************************************************************/

	/// <summary>A type representing the combination of date and time.</summary>
	struct DateTime
	{
		int				year;
		int				month;
		int				dayOfWeek;
		int				day;
		int				hour;
		int				minute;
		int				second;
		int				milliseconds;

		vuint64_t			totalMilliseconds;
		
		// in gcc, this will be mktime(t) * 1000 + gettimeofday().tv_usec / 1000
		vuint64_t			filetime;

		/// <summary>Get the current local time.</summary>
		/// <returns>The current local time.</returns>
		static DateTime		LocalTime();

		/// <summary>Get the current UTC time.</summary>
		/// <returns>The current UTC time.</returns>
		static DateTime		UtcTime();

		/// <summary>Create a date time value.</summary>
		/// <returns>The created date time value.</returns>
		/// <param name="_year">The year.</param>
		/// <param name="_month">The month.</param>
		/// <param name="_day">The day.</param>
		/// <param name="_hour">The hour.</param>
		/// <param name="_minute">The minute.</param>
		/// <param name="_second">The second.</param>
		/// <param name="_milliseconds">The millisecond.</param>
		static DateTime		FromDateTime(int _year, int _month, int _day, int _hour=0, int _minute=0, int _second=0, int _milliseconds=0);
	
		static DateTime		FromFileTime(vuint64_t filetime);

		/// <summary>Create an empty date time value.</summary>
		DateTime();

		/// <summary>Convert the UTC time to the local time.</summary>
		/// <returns>The UTC time.</returns>
		DateTime			ToLocalTime();
		/// <summary>Convert the local time to the UTC time.</summary>
		/// <returns>The local time.</returns>
		DateTime			ToUtcTime();
		/// <summary>Move forward.</summary>
		/// <returns>The moved time.</returns>
		/// <param name="milliseconds">The delta in milliseconds.</param>
		DateTime			Forward(vuint64_t milliseconds);
		/// <summary>Move Backward.</summary>
		/// <returns>The moved time.</returns>
		/// <param name="milliseconds">The delta in milliseconds.</param>
		DateTime			Backward(vuint64_t milliseconds);

		bool operator==(const DateTime& value)const { return filetime==value.filetime; }
		bool operator!=(const DateTime& value)const { return filetime!=value.filetime; }
		bool operator<(const DateTime& value)const { return filetime<value.filetime; }
		bool operator<=(const DateTime& value)const { return filetime<=value.filetime; }
		bool operator>(const DateTime& value)const { return filetime>value.filetime; }
		bool operator>=(const DateTime& value)const { return filetime>=value.filetime; }
	};

/***********************************************************************
接口
***********************************************************************/
	
	/// <summary>Base type of all interfaces. All interface types are encouraged to be virtual inherited.</summary>
	class Interface : private NotCopyable
	{
	public:
		virtual ~Interface();
	};

/***********************************************************************
类型萃取
***********************************************************************/

	struct YesType{};
	struct NoType{};

	template<typename T, typename YesOrNo>
	struct AcceptType
	{
	};

	template<typename T>
	struct AcceptType<T, YesType>
	{
		typedef T Type;
	};

	template<typename YesOrNo>
	struct AcceptValue
	{
		static const bool Result=false;
	};

	template<>
	struct AcceptValue<YesType>
	{
		static const bool Result=true;
	};


	template<typename T, typename U>
	struct AcceptAlways
	{
		typedef T Type;
	};

#endif
