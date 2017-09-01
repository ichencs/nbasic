/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Data Structure::Smart Pointer

Classes:
	Ptr<T>							：智能指针

Brief:
	引用陈子涵所写的智能指针，相对于源代码删减C++11标准支持部分。
***********************************************************************/

#pragma once
#include "basic.h"


// #define INCRC(x)	(_InterlockedIncrement((volatile long*)(x)))		//++
// #define DECRC(x)	(_InterlockedDecrement((volatile long*)(x)))		//--

// struct YesType {};


/***********************************************************************
ReferenceCounterOperator
***********************************************************************/

	/// <summary>The strategy to get the pointer to the reference counter from an object. If you get the same pointer multiple times from the same object by calling [M:vl.ReferenceCounterOperator`2.CreateCounter], than it is safe to convert a object pointer to a [T:vl.Ptr`1]. Currently for reflectable C++ types which inherit from [T:vl.reflection.DescriptableObject] it is yet. For others it is no.</summary>
	/// <typeparam name="T">The type of the object.</typeparam>
	/// <typeparam name="Enabled">[T:vl.Ptr`1] will always use [T:vl.YesType] as the second type parameter. This parameter is useful when you want to do partial specialization in the SFINAE way.</typeparam>
	template<typename T, typename Enabled=YesType>
	struct ReferenceCounterOperator
	{
		/// <summary>Create a pointer to the reference counter from an object.</summary>
		/// <returns>The pointer to the reference counter.</returns>
		/// <param name="reference">The object.</param>
		static __forceinline volatile int* CreateCounter(T* reference)
		{
			return new int(0);
		}

		/// <summary>Destroy a pointer to the reference counter from an object.</summary>
		/// <param name="counter">The pointer to the reference counter.</param>
		/// <param name="reference">The object.</param>
		static __forceinline void DeleteReference(volatile int* counter, void* reference)
		{
			delete counter;
			delete (T*)reference;
		}
	};

/***********************************************************************
Ptr
***********************************************************************/

	/// <summary>A smart pointer. It is always safe to convert a pointer to an object to a smart pointer once. If you do it multiple times, it may be wrong due to different implementation of [T:vl.ReferenceCounterOperator`2]. In case of wrong, disposing the smart pointer will cause an access violation.</summary>
	/// <typeparam name="T">The type of the object.</typeparam>
	template<typename T>
	class Ptr
	{
		 template<typename X>
		 friend class Ptr;
	protected:
		typedef void		(*Destructor)(volatile int*, void*);

		volatile int*		counter;
		T*					reference;
		void*				originalReference;
		Destructor			originalDestructor;

		void Inc()
		{
			if(counter)
			{
				INCRC(counter);
			}
		}

		void Dec(bool deleteIfZero = true)
		{
			if(counter)
			{
				if(DECRC(counter)==0)
				{
					if (deleteIfZero)
					{
						originalDestructor(counter, originalReference);
					}
					counter= 0;
					reference= 0;
					originalReference= 0;
					originalDestructor= 0;
				}
			}
		}

		volatile int* Counter()const
		{
			return counter;
		}

		Ptr(volatile int* _counter, T* _reference, void* _originalReference, Destructor _originalDestructor)
			:counter(_counter)
			,reference(_reference)
			,originalReference(_originalReference)
			,originalDestructor(_originalDestructor)
		{
			Inc();
		}
	public:

		/// <summary>Create a null pointer.</summary>
		Ptr()
			:counter(0)
			,reference(0)
			,originalReference(0)
			,originalDestructor(0)
		{
		}
		
		/// <summary>Convert a pointer to an object to a smart pointer.</summary>
		/// <param name="pointer">The pointer to the object.</param>
		Ptr(T* pointer)
			:counter(0)
			,reference(0)
			,originalReference(0)
			,originalDestructor(0)
		{
			if(pointer)
			{
				counter=ReferenceCounterOperator<T>::CreateCounter(pointer);
				reference=pointer;
				originalReference=pointer;
				originalDestructor=&ReferenceCounterOperator<T>::DeleteReference;
				Inc();
			}
		}
		
		/// <summary>Copy a smart pointer.</summary>
		/// <param name="pointer">The smart pointer to copy.</param>
		Ptr(const Ptr<T>& pointer)
			:counter(pointer.counter)
			,reference(pointer.reference)
			,originalReference(pointer.originalReference)
			,originalDestructor(pointer.originalDestructor)
		{
			Inc();
		}
		
		//&& ：C++ 11 右值引用
		/// <summary>Move a smart pointer.</summary>
		/// <param name="pointer">The smart pointer to Move.</param>
// 		Ptr(Ptr<T>&& pointer)
// 			:counter(pointer.counter)
// 			,reference(pointer.reference)
// 			,originalReference(pointer.originalReference)
// 			,originalDestructor(pointer.originalDestructor)
// 		{
// 			pointer.counter=0;
// 			pointer.reference=0;
// 			pointer.originalReference=0;
// 			pointer.originalDestructor=0;
// 		}
		
		/// <summary>Cast a smart pointer.</summary>
		/// <typeparam name="C">The type of the object before casting.</typeparam>
		/// <param name="pointer">The smart pointer to cast.</param>
		template<typename C>
		Ptr(const Ptr<C>& pointer)
			:counter(0)
			,reference(0)
			,originalReference(0)
			,originalDestructor(0)
		{
			T* converted=pointer.Obj();
			if(converted)
			{
				counter=pointer.Counter();
				reference=converted;
				originalReference=pointer.originalReference;
				originalDestructor=pointer.originalDestructor;
				Inc();
			}
		}

		~Ptr()
		{
			Dec();
		}
		
		/// <summary>Detach the contained object from this smart pointer.</summary>
		/// <returns>The detached object. Returns null if this smart pointer is empty.</returns>
		T* Detach()
		{
			auto detached = reference;
			Dec(false);
			return detached;
		}
		
		/// <summary>Cast a smart pointer.</summary>
		/// <typeparam name="C">The type of the object after casting.</typeparam>
		/// <returns>The casted smart pointer. Returns null if failed.</returns>
		template<typename C>
		Ptr<C> Cast()const
		{
			C* converted=dynamic_cast<C*>(reference);
			return Ptr<C>((converted?counter:0), converted, originalReference, originalDestructor);
		}
		
		/// <summary>Convert a pointer to an object to a smart pointer.</summary>
		/// <returns>The converted smart pointer.</returns>
		/// <param name="pointer">The pointer to the object.</param>
		Ptr<T>& operator=(T* pointer)
		{
			Dec();
			if(pointer)
			{
				counter = ReferenceCounterOperator<T>::CreateCounter(pointer);
				reference = pointer;
				originalReference = pointer;
				originalDestructor = &ReferenceCounterOperator<T>::DeleteReference;
				Inc();
			}
			else
			{
				counter=0;
				reference=0;
				originalReference=0;
				originalDestructor=0;
			}
			return *this;
		}
		
		/// <summary>Copy a smart pointer.</summary>
		/// <returns>The copied smart pointer.</returns>
		/// <param name="pointer">The smart pointer to copy.</param>
		Ptr<T>& operator=(const Ptr<T>& pointer)
		{
			if(this!=&pointer)
			{
				Dec();
				counter=pointer.counter;
				reference=pointer.reference;
				originalReference=pointer.originalReference;
				originalDestructor=pointer.originalDestructor;
				Inc();
			}
			return *this;
		}
		
		//&& ：C++ 11 右值引用
		/// <summary>Move a smart pointer.</summary>
		/// <returns>The moved smart pointer.</returns>
		/// <param name="pointer">The smart pointer to Move.</param>
// 		Ptr<T>& operator=(Ptr<T>&& pointer)
// 		{
// 			if(this!=&pointer)
// 			{
// 				Dec();
// 				counter=pointer.counter;
// 				reference=pointer.reference;
// 				originalReference=pointer.originalReference;
// 				originalDestructor=pointer.originalDestructor;
// 				
// 				pointer.counter=0;
// 				pointer.reference=0;
// 				pointer.originalReference=0;
// 				pointer.originalDestructor=0;
// 			}
// 			return *this;
// 		}
		
		/// <summary>Cast a smart pointer.</summary>
		/// <typeparam name="C">The type of the object before casting.</typeparam>
		/// <returns>The smart pointer after casting.</returns>
		/// <param name="pointer">The smart pointer to cast.</param>
		template<typename C>
		Ptr<T>& operator=(const Ptr<C>& pointer)
		{
			T* converted=pointer.Obj();
			Dec();
			if(converted)
			{
				counter=pointer.counter;
				reference=converted;
				originalReference=pointer.originalReference;
				originalDestructor=pointer.originalDestructor;
				Inc();
			}
			else
			{
				counter=0;
				reference=0;
				originalReference=0;
				originalDestructor=0;
			}
			return *this;
		}

		bool operator==(const T* pointer)const
		{
			return reference==pointer;
		}

		bool operator!=(const T* pointer)const
		{
			return reference!=pointer;
		}

		bool operator>(const T* pointer)const
		{
			return reference>pointer;
		}

		bool operator>=(const T* pointer)const
		{
			return reference>=pointer;
		}

		bool operator<(const T* pointer)const
		{
			return reference<pointer;
		}

		bool operator<=(const T* pointer)const
		{
			return reference<=pointer;
		}

		bool operator==(const Ptr<T>& pointer)const
		{
			return reference==pointer.reference;
		}

		bool operator!=(const Ptr<T>& pointer)const
		{
			return reference!=pointer.reference;
		}

		bool operator>(const Ptr<T>& pointer)const
		{
			return reference>pointer.reference;
		}

		bool operator>=(const Ptr<T>& pointer)const
		{
			return reference>=pointer.reference;
		}

		bool operator<(const Ptr<T>& pointer)const
		{
			return reference<pointer.reference;
		}

		bool operator<=(const Ptr<T>& pointer)const
		{
			return reference<=pointer.reference;
		}

		/// <summary>Test if it is a null pointer.</summary>
		/// <returns>Returns true if it is not null.</returns>
		operator bool()const
		{
			return reference!=0;
		}

		/// <summary>Get the pointer to the object.</summary>
		/// <returns>The pointer to the object.</returns>
		T* Obj()const
		{
			return reference;
		}
		
		/// <summary>Get the pointer to the object.</summary>
		/// <returns>The pointer to the object.</returns>
		T* operator->()const
		{
			return reference;
		}
	};





