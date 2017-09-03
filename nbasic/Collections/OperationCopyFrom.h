/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONCOPYFROM
#define VCZH_COLLECTIONS_OPERATIONCOPYFROM

#include "Interfaces.h"



/***********************************************************************
ÈÝÆ÷¸´ÖÆ
***********************************************************************/


			template<typename Ds, typename Ss, bool DsRA, bool SsRA>
			struct CopyFromAlgorithm
			{
			};

			template<typename Ds, typename Ss>
			struct CopyFromAlgorithm<Ds, Ss, true, true>
			{
				static void Perform(Ds& ds, const Ss& ss, bool append)
				{
					int copyCount=RandomAccess<Ss>::GetCount(ss);
					int index=(append?RandomAccess<Ds>::GetCount(ds):0);
					int resizeCount=index+copyCount;
					RandomAccess<Ds>::SetCount(ds, resizeCount);
					for(int i=0;i<copyCount;i++)
					{
						RandomAccess<Ds>::SetValue(ds, index+i, RandomAccess<Ss>::GetValue(ss, i));
					}
				}
			};

			template<typename Ds, typename Ss>
			struct CopyFromAlgorithm<Ds, Ss, false, true>
			{
				static void Perform(Ds& ds, const Ss& ss, bool append)
				{
					if(!append)
					{
						ds.Clear();
					}
					nint copyCount=RandomAccess<Ss>::GetCount(ss);
					for(int i=0;i<copyCount;i++)
					{
						RandomAccess<Ds>::AppendValue(ds, RandomAccess<Ss>::GetValue(ss, i));
					}
				}
			};

			template<typename Ds, typename Ss>
			struct CopyFromAlgorithm<Ds, Ss, true, false>
			{
				static void Perform(Ds& ds, const Ss& ss, bool append)
				{
					Ptr<IEnumerator<typename Ss::ElementType>> enumerator;
					nint copyCount=0;

					enumerator=ss.CreateEnumerator();
					while(enumerator->Next())
					{
						copyCount++;
					}

					nint index=(append?RandomAccess<Ds>::GetCount(ds):0);
					nint resizeCount=index+copyCount;
					RandomAccess<Ds>::SetCount(ds, resizeCount);

					enumerator=ss.CreateEnumerator();
					while(enumerator->Next())
					{
						RandomAccess<Ds>::SetValue(ds, index++, enumerator->Current());
					}
				}
			};

			template<typename Ds, typename Ss>
			struct CopyFromAlgorithm<Ds, Ss, false, false>
			{
				static void Perform(Ds& ds, const Ss& ss, bool append)
				{
					if(!append)
					{
						ds.Clear();
					}
					Ptr<IEnumerator<typename Ss::ElementType>> enumerator=ss.CreateEnumerator();
					while(enumerator->Next())
					{
						RandomAccess<Ds>::AppendValue(ds, enumerator->Current());
					}
				}
			};

			template<typename T>
			struct Slice
			{
				const T*	items;
				int		count;
			};

			template<typename T>
			struct RandomAccessable<Slice<T>>
			{
				static const bool							CanRead = true;
				static const bool							CanResize = true;
			};
		
			template<typename T>
			struct RandomAccess<Slice<T>>
			{
				static int GetCount(const Slice<T>& t)
				{
					return t.count;
				}

				static const T& GetValue(const Slice<T>& t, int index)
				{
					return t.items[index];
				}
			};

		/// <summary>Copy containers.</summary>
		/// <typeparam name="Ds">Type of the destination container.</typeparam>
		/// <typeparam name="Ss">Type of the source container.</typeparam>
		/// <param name="ds">The destination container.</param>
		/// <param name="ss">The source container.</param>
		/// <param name="append">Set to false to delete everything in the destination container before copying.</param>
		template<typename Ds, typename Ss>
		void CopyFrom(Ds& ds, const Ss& ss, bool append=false)
		{
			CopyFromAlgorithm<Ds, Ss, RandomAccessable<Ds>::CanResize, RandomAccessable<Ss>::CanRead>::Perform(ds, ss, append);
		}
		
		/// <summary>Copy containers.</summary>
		/// <typeparam name="Ds">Type of the destination container.</typeparam>
		/// <typeparam name="S">Type of the elements in the source container.</typeparam>
		/// <param name="ds">The destination container.</param>
		/// <param name="buffer">Pointer to the source array.</param>
		/// <param name="count">The number of elements to copy.</param>
		/// <param name="append">Set to false to delete everything in the destination container before copying.</param>
		template<typename Ds, typename S>
		void CopyFrom(Ds& ds, const S* buffer, int count, bool append=false)
		{
			Slice<S> slice={buffer, count};
			CopyFrom(ds, slice, append);
		}
		
		/// <summary>Copy containers.</summary>
		/// <typeparam name="Ds">Type of the destination container.</typeparam>
		/// <typeparam name="S">Type of the elements in the source container.</typeparam>
		/// <param name="ds">The destination container.</param>
		/// <param name="begin">Pointer to the first element in the source array.</param>
		/// <param name="end">Pointer to the element after the last element in the source array.</param>
		/// <param name="append">Set to false to delete everything in the destination container before copying.</param>
		template<typename Ds, typename S>
		void CopyFrom(Ds& ds, const S* begin, const S* end, bool append=false)
		{
			Slice<S> slice={begin, end-begin};
			CopyFrom(ds, slice, append);
		}
	


#endif