/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Data Structure::Interfaces

Interfaces:
	IEnumerator<T>									��ö����
	IEnumerable<T>									����ö�ٶ���
***********************************************************************/

#ifndef VCZH_COLLECTIONS_INTERFACES
#define VCZH_COLLECTIONS_INTERFACES

#include "Basic.h"
#include "Pointer.h"
#include "Pair.h"


/***********************************************************************
�ӿ�
***********************************************************************/

		/// <summary>Enumerator.</summary>
		/// <typeparam name="T">Type of the elements in the enumerator.</typeparam>
		template<typename T>
		class IEnumerator : public virtual Interface
		{
		public:
			typedef T									ElementType;

			/// <summary>Copy the enumerator with the current state.</summary>
			/// <returns>The copied enumerator.</returns>
			virtual IEnumerator<T>*						Clone()const=0;
			/// <summary>Get the reference to the current element in the enumerator.</summary>
			/// <returns>The reference to the current element.</returns>
			virtual const T&							Current()const=0;
			/// <summary>Get the position of the current element in the enumerator.</summary>
			/// <returns>The position of the current element.</returns>
			virtual int								Index()const=0;
			/// <summary>Step forward.</summary>
			/// <returns>Returns false if the enumerator reaches the end.</returns>
			virtual bool								Next()=0;
			/// <summary>Reset the enumerator.</summary>
			virtual void								Reset()=0;

			virtual bool								Evaluated()const{return false;}
		};
		
		/// <summary>Enumerable.</summary>
		/// <typeparam name="T">Type of the elements in the enumerator.</typeparam>
		template<typename T>
		class IEnumerable : public virtual Interface
		{
		public:
			typedef T									ElementType;

			/// <summary>Create an enumerator. [M:vl.collections.IEnumerator`1.Next] should be called to get the first element.</summary>
			/// <returns>The enumerator.</returns>
			virtual IEnumerator<T>*						CreateEnumerator()const=0;
		};

/***********************************************************************
�����ȡ
***********************************************************************/

			template<typename T>
			struct RandomAccessable
			{
				static const bool							CanRead = false;
				static const bool							CanResize = false;
			};

			template<typename T>
			struct RandomAccess
			{
				static int GetCount(const T& t)
				{
					return t.Count();
				}

				static const typename T::ElementType& GetValue(const T& t, int index)
				{
					return t.Get(index);
				}

				static void SetCount(T& t, int count)
				{
					t.Resize(count);
				}

				static void SetValue(T& t, int index, const typename T::ElementType& value)
				{
					t.Set(index, value);
				}

				static void AppendValue(T& t, const typename T::ElementType& value)
				{
					t.Add(value);
				}
			};


#endif