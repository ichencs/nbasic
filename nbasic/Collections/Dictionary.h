/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Data Structure::Dictionary

Classes:
	Dictionary<KT, VT, KK, VK>					£ºÓ³Éä
	Group<KT, VT, KK, VK>						£º¶àÖØÓ³Éä
***********************************************************************/

#ifndef VCZH_COLLECTIONS_DICTIONARY
#define VCZH_COLLECTIONS_DICTIONARY

#include "NList.h"

		/// <summary>Dictionary. ×Öµä.Ë³ÐòµÄ</summary>
		/// <typeparam name="KT">Type of keys.</typeparam>
		/// <typeparam name="VT">Type of values.</typeparam>
		/// <typeparam name="KK">Type of the key type of keys.</typeparam>
		/// <typeparam name="VK">Type of the key type of values.</typeparam>
	template<
		typename KT,
		typename VT,
		typename KK=typename KeyType<KT>::Type, 
		typename VK=typename KeyType<VT>::Type
	>
	class Dictionary : public Object, public virtual IEnumerable<NPair<KT, VT>>
	{
	public:
		typedef NSortedList<KT, KK>			KeyContainer;
		typedef NList<VT, VK>				ValueContainer;
	protected:
		class Enumerator : public Object, public virtual IEnumerator<NPair<KT, VT>>
		{
		private:
			const Dictionary<KT, VT, KK, VK>*	container;
			nint								index;
			NPair<KT, VT>						current;

			void UpdateCurrent()
			{
				if(index<container->Count())
				{
					current.key=container->Keys().Get(index);
					current.value=container->Values().Get(index);
				}
			}
		public:
			Enumerator(const Dictionary<KT, VT, KK, VK>* _container, nint _index=-1)
			{
				container=_container;
				index=_index;
			}
				
			IEnumerator<NPair<KT, VT>>* Clone()const
			{
				return new Enumerator(container, index);
			}

			const NPair<KT, VT>& Current()const
			{
				return current;
			}

			nint Index()const
			{
				return index;
			}

			bool Next()
			{
				index++;
				UpdateCurrent();
				return index>=0 && index<container->Count();
			}

			void Reset()
			{
				index=-1;
				UpdateCurrent();
			}
		};

		KeyContainer						keys;
		ValueContainer						values;
	public:
		/// <summary>Create a dictionary.</summary>
		Dictionary()
		{
		}

		IEnumerator<NPair<KT, VT>>* CreateEnumerator()const
		{
			return new Enumerator(this);
		}
			
		/// <summary>Set a preference of using memory.</summary>
		/// <param name="mode">Set to true (by default) to let the container efficiently reduce memory usage when necessary.</param>
		void SetLessMemoryMode(bool mode)
		{
			keys.SetLessMemoryMode(mode);
			values.SetLessMemoryMode(mode);
		}

		/// <summary>Get all keys.</summary>
		/// <returns>All keys.</returns>
		const KeyContainer& Keys()const
		{
			return keys;
		}
			
		/// <summary>Get all values.</summary>
		/// <returns>All values.</returns>
		const ValueContainer& Values()const
		{
			return values;
		}

		/// <summary>Get the number of keys.</summary>
		/// <returns>The number of keys.</returns>
		nint Count()const
		{
			return keys.Count();
		}

		/// <summary>Get the reference to the value associated with a key.</summary>
		/// <returns>The reference to the value.</returns>
		/// <param name="key">The key to find.</param>
		const VT& Get(const KK& key)const
		{
			return values.Get(keys.IndexOf(key));
		}
			
		/// <summary>Get the reference to the value associated with a key.</summary>
		/// <returns>The reference to the value.</returns>
		/// <param name="key">The key to find.</param>
		const VT& operator[](const KK& key)const
		{
			return values.Get(keys.IndexOf(key));
		}
			
		/// <summary>Replace the value associated with a key.</summary>
		/// <returns>Returns true if the value is replaced.</returns>
		/// <param name="key">The key to find.</param>
		/// <param name="value">The key to replace.</param>
		bool Set(const KT& key, const VT& value)
		{
			nint index=keys.IndexOf(KeyType<KT>::GetKeyValue(key));
			if(index==-1)
			{
				index=keys.Add(key);
				values.Insert(index, value);
			}
			else
			{
				values[index]=value;
			}
			return true;
		}

		/// <summary>Add a key with an associated value. Exception will raise if the key already exists.</summary>
		/// <returns>Returns true if the pair is added.</returns>
		/// <param name="value">The pair of key and value.</param>
		bool Add(const NPair<KT, VT>& value)
		{
			return Add(value.key, value.value);
		}
			
		/// <summary>Add a key with an associated value. Exception will raise if the key already exists.</summary>
		/// <returns>Returns true if the pair is added.</returns>
		/// <param name="key">The key.</param>
		/// <param name="value">The value.</param>
		bool Add(const KT& key, const VT& value)
		{
			CHECK_ERROR(!keys.Contains(KeyType<KT>::GetKeyValue(key)), L"Dictionary<KT, KK, ValueContainer, VT, VK>::Add(const KT&, const VT&)#Key already exists.");
			nint index = keys.Add(key);
			values.Insert(index, value);
			return true;
		}

		/// <summary>Remove a key with the associated value.</summary>
		/// <returns>Returns true if the key and the value is removed.</returns>
		/// <param name="key">The key.</param>
		bool Remove(const KK& key)
		{
			nint index=keys.IndexOf(key);
			if(index!=-1)
			{
				keys.RemoveAt(index);
				values.RemoveAt(index);
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>Remove everything.</summary>
		/// <returns>Returns true if all keys and values are removed.</returns>
		bool Clear()
		{
			keys.Clear();
			values.Clear();
			return true;
		}
	};
		
		/// <summary>Group, which is similar to an dictionary, but a group can associate multiple values with a key.</summary>
		/// <typeparam name="KT">Type of keys.</typeparam>
		/// <typeparam name="VT">Type of values.</typeparam>
		/// <typeparam name="KK">Type of the key type of keys.</typeparam>
		/// <typeparam name="VK">Type of the key type of values.</typeparam>
		template<
			typename KT,
			typename VT,
			typename KK=typename KeyType<KT>::Type,
			typename VK=typename KeyType<VT>::Type
		>
		class Group : public Object, public virtual IEnumerable<NPair<KT, VT>>
		{
			typedef NSortedList<KT, KK>		KeyContainer;
			typedef NList<VT, VK>			ValueContainer;
		protected:
			class Enumerator : public Object, public virtual IEnumerator<NPair<KT, VT>>
			{
			private:
				const Group<KT, VT, KK, VK>*		container;
				nint								keyIndex;
				nint								valueIndex;
				NPair<KT, VT>						current;

				void UpdateCurrent()
				{
					if(keyIndex<container->Count())
					{
						const ValueContainer& values=container->GetByIndex(keyIndex);
						if(valueIndex<values.Count())
						{
							current.key=container->Keys().Get(keyIndex);
							current.value=values.Get(valueIndex);
						}
					}
				}
			public:
				Enumerator(const Group<KT, VT, KK, VK>* _container, nint _keyIndex=-1, nint _valueIndex=-1)
				{
					container=_container;
					keyIndex=_keyIndex;
					valueIndex=_valueIndex;
				}
				
				IEnumerator<NPair<KT, VT>>* Clone()const
				{
					return new Enumerator(container, keyIndex, valueIndex);
				}

				const NPair<KT, VT>& Current()const
				{
					return current;
				}

				nint Index()const
				{
					if(0<=keyIndex && keyIndex<container->Count())
					{
						nint index=0;
						for(nint i=0;i<keyIndex;i++)
						{
							index+=container->GetByIndex(i).Count();
						}
						return index+valueIndex;
					}
					else
					{
						return -1;
					}
				}

				bool Next()
				{
					if(keyIndex==-1)
					{
						keyIndex=0;
					}
					while(keyIndex<container->Count())
					{
						valueIndex++;
						const ValueContainer& values=container->GetByIndex(keyIndex);
						if(valueIndex<values.Count())
						{
							UpdateCurrent();
							return true;
						}
						else
						{
							keyIndex++;
							valueIndex=-1;
						}
					}
					return false;
				}

				void Reset()
				{
					keyIndex=-1;
					valueIndex=-1;
					UpdateCurrent();
				}
			};

			KeyContainer					keys;
			NList<ValueContainer*>			values;
		public:
			Group()
			{
			}

			~Group()
			{
				Clear();
			}

			IEnumerator<NPair<KT, VT>>* CreateEnumerator()const
			{
				return new Enumerator(this);
			}
			
			/// <summary>Get all keys.</summary>
			/// <returns>All keys.</returns>
			const KeyContainer& Keys()const
			{
				return keys;
			}
			
			/// <summary>Get the number of keys.</summary>
			/// <returns>The number of keys.</returns>
			nint Count()const
			{
				return keys.Count();
			}
			
			/// <summary>Get all values associated with a key.</summary>
			/// <returns>All values.</returns>
			/// <param name="key">The key to find.</param>
			const ValueContainer& Get(const KK& key)const
			{
				return *values.Get(keys.IndexOf(key));
			}
			
			/// <summary>Get all values associated with a key.</summary>
			/// <returns>All values.</returns>
			/// <param name="index">The position of a the key.</param>
			const ValueContainer& GetByIndex(nint index)const
			{
				return *values.Get(index);
			}
			
			/// <summary>Get all values associated with a key.</summary>
			/// <returns>All values.</returns>
			/// <param name="key">The key to find.</param>
			const ValueContainer& operator[](const KK& key)const
			{
				return *values.Get(keys.IndexOf(key));
			}

			/// <summary>Test if a key exists in the group or not.</summary>
			/// <returns>Returns true if the key exists.</returns>
			/// <param name="key">The key to find.</param>
			bool Contains(const KK& key)const
			{
				return keys.Contains(key);
			}
			
			/// <summary>Test if a key exists with an associated value in the group or not.</summary>
			/// <returns>Returns true if the key exists with an associated value.</returns>
			/// <param name="key">The key to find.</param>
			/// <param name="value">The value to find.</param>
			bool Contains(const KK& key, const VK& value)const
			{
				nint index=keys.IndexOf(key);
				if(index!=-1)
				{
					return values.Get(index)->Contains(value);
				}
				else
				{
					return false;
				}
			}
			
			/// <summary>Add a key with an associated value. If the key already exists, the value will be associated with the key with other values.</summary>
			/// <returns>Returns true if the pair is added.</returns>
			/// <param name="value">The pair of key and value.</param>
			bool Add(const NPair<KT, VT>& value)
			{
				return Add(value.key, value.value);
			}
			
			/// <summary>Add a key with an associated value. If the key already exists, the value will be associated with the key with other values.</summary>
			/// <returns>Returns true if the pair is added.</returns>
			/// <param name="key">The key.</param>
			/// <param name="value">The value.</param>
			bool Add(const KT& key, const VT& value)
			{
				ValueContainer* target=0;
				nint index=keys.IndexOf(KeyType<KT>::GetKeyValue(key));
				if(index==-1)
				{
					target=new ValueContainer;
					values.Insert(keys.Add(key), target);
				}
				else
				{
					target=values[index];
				}
				target->Add(value);
				return true;
			}
			
			/// <summary>Remove a key with all associated values.</summary>
			/// <returns>Returns true if the key and all associated values are removed.</returns>
			/// <param name="key">The key.</param>
			bool Remove(const KK& key)
			{
				nint index=keys.IndexOf(key);
				if(index!=-1)
				{
					keys.RemoveAt(index);
					NList<VT, VK>* target=values[index];
					values.RemoveAt(index);
					delete target;
					return true;
				}
				else
				{
					return false;
				}
			}
			
			/// <summary>Remove a key with the associated values.</summary>
			/// <returns>Returns true if the key and the associated values are removed. If there are multiple values associated with the key, only the value will be removed.</returns>
			/// <param name="key">The key.</param>
			/// <param name="value">The value.</param>
			bool Remove(const KK& key, const VK& value)
			{
				nint index=keys.IndexOf(key);
				if(index!=-1)
				{
					NList<VT, VK>* target=values[index];
					target->Remove(value);
					if(target->Count()==0)
					{
						keys.RemoveAt(index);
						values.RemoveAt(index);
						delete target;
					}
					return true;
				}
				else
				{
					return false;
				}
			}
			
			/// <summary>Remove everything.</summary>
			/// <returns>Returns true if all keys and values are removed.</returns>
			bool Clear()
			{
				for(nint i=0;i<values.Count();i++)
				{
					delete values[i];
				}
				keys.Clear();
				values.Clear();
				return true;
			}
		};

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

		template<
			typename TKey,
			typename TValueFirst,
			typename TValueSecond,
			typename TDiscardFirst,		// TKey * [TValueFirst] -> void
			typename TDiscardSecond,	// TKey * [TValueSecond] -> void
			typename TAccept			// TKey * [TValueFirst] * [TValueSecond] -> void
		>
			void GroupInnerJoin(
				const Group<TKey, TValueFirst>& first,
				const Group<TKey, TValueSecond>& second,
				const TDiscardFirst& discardFirst,
				const TDiscardSecond& discardSecond,
				const TAccept& accept
			)
		{
			nint firstIndex = 0;
			nint secondIndex = 0;
			nint firstCount = first.Keys().Count();
			nint secondCount = second.Keys().Count();
			while (true)
			{
				if (firstIndex < firstCount)
				{
					auto firstKey = first.Keys()[firstIndex];
					const NList<TValueFirst>& firstValues = first.GetByIndex(firstIndex);

					if (secondIndex < secondCount)
					{
						auto secondKey = second.Keys()[secondIndex];
						const NList<TValueSecond>& secondValues = second.GetByIndex(secondIndex);

						if (firstKey < secondKey)
						{
							discardFirst(firstKey, firstValues);
							firstIndex++;
						}
						else if (firstKey > secondKey)
						{
							discardSecond(secondKey, secondValues);
							secondIndex++;
						}
						else
						{
							accept(firstKey, firstValues, secondValues);
							firstIndex++;
							secondIndex++;
						}
					}
					else
					{
						discardFirst(firstKey, firstValues);
						firstIndex++;
					}
				}
				else
				{
					if (secondIndex < secondCount)
					{
						auto secondKey = second.Keys()[secondIndex];
						const NList<TValueSecond>& secondValues = second.GetByIndex(secondIndex);

						discardSecond(secondKey, secondValues);
						secondIndex++;
					}
					else
					{
						break;
					}
				}
			}
		}

/***********************************************************************
Ëæ»ú·ÃÎÊ
***********************************************************************/
			template<typename KT, typename VT, typename KK, typename VK>
			struct RandomAccessable<Dictionary<KT, VT, KK, VK>>
			{
				static const bool							CanRead = true;
				static const bool							CanResize = false;
			};
		
			template<typename KT, typename VT, typename KK, typename VK>
			struct RandomAccess<Dictionary<KT, VT, KK, VK>>
			{
				static nint GetCount(const Dictionary<KT, VT, KK, VK>& t)
				{
					return t.Count();
				}

				static NPair<KT, VT> GetValue(const Dictionary<KT, VT, KK, VK>& t, nint index)
				{
					return NPair<KT, VT>(t.Keys().Get(index), t.Values().Get(index));
				}

				static void AppendValue(Dictionary<KT, VT, KK, VK>& t, const NPair<KT, VT>& value)
				{
					t.Set(value.key, value.value);
				}
			};

#endif