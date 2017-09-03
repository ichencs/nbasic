/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Data Structure::List

Classes:
	ListStore<T,PODType>				���б��洢�����㷨
	ListBase<T,K>						���б�����
	Array<T,K>							������
	List<T,K>							���б�
	SortedList<T,K>						�������б�
***********************************************************************/

#ifndef VCZH_COLLECTIONS_LIST
#define VCZH_COLLECTIONS_LIST

#include <string.h>
#include "Interfaces.h"

// #ifdef VCZH_CHECK_MEMORY_LEAKS_NEW
// #undef new
// #endif

#include <new>
#include "..\Basic.h"


/***********************************************************************
Memory Management
***********************************************************************/

		template<typename T, bool PODType>
		class ListStore abstract : public Object
		{
		};

		template<typename T>
		class ListStore<T, false> abstract : public Object
		{
		protected:
			static void InitializeItemsByDefault(void* dst, nint count)
			{
				T* ds = (T*)dst;

				for (nint i = 0; i < count; i++)
				{
					new(&ds[i])T();
				}
			}

			static void InitializeItemsByCopy(void* dst, void* src, nint count)
			{
				T* ds = (T*)dst;
				T* ss = (T*)src;

				for (nint i = 0; i < count; i++)
				{
 					new(&ds[i])T(ss[i]);
				}
			}

			static void MoveItemsInTheSameBuffer(void* dst, void* src, nint count)
			{
				T* ds = (T*)dst;
				T* ss = (T*)src;

				if (ds < ss)
				{
					for (nint i = 0; i < count; i++)
					{
						ds[i] = /*MoveValue*/(ss[i]);
					}
				}
				else if (ds > ss)
				{
					for (nint i = count - 1; i >= 0; i--)
					{
						ds[i] = /*MoveValue*/(ss[i]);
					}
				}
			}

			static void ReleaseItems(void* dst, nint count)
			{
				T* ds = (T*)dst;

				for (nint i = 0; i < count; i++)
				{
					ds[i].~T();
				}
			}
			
			static void* AllocateBuffer(nint size)
			{
				if (size <= 0) return 0;
				return (void*)malloc(sizeof(T) * size);
			}

			static void DeallocateBuffer(void* buffer)
			{
				if (buffer == 0)return;
				free(buffer);
			}
		public:
		};

		template<typename T>
		class ListStore<T, true> abstract : public Object
		{
		protected:
			static void InitializeItemsByDefault(void* dst, nint count)
			{
			}

			static void InitializeItemsByMove(void* dst, void* src, nint count)
			{
				if (count > 0)
				{
					memcpy(dst, src, sizeof(T) * count);
				}
			}

			static void InitializeItemsByCopy(void* dst, void* src, nint count)
			{
				if (count > 0)
				{
					memcpy(dst, src, sizeof(T) * count);
				}
			}

			static void MoveItemsInTheSameBuffer(void* dst, void* src, nint count)
			{
				if (count > 0)
				{
					memmove(dst, src, sizeof(T) * count);
				}
			}

			/// <summary>
			/// POD�ͣ������ͷ�
			/// </summary>
			/// <param name="dst"></param>
			/// <param name="count"></param>
			/// <returns></returns>
			static void ReleaseItems(void* dst, nint count)
			{
			}

			static void* AllocateBuffer(nint size)
			{
				if (size <= 0) return 0;
				return (void*)malloc(sizeof(T) * size);
			}

			static void DeallocateBuffer(void* buffer)
			{
				if (buffer == 0) return;
				free(buffer);
			}
		public:
		};

/***********************************************************************
ArrayBase
***********************************************************************/

		/// <summary>Base type of all linear container.</summary>
		/// <typeparam name="T">Type of elements.</typeparam>
		template<typename T>
		class ArrayBase abstract : public ListStore<T, POD<T>::Result>, public virtual IEnumerable<T>
		{
		protected:
			class Enumerator : public Object, public virtual IEnumerator<T>
			{
			private:
				const ArrayBase<T>*				container;
				nint							index;

			public:
				Enumerator(const ArrayBase<T>* _container, nint _index = -1)
				{
					container = _container;
					index = _index;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(container, index);
				}

				const T& Current()const
				{
					return container->Get(index);
				}

				nint Index()const
				{
					return index;
				}

				bool Next()
				{
					index++;
					return index >= 0 && index < container->Count();
				}

				void Reset()
				{
					index = -1;
				}
			};

			void*					buffer /*= nullptr*/;
			nint					count /*= 0*/;

			static void* AddressOf(void* bufferOfTs, nint index)
			{
				return (void*)((char*)bufferOfTs + sizeof(T) * index);
			}

			const T& ItemOf(nint index)const
			{
				return *(const T*)AddressOf(buffer, index);
			}

			T& ItemOf(nint index)
			{
				return *(T*)AddressOf(buffer, index);
			}
		public:
			ArrayBase()
				:buffer(0),count(0)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(this);
			}

			/// <summary>Get the number of elements in the container.</summary>
			/// <returns>The number of elements.</returns>
			nint Count()const
			{
				return count;
			}

			/// <summary>Get the reference to the specified element.</summary>
			/// <returns>The reference to the specified element.</returns>
			/// <param name="index">The index of the element.</param>
			const T& Get(nint index)const
			{
				CHECK_ERROR(index >= 0 && index < this->count, L"ArrayBase<T, K>::Get(nint)#Argument index not in range.");
				return ItemOf(index);
			}

			/// <summary>Get the reference to the specified element.</summary>
			/// <returns>The reference to the specified element.</returns>
			/// <param name="index">The index of the element.</param>
			const T& operator[](nint index)const
			{
				CHECK_ERROR(index >= 0 && index < this->count, L"ArrayBase<T, K>::operator[](nint)#Argument index not in range.");
				return ItemOf(index);
			}
		};

/***********************************************************************
Array
***********************************************************************/

		/// <summary>Array.</summary>
		/// <typeparam name="T">Type of elements.</typeparam>
		/// <typeparam name="K">Type of the key type of elements.</typeparam>
		template<typename T, typename K = typename KeyType<T>::Type>
		class NArray : public ArrayBase<T>
		{
		public:
			/// <summary>Create an array.</summary>
			/// <param name="size">The size of the array.</param>
			NArray(nint size = 0)
			{
				this->buffer = this->AllocateBuffer(size);
				this->InitializeItemsByDefault(this->buffer, size);
				this->count = size;
			}

			/// <summary>Create an array.</summary>
			/// <param name="_buffer">Pointer to an array to copy.</param>
			/// <param name="size">The size of the array.</param>
			NArray(const T* _buffer, nint size)
			{
				this->buffer = this->AllocateBuffer(size);
				this->InitializeItemsByCopy(this->buffer, (void*)_buffer, size);
				this->count = size;
			}

			~NArray()
			{
				this->ReleaseItems(this->buffer, this->count);
				this->DeallocateBuffer(this->buffer);
			}

			/// <summary>Test does the array contain an item or not.</summary>
			/// <returns>Returns true if the array contains the specified item.</returns>
			/// <param name="item">The item to test.</param>
			bool Contains(const K& item)const
			{
				return IndexOf(item) != -1;
			}

			/// <summary>Get the position of an item in this array.</summary>
			/// <returns>Returns the position. Returns -1 if not exists</returns>
			/// <param name="item">The item to find.</param>
			nint IndexOf(const K& item)const
			{
				for (nint i = 0; i < this->count; i++)
				{
					if (this->ItemOf(i) == item)
					{
						return i;
					}
				}
				return -1;
			}

			/// <summary>Replace an item.</summary>
			/// <param name="index">The position of the item.</param>
			/// <param name="item">The new item to put into the array.</param>
			void Set(nint index, const T& item)
			{
				CHECK_ERROR(index >= 0 && index < this->count, L"Array<T, K>::Set(nint)#Argument index not in range.");
				this->ItemOf(index) = item;
			}

			/// <summary>Get the reference to the specified element.</summary>
			/// <returns>The reference to the specified element.</returns>
			/// <param name="index">The index of the element.</param>
			using ArrayBase<T>::operator[];
			T& operator[](nint index)
			{
				CHECK_ERROR(index >= 0 && index < this->count, L"Array<T, K>::operator[](nint)#Argument index not in range.");
				return this->ItemOf(index);
			}

			/// <summary>Change the size of the array.</summary>
			/// <param name="size">The new size of the array.</param>
			void Resize(nint size)
			{
				void* newBuffer = this->AllocateBuffer(size);
				if (size < this->count)
				{
					this->InitializeItemsByCopy(this->AddressOf(newBuffer, 0), this->AddressOf(this->buffer, 0), size);
				}
				else
				{
					this->InitializeItemsByCopy(this->AddressOf(newBuffer, 0), this->AddressOf(this->buffer, 0), this->count);
					this->InitializeItemsByDefault(this->AddressOf(newBuffer, this->count), size - this->count);
				}

				this->ReleaseItems(this->buffer, this->count);
				this->DeallocateBuffer(this->buffer);
				this->buffer = newBuffer;
				this->count = size;
			}
		};

/***********************************************************************
ListBase
***********************************************************************/

		/// <summary>Base type for a list container.</summary>
		/// <typeparam name="T">Type of elements.</typeparam>
		/// <typeparam name="K">Type of the key type of elements.</typeparam>
		template<typename T, typename K = typename KeyType<T>::Type>
		class ListBase abstract : public ArrayBase<T>
		{
		protected:
			nint					capacity /*= 0*/;
			bool					lessMemoryMode /*= false*/;

			nint CalculateCapacity(nint expected)
			{
				nint result = capacity;
				while (result < expected)
				{
					result = result * 5 / 4 + 1;
				}
				return result;
			}

			void MakeRoom(nint index, nint _count, bool& uninitialized)
			{
				nint newCount = this->count + _count;
				if (newCount > capacity)
				{
					nint newCapacity = CalculateCapacity(newCount);
					void* newBuffer = this->AllocateBuffer(newCapacity);
					this->InitializeItemsByCopy(this->AddressOf(newBuffer, 0), this->AddressOf(this->buffer, 0), index);
					this->InitializeItemsByCopy(this->AddressOf(newBuffer, index + _count), this->AddressOf(this->buffer, index), this->count - index);
					this->ReleaseItems(this->buffer, this->count);
					this->DeallocateBuffer(this->buffer);
					this->capacity = newCapacity;
					this->buffer = newBuffer;
					uninitialized = true;
				}
				else if (index >= this->count)
				{
					uninitialized = true;
				}
				else if (this->count - index < _count)
				{
					this->InitializeItemsByCopy(this->AddressOf(this->buffer, index + _count), this->AddressOf(this->buffer, index), this->count - index);
					this->ReleaseItems(this->AddressOf(this->buffer, index), _count - (this->count - index));
					uninitialized = true;
				}
				else
				{
					this->InitializeItemsByCopy(this->AddressOf(this->buffer, this->count), this->AddressOf(this->buffer, this->count - _count), _count);
					this->MoveItemsInTheSameBuffer(this->AddressOf(this->buffer, index + _count), this->AddressOf(this->buffer, index), this->count - index - _count);
					uninitialized = false;
				}
				this->count = newCount;
			}

			void ReleaseUnnecessaryBuffer(nint previousCount)
			{
				if (this->buffer && this->count < previousCount)
				{
					this->ReleaseItems(this->AddressOf(this->buffer, this->count), previousCount - this->count);
				}
				if (this->lessMemoryMode && this->count <= this->capacity / 2)
				{
					nint newCapacity = capacity * 5 / 8;
					if (this->count < newCapacity)
					{
						void* newBuffer = this->AllocateBuffer(newCapacity);
						this->InitializeItemsByCopy(this->AddressOf(newBuffer, 0), this->AddressOf(this->buffer, 0), this->count);
						this->ReleaseItems(this->buffer, this->count);
						this->DeallocateBuffer(this->buffer);
						this->capacity = newCapacity;
						this->buffer = newBuffer;
					}
				}
			}
		public:
			ListBase()
				:capacity(0),lessMemoryMode(false)
			{
			}

			~ListBase()
			{
				this->ReleaseItems(this->buffer, this->count);
				this->DeallocateBuffer(this->buffer);
			}

			/// <summary>Set a preference of using memory.</summary>
			/// <param name="mode">Set to true (by default) to let the container efficiently reduce memory usage when necessary.</param>
			void SetLessMemoryMode(bool mode)
			{
				this->lessMemoryMode = mode;
			}

			/// <summary>Remove an element.</summary>
			/// <returns>Returns true if the element is removed.</returns>
			/// <param name="index">The index of the element to remove.</param>
			bool RemoveAt(nint index)
			{
				nint previousCount = this->count;
				CHECK_ERROR(index >= 0 && index < this->count, L"ListBase<T, K>::RemoveAt(nint)#Argument index not in range.");
				this->MoveItemsInTheSameBuffer(this->AddressOf(this->buffer, index), this->AddressOf(this->buffer, index + 1), this->count - index - 1);
				this->count--;
				ReleaseUnnecessaryBuffer(previousCount);
				return true;
			}

			/// <summary>Remove elements.</summary>
			/// <returns>Returns true if the element is removed.</returns>
			/// <param name="index">The index of the first element to remove.</param>
			/// <param name="_count">The number of elements to remove.</param>
			bool RemoveRange(nint index, nint _count)
			{
				nint previousCount = this->count;
				CHECK_ERROR(index >= 0 && index <= this->count, L"ListBase<T, K>::RemoveRange(nint, nint)#Argument index not in range.");
				CHECK_ERROR(index + _count >= 0 && index + _count <= this->count, L"ListBase<T,K>::RemoveRange(nint, nint)#Argument _count not in range.");
				this->MoveItemsInTheSameBuffer(this->AddressOf(this->buffer, index), this->AddressOf(this->buffer, index + _count), this->count - index - _count);
				this->count -= _count;
				ReleaseUnnecessaryBuffer(previousCount);
				return true;
			}

			/// <summary>Remove all elements.</summary>
			/// <returns>Returns true if all elements are removed.</returns>
			bool Clear()
			{
				nint previousCount = this->count;
				this->count = 0;
				if (lessMemoryMode)
				{
					this->capacity = 0;
					this->ReleaseItems(this->buffer, this->count);
					this->DeallocateBuffer(this->buffer);
					this->buffer = 0;
				}
				else
				{
					ReleaseUnnecessaryBuffer(previousCount);
				}
				return true;
			}
		};

/***********************************************************************
List
***********************************************************************/

		/// <summary>List.</summary>
		/// <typeparam name="T">Type of elements.</typeparam>
		/// <typeparam name="K">Type of the key type of elements.</typeparam>
		template<typename T, typename K = typename KeyType<T>::Type>
		class NList : public ListBase<T, K>
		{
		public:
			/// <summary>Create a list.</summary>
			NList()
			{
			}

			/// <summary>Test does the list contain an item or not.</summary>
			/// <returns>Returns true if the list contains the specified item.</returns>
			/// <param name="item">The item to test.</param>
			bool Contains(const K& item)const
			{
				return IndexOf(item) != -1;
			}

			/// <summary>Get the position of an item in this list.</summary>
			/// <returns>Returns the position. Returns -1 if not exists</returns>
			/// <param name="item">The item to find.</param>
			nint IndexOf(const K& item)const
			{
				for (nint i = 0; i < this->count; i++)
				{
					if (this->ItemOf(i) == item)
					{
						return i;
					}
				}
				return -1;
			}

			/// <summary>Add an item at the end of the list.</summary>
			/// <returns>The index of the added item.</returns>
			/// <param name="item">The item to add.</param>
			nint Add(const T& item)
			{
				return Insert(this->count, item);
			}

			/// <summary>Add an item at the specified position.</summary>
			/// <returns>The index of the added item.</returns>
			/// <param name="index">The position of the item to add.</param>
			/// <param name="item">The item to add.</param>
			nint Insert(nint index, const T& item)
			{
				CHECK_ERROR(index >= 0 && index <= this->count, L"List<T, K>::Insert(nint, const T&)#Argument index not in range.");
				bool uninitialized = false;
				this->MakeRoom(index, 1, uninitialized);
				if (uninitialized)
				{
  					new(&this->ItemOf(index))T(item);
				}
				else
				{
					this->ItemOf(index) = item;
				}
				return index;
			}

			/// <summary>Remove an item.</summary>
			/// <returns>Returns true if the item is removed.</returns>
			/// <param name="item">The item to remove.</param>
			bool Remove(const K& item)
			{
				nint index = IndexOf(item);
				if (index >= 0 && index < this->count)
				{
					this->RemoveAt(index);
					return true;
				}
				else
				{
					return false;
				}
			}

			/// <summary>Replace an item.</summary>
			/// <returns>Returns true if this operation succeeded.</returns>
			/// <param name="index">The position of the item.</param>
			/// <param name="item">The new item to put into the array.</param>
			bool Set(nint index, const T& item)
			{
				CHECK_ERROR(index >= 0 && index < this->count, L"List<T, K>::Set(nint)#Argument index not in range.");
				this->ItemOf(index) = item;
				return true;
			}

			/// <summary>Get the reference to the specified element.</summary>
			/// <returns>The reference to the specified element.</returns>
			/// <param name="index">The index of the element.</param>
			using ListBase<T, K>::operator[];
			T& operator[](nint index)
			{
				CHECK_ERROR(index >= 0 && index < this->count, L"List<T, K>::operator[](nint)#Argument index not in range.");
				return this->ItemOf(index);
			}
		};

/***********************************************************************
SortedList
***********************************************************************/

		/// <summary>List that keeps everything in order.������ʱ�����ֲ��ң�</summary>
		/// <typeparam name="T">Type of elements.</typeparam>
		/// <typeparam name="K">Type of the key type of elements.</typeparam>
		template<typename T, typename K = typename KeyType<T>::Type>
		class NSortedList : public ListBase<T, K>
		{
		protected:

			/// <summary>Get the position of an item in this list.</summary>
			/// <typeparam name="Key">Type of the item to find.</typeparam>
			/// <returns>Returns the position. Returns -1 if not exists</returns>
			/// <param name="item">The item to find.</param>
			/// <param name="index">Returns the last index.</param>
			template<typename Key>
			nint IndexOfInternal(const Key& item, nint& index)const
			{
				nint start = 0;
				nint end = this->count - 1;
				index = -1;
				while (start <= end)
				{
					index = start + (end - start) / 2;
					if (this->ItemOf(index) == item)
					{
						return index;
					}
					else if (this->ItemOf(index) > item)
					{
						end = index - 1;
					}
					else
					{
						start = index + 1;
					}
				}
				return -1;
			}

			nint Insert(nint index, const T& item)
			{
				bool uninitialized = false;
				this->MakeRoom(index, 1, uninitialized);
				if (uninitialized)
				{
					new(&this->ItemOf(index))T(item);
				}
				else
				{
					this->ItemOf(index) = item;
				}
				return index;
			}
		public:
			/// <summary>Create a list.</summary>
			NSortedList()
			{
			}

			/// <summary>Test does the list contain an item or not.</summary>
			/// <returns>Returns true if the list contains the specified item.</returns>
			/// <param name="item">The item to test.</param>
			bool Contains(const K& item)const
			{
				return IndexOf(item) != -1;
			}

			/// <summary>Get the position of an item in this list.</summary>
			/// <returns>Returns the position. Returns -1 if not exists</returns>
			/// <param name="item">The item to find.</param>
			nint IndexOf(const K& item)const
			{
				nint outputIndex = -1;
				return IndexOfInternal<K>(item, outputIndex);
			}

			/// <summary>Add an item at a correct position to keep everying in order.</summary>
			/// <returns>The index of the added item.</returns>
			/// <param name="item">The item to add.</param>
			nint Add(const T& item)
			{
				if (ArrayBase<T>::count == 0)
				{
					return Insert(0, item);
				}
				else
				{
					nint outputIndex = -1;
					IndexOfInternal<T>(item, outputIndex);
					CHECK_ERROR(outputIndex >= 0 && outputIndex < this->count, L"SortedList<T, K>::Add(const T&)#Internal error, index not in range.");
					if (this->ItemOf(outputIndex) < item)
					{
						outputIndex++;
					}
					return Insert(outputIndex, item);
				}
			}

			/// <summary>Remove an item.</summary>
			/// <returns>Returns true if the item is removed.</returns>
			/// <param name="item">The item to remove.</param>
			bool Remove(const K& item)
			{
				nint index = IndexOf(item);
				if (index >= 0 && index < ArrayBase<T>::count)
				{
					this->RemoveAt(index);
					return true;
				}
				else
				{
					return false;
				}
			}
		};

/***********************************************************************
Special Containers
***********************************************************************/

		template<typename T>
		class PushOnlyAllocator : public Object, private NotCopyable
		{
		protected:
			nint							blockSize;
			nint							allocatedSize;
			NList<T*>						blocks;

		public:
			PushOnlyAllocator(nint _blockSize = 65536)
				:blockSize(_blockSize)
				, allocatedSize(0)
			{
			}

			~PushOnlyAllocator()
			{
				for (nint i = 0; i < blocks.Count(); i++)
				{
					delete[] blocks[i];
				}
			}

			T* Get(nint index)
			{
				if (index >= allocatedSize)
				{
					return 0;
				}
				nint row = index / blockSize;
				nint column = index % blockSize;
				return &blocks[row][column];
			}

			T* Create()
			{
				if (allocatedSize == blocks.Count()*blockSize)
				{
					blocks.Add(new T[blockSize]);
				}
				nint index = allocatedSize++;
				return Get(index);
			}
		};

			struct TreeNode
			{
				TreeNode*					nodes[4];
			};

			template<nint Index = 4>
			struct Accessor
			{
				static __forceinline void* Get(TreeNode* root, nuint8_t index)
				{
					if (!root)
					{
						return 0;
					}
					nint fragmentIndex = (index >> (2 * (Index - 1))) % 4;
					TreeNode* fragmentRoot = root->nodes[fragmentIndex];
					return fragmentRoot ? Accessor<Index - 1>::Get(fragmentRoot, index) : 0;
				}

				static __forceinline void Set(TreeNode*& root, nuint8_t index, void* value, PushOnlyAllocator<TreeNode>& allocator)
				{
					if (!root)
					{
						root = allocator.Create();
						memset(root->nodes, 0, sizeof(root->nodes));
					}
					nint fragmentIndex = (index >> (2 * (Index - 1))) % 4;
					TreeNode*& fragmentRoot = root->nodes[fragmentIndex];
					Accessor<Index - 1>::Set(fragmentRoot, index, value, allocator);
				}
			};

			template<>
			struct Accessor<0>
			{
				static __forceinline void* Get(TreeNode* root, nuint8_t index)
				{
					return (void*)root;
				}

				static __forceinline void Set(TreeNode*& root, nuint8_t index, void* value, PushOnlyAllocator<TreeNode>& allocator)
				{
					((void*&)root) = value;
				}
			};
		

		template<typename T>
		class ByteObjectMap : public Object, private NotCopyable
		{
		public:
			typedef PushOnlyAllocator<TreeNode>			Allocator;
		protected:
			TreeNode*			root;

		public:
			ByteObjectMap()
				:root(0)
			{
			}

			~ByteObjectMap()
			{
			}

			T* Get(nuint8_t index)
			{
				return (T*)Accessor<>::Get(root, index);
			}

			void Set(nuint8_t index, T* value, Allocator& allocator)
			{
				Accessor<>::Set(root, index, value, allocator);
			}
		};

/***********************************************************************
Random Access
***********************************************************************/

			template<typename T, typename K>
			struct RandomAccessable<NArray<T, K>>
			{
				static const bool							CanRead = true;
				static const bool							CanResize = true;
			};

			template<typename T, typename K>
			struct RandomAccessable<NList<T, K>>
			{
				static const bool							CanRead = true;
				static const bool							CanResize = false;
			};

			template<typename T, typename K>
			struct RandomAccessable<NSortedList<T, K>>
			{
				static const bool							CanRead = true;
				static const bool							CanResize = false;
			};


#endif