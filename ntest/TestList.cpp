#include "stdafx.h"
#include "UnitTest.h"

#define _ ,
#define CHECK_EMPTY_LIST(CONTAINER) TestReadonlyList(CONTAINER, 0, 0)
#define CHECK_LIST_ITEMS(CONTAINER,ITEMS)do{nint __items__[]=ITEMS;TestReadonlyList(CONTAINER,__items__, sizeof(__items__)/sizeof(*__items__));}while(0)
#define CHECK_EMPTY_DICTIONARY(CONTAINER) TestReadonlyDictionary(CONTAINER, 0, 0, 0);
#define CHECK_DICTIONARY_ITEMS(CONTAINER,KEYS,VALUES)do{nint __keys__[]=KEYS;nint __values__[]=VALUES;TestReadonlyDictionary(CONTAINER, __keys__, __values__, sizeof(__keys__)/sizeof(*__keys__));}while(0)
#define CHECK_EMPTY_GROUP(CONTAINER) TestReadonlyGroup(CONTAINER, 0, 0, 0, 0);
#define CHECK_GROUP_ITEMS(CONTAINER,KEYS,VALUES,COUNTS)do{nint __keys__[]=KEYS;nint __values__[]=VALUES;nint __counts__[]=COUNTS;TestReadonlyGroup(CONTAINER, __keys__, __values__, __counts__, sizeof(__keys__)/sizeof(*__keys__));}while(0)


template<typename TList>
void TestReadonlyList(const TList& list, nint* items, nint count)
{
	TEST_ASSERT(list.Count() == count);
	IEnumerator<nint>* enumerator = list.CreateEnumerator();
	
	for (nint i = 0; i < count; i++)
	{
		TEST_ASSERT(list.Contains(items[i]));
		TEST_ASSERT(list.Get(i) == items[i]);
		TEST_ASSERT(enumerator->Next());
		TEST_ASSERT(enumerator->Current() == items[i]);
		TEST_ASSERT(enumerator->Index() == i);
	}
	
	TEST_ASSERT(enumerator->Next() == false);
	delete enumerator;
}

template<typename T>
void CompareEnumerable(const IEnumerable<T>& dst, const IEnumerable<T>& src)
{
	IEnumerator<T>* dstEnum = dst.CreateEnumerator();
	IEnumerator<T>* srcEnum = src.CreateEnumerator();
	
	while (dstEnum->Next())
	{
		TEST_ASSERT(srcEnum->Next());
		TEST_ASSERT(dstEnum->Current() == srcEnum->Current());
		TEST_ASSERT(dstEnum->Index() == srcEnum->Index());
	}
	
	TEST_ASSERT(dstEnum->Next() == false);
	TEST_ASSERT(srcEnum->Next() == false);
	delete dstEnum;
	delete srcEnum;
}

void TestArray(NArray<nint>& arr)
{
	arr.Resize(0);
	CHECK_EMPTY_LIST(arr);
	arr.Resize(10);
	
	for (nint i = 0; i < 10; i++)
	{
		arr.Set(i, i);
	}
	
	CHECK_LIST_ITEMS(arr, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 });
	arr.Resize(15);
	
	for (nint i = 10; i < 15; i++)
	{
		arr.Set(i, i * 2);
	}
	
	CHECK_LIST_ITEMS(arr, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 20 _ 22 _ 24 _ 26 _ 28 });
	arr.Resize(5);
	CHECK_LIST_ITEMS(arr, { 0 _ 1 _ 2 _ 3 _ 4 });
}

template<typename TCollection>
void TestCollectionWithIncreasingItems(TCollection& collection)
{
	collection.Clear();
	CHECK_EMPTY_LIST(collection);
	collection.Add(0);
	CHECK_LIST_ITEMS(collection, { 0 });
	
	for (nint i = 1; i < 10; i++)
	{
		collection.Add(i);
	}
	
	CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 });
	collection.RemoveAt(2);
	CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 });
	collection.RemoveRange(3, 4);
	CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 3 _ 8 _ 9 });
	TEST_ASSERT(collection.Remove(3) == true);
	CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 8 _ 9 });
	TEST_ASSERT(collection.Remove(9) == true);
	CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 8 });
	TEST_ASSERT(collection.Remove(0) == true);
	CHECK_LIST_ITEMS(collection, { 1 _ 8 });
	
	for (nint i = 0; i < 10; i++)
	{
		if (i != 1 && i != 8)
		{
			TEST_ASSERT(collection.Remove(i) == false);
			CHECK_LIST_ITEMS(collection, { 1 _ 8 });
		}
	}
	
	collection.Clear();
	CHECK_EMPTY_LIST(collection);
}

template<typename TCollection>
void TestSortedCollection(TCollection& collection)
{
	nint items[] = { 7, 1, 12, 2, 8, 3, 11, 4, 9, 5, 13, 6, 10 };
	collection.Clear();
	
	for (nint i = 0; i < sizeof(items) / sizeof(*items); i++)
	{
		collection.Add(items[i]);
	}
	
	CHECK_LIST_ITEMS(collection, { 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });
	TEST_ASSERT(collection.Remove(7) == true);
	CHECK_LIST_ITEMS(collection, { 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });
	TEST_ASSERT(collection.Remove(1) == true);
	CHECK_LIST_ITEMS(collection, { 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });
	TEST_ASSERT(collection.Remove(12) == true);
	CHECK_LIST_ITEMS(collection, { 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 13 });
	TEST_ASSERT(collection.Remove(2) == true);
	CHECK_LIST_ITEMS(collection, { 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 13 });
	TEST_ASSERT(collection.Remove(8) == true);
	CHECK_LIST_ITEMS(collection, { 3 _ 4 _ 5 _ 6 _ 9 _ 10 _ 11 _ 13 });
	TEST_ASSERT(collection.Remove(3) == true);
	CHECK_LIST_ITEMS(collection, { 4 _ 5 _ 6 _ 9 _ 10 _ 11 _ 13 });
	
	for (nint i = 0; i < sizeof(items) / sizeof(*items); i++)
	{
		collection.Add(items[i]);
	}
	
	CHECK_LIST_ITEMS(collection, { 1 _ 2 _ 3 _ 4 _ 4 _ 5 _ 5 _ 6 _ 6 _ 7 _ 8 _ 9 _ 9 _ 10 _ 10 _ 11 _ 11 _ 12 _ 13 _ 13 });
	collection.Clear();
	CHECK_EMPTY_LIST(collection);
}

template<typename TList>
void TestNormalList(TList& list)
{
	list.Clear();
	
	for (nint i = 0; i < 10; i++)
	{
		list.Insert(i / 2, i);
	}
	
	CHECK_LIST_ITEMS(list, { 1 _ 3 _ 5 _ 7 _ 9 _ 8 _ 6 _ 4 _ 2 _ 0 });
	
	for (nint i = 0; i < 10; i++)
	{
		list.Set(i, 9 - i);
	}
	
	CHECK_LIST_ITEMS(list, { 9 _ 8 _ 7 _ 6 _ 5 _ 4 _ 3 _ 2 _ 1 _ 0 });
	
	for (nint i = 0; i < 10; i++)
	{
		list.Set(i, i * 2);
	}
	
	CHECK_LIST_ITEMS(list, { 0 _ 2 _ 4 _ 6 _ 8 _ 10 _ 12 _ 14 _ 16 _ 18 });
}

void TestReadonlyDictionary(const NDictionary<nint, nint>& dictionary, nint* keys, nint* values, nint count)
{
	TEST_ASSERT(dictionary.Count() == count);
	TestReadonlyList(dictionary.Keys(), keys, count);
	TestReadonlyList(dictionary.Values(), values, count);
	
	for (nint i = 0; i < count; i++)
	{
		TEST_ASSERT(dictionary.Get(keys[i]) == values[i]);
	}
	
	IEnumerator<NPair<nint, nint>>* enumerator = dictionary.CreateEnumerator();
	
	for (nint i = 0; i < count; i++)
	{
		NPair<nint, nint> pair(keys[i], values[i]);
		TEST_ASSERT(enumerator->Next());
		TEST_ASSERT(enumerator->Current() == pair);
		TEST_ASSERT(enumerator->Index() == i);
	}
	
	TEST_ASSERT(enumerator->Next() == false);
	delete enumerator;
}

void TestSortedDictionary(NDictionary<nint, nint>& dictionary)
{
	dictionary.Clear();
	CHECK_EMPTY_DICTIONARY(dictionary);
	dictionary.Add(1, 1);
	dictionary.Add(2, 4);
	dictionary.Add(3, 9);
	dictionary.Add(4, 16);
	CHECK_DICTIONARY_ITEMS(dictionary, { 1 _ 2 _ 3 _ 4 }, { 1 _ 4 _ 9 _ 16 });
	dictionary.Set(1, -1);
	dictionary.Set(2, -4);
	dictionary.Set(3, -9);
	dictionary.Set(4, -16);
	dictionary.Set(5, -25);
	CHECK_DICTIONARY_ITEMS(dictionary, { 1 _ 2 _ 3 _ 4 _ 5 }, { -1 _ - 4 _ - 9 _ - 16 _ - 25 });
	dictionary.Remove(4);
	dictionary.Remove(5);
	dictionary.Remove(6);
	CHECK_DICTIONARY_ITEMS(dictionary, { 1 _ 2 _ 3 }, { -1 _ - 4 _ - 9 });
	dictionary.Clear();
	CHECK_EMPTY_DICTIONARY(dictionary);
}

void TestReadonlyGroup(const NGroup<nint, nint>& group, nint* keys, nint* values, nint* counts, nint count)
{
	TEST_ASSERT(group.Count() == count);
	TestReadonlyList(group.Keys(), keys, count);
	nint offset = 0;
	
	for (nint i = 0; i < count; i++)
	{
		TEST_ASSERT(group.Contains(keys[i]) == true);
		TestReadonlyList(group.Get(keys[i]), values + offset, counts[i]);
		TestReadonlyList(group[keys[i]], values + offset, counts[i]);
		TestReadonlyList(group.GetByIndex(i), values + offset, counts[i]);
		
		for (nint j = 0; j < counts[i]; j++)
		{
			TEST_ASSERT(group.Contains(keys[i], values[offset + j]));
		}
		
		offset += counts[i];
	}
	
	IEnumerator<NPair<nint, nint>>* enumerator = group.CreateEnumerator();
	nint keyIndex = 0;
	nint valueIndex = 0;
	nint index = 0;
	
	while (keyIndex < count)
	{
		NPair<nint, nint> pair(keys[keyIndex], values[index]);
		TEST_ASSERT(enumerator->Next());
		TEST_ASSERT(enumerator->Current() == pair);
		TEST_ASSERT(enumerator->Index() == index);
		valueIndex++;
		index++;
		
		if (valueIndex == counts[keyIndex])
		{
			keyIndex++;
			valueIndex = 0;
		}
	}
	
	TEST_ASSERT(enumerator->Next() == false);
	delete enumerator;
}

void TestSortedGroup(NGroup<nint, nint>& group)
{
	group.Clear();
	CHECK_EMPTY_GROUP(group);
	
	for (nint i = 0; i < 20; i++)
	{
		group.Add(i / 5, i);
	}
	
	CHECK_GROUP_ITEMS(group, { 0 _ 1 _ 2 _ 3 }, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 }, { 5 _ 5 _ 5 _ 5 });
	group.Remove(1);
	CHECK_GROUP_ITEMS(group, { 0 _ 2 _ 3 }, { 0 _ 1 _ 2 _ 3 _ 4 _ 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 }, { 5 _ 5 _ 5 });
	
	for (nint i = 13; i < 18; i++)
	{
		group.Remove(i / 5, i);
	}
	
	CHECK_GROUP_ITEMS(group, { 0 _ 2 _ 3 }, { 0 _ 1 _ 2 _ 3 _ 4 _ 10 _ 11 _ 12 _ 18 _ 19 }, { 5 _ 3 _ 2 });
	group.Clear();
	CHECK_EMPTY_GROUP(group);
}

TEST_CASE(TestArray)
{
	NArray<nint> arr;
	TestArray(arr);
}

TEST_CASE(TestSortedList)
{
	NSortedList<nint> list;
	TestCollectionWithIncreasingItems(list);
	TestSortedCollection(list);
}

TEST_CASE(TestList)
{
	NList<nint> list;
	TestCollectionWithIncreasingItems(list);
	TestNormalList(list);
}

TEST_CASE(TestDictionary)
{
	NDictionary<nint, nint> dictionary;
	TestSortedDictionary(dictionary);
}

TEST_CASE(TestGroup)
{
	NGroup<nint, nint> group;
	TestSortedGroup(group);
}

TEST_CASE(TestListCopy)
{
	NArray<nint> arr;
	NList<nint> list;
	NSortedList<nint> sortedList;
	arr.Resize(5);
	
	for (nint i = 0; i < 5; i++)
	{
		arr[i] = i;
	}
	
	CopyFrom(list, arr);
	CopyFrom(sortedList, arr);
	CHECK_LIST_ITEMS(list, { 0 _ 1 _ 2 _ 3 _ 4 });
	CHECK_LIST_ITEMS(sortedList, { 0 _ 1 _ 2 _ 3 _ 4 });
	list.Clear();
	
	for (nint i = 10; i < 20; i++)
	{
		list.Add(i);
	}
	
	CopyFrom(arr, list);
	CopyFrom(sortedList, list);
	CHECK_LIST_ITEMS(arr, { 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 });
	CHECK_LIST_ITEMS(sortedList, { 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 });
	sortedList.Clear();
	
	for (nint i = 6; i < 9; i++)
	{
		sortedList.Add(i);
	}
	
	CopyFrom(arr, sortedList);
	CopyFrom(list, sortedList);
	CHECK_LIST_ITEMS(arr, { 6 _ 7 _ 8 });
	CHECK_LIST_ITEMS(list, { 6 _ 7 _ 8 });
	arr.Resize(5);
	
	for (nint i = 0; i < 5; i++)
	{
		arr[i] = i;
	}
	
	CopyFrom(list, (const IEnumerable<nint>&)arr);
	CopyFrom(sortedList, (const IEnumerable<nint>&)arr);
	CHECK_LIST_ITEMS(list, { 0 _ 1 _ 2 _ 3 _ 4 });
	CHECK_LIST_ITEMS(sortedList, { 0 _ 1 _ 2 _ 3 _ 4 });
	list.Clear();
	
	for (nint i = 10; i < 20; i++)
	{
		list.Add(i);
	}
	
	CopyFrom(arr, (const IEnumerable<nint>&)list);
	CopyFrom(sortedList, (const IEnumerable<nint>&)list);
	CHECK_LIST_ITEMS(arr, { 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 });
	CHECK_LIST_ITEMS(sortedList, { 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 });
	sortedList.Clear();
	
	for (nint i = 6; i < 9; i++)
	{
		sortedList.Add(i);
	}
	
	CopyFrom(arr, (const IEnumerable<nint>&)sortedList);
	CopyFrom(list, (const IEnumerable<nint>&)sortedList);
	CHECK_LIST_ITEMS(arr, { 6 _ 7 _ 8 });
	CHECK_LIST_ITEMS(list, { 6 _ 7 _ 8 });
}

TEST_CASE(TestListCopyIterator)
{
	NArray<nint> arr;
	NList<nint> list;
	NSortedList<nint> sortedList;
	nint numbers[] = { 1, 2, 3, 4, 5 };
	CopyFrom(arr, &numbers[0], sizeof(numbers) / sizeof(*numbers), false);
	CopyFrom(list, &numbers[0], sizeof(numbers) / sizeof(*numbers), false);
	CopyFrom(sortedList, &numbers[0], sizeof(numbers) / sizeof(*numbers), false);
	CHECK_LIST_ITEMS(arr, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(sortedList, { 1 _ 2 _ 3 _ 4 _ 5 });
	CopyFrom(arr, &numbers[0], sizeof(numbers) / sizeof(*numbers), false);
	CopyFrom(list, &numbers[0], sizeof(numbers) / sizeof(*numbers), false);
	CopyFrom(sortedList, &numbers[0], sizeof(numbers) / sizeof(*numbers), false);
	CHECK_LIST_ITEMS(arr, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(sortedList, { 1 _ 2 _ 3 _ 4 _ 5 });
	CopyFrom(arr, &numbers[0], sizeof(numbers) / sizeof(*numbers), true);
	CopyFrom(list, &numbers[0], sizeof(numbers) / sizeof(*numbers), true);
	CopyFrom(sortedList, &numbers[0], sizeof(numbers) / sizeof(*numbers), true);
	CHECK_LIST_ITEMS(arr, { 1 _ 2 _ 3 _ 4 _ 5 _ 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 _ 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(sortedList, { 1 _ 1 _ 2 _ 2 _ 3 _ 3 _ 4 _ 4 _ 5 _ 5 });
	CopyFrom(arr, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], false);
	CopyFrom(list, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], false);
	CopyFrom(sortedList, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], false);
	CHECK_LIST_ITEMS(arr, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(sortedList, { 1 _ 2 _ 3 _ 4 _ 5 });
	CopyFrom(arr, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], false);
	CopyFrom(list, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], false);
	CopyFrom(sortedList, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], false);
	CHECK_LIST_ITEMS(arr, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(sortedList, { 1 _ 2 _ 3 _ 4 _ 5 });
	CopyFrom(arr, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], true);
	CopyFrom(list, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], true);
	CopyFrom(sortedList, &numbers[0], &numbers[sizeof(numbers) / sizeof(*numbers)], true);
	CHECK_LIST_ITEMS(arr, { 1 _ 2 _ 3 _ 4 _ 5 _ 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 _ 1 _ 2 _ 3 _ 4 _ 5 });
	CHECK_LIST_ITEMS(sortedList, { 1 _ 1 _ 2 _ 2 _ 3 _ 3 _ 4 _ 4 _ 5 _ 5 });
}


// TEST_CASE(TestSelectOperation)
// {
// 	NList<nint> src;
// 	NList<nint> dst;
//
// 	for (nint i = 1; i <= 10; i++)
// 	{
// 		src.Add(i);
// 	}
//
// 	CopyFrom(dst, From(src).Select(Square).Select(Double));
// 	CHECK_LIST_ITEMS(dst, { 2 _ 8 _ 18 _ 32 _ 50 _ 72 _ 98 _ 128 _ 162 _ 200 });
// 	CompareEnumerable(dst, From(src).Select(Square).Select(Double));
// }

template<typename TList>
void TestABCDE(const TList& list, nint repeat)
{
	TEST_ASSERT(list.Count() == 5 * repeat);
	
	for (nint i = 0; i < 5 * repeat; i++)
	{
		TEST_ASSERT(list.Get(i) == L'a' + i % 5);
	}
}

TEST_CASE(TestCopyFromString)
{
	WString string;
	NList<wchar_t> list;
	NArray<wchar_t> arr;
	CopyFrom(list, WString(L"abcde"), true);
	TestABCDE(list, 1);
	CopyFrom(list, WString(L"abcde"), false);
	TestABCDE(list, 1);
	CopyFrom(list, WString(L"abcde"), true);
	TestABCDE(list, 2);
	CopyFrom(list, WString(L"abcde"), false);
	TestABCDE(list, 1);
	CopyFrom(arr, WString(L"abcde"), true);
	TestABCDE(arr, 1);
	CopyFrom(arr, WString(L"abcde"), false);
	TestABCDE(arr, 1);
	CopyFrom(arr, WString(L"abcde"), true);
	TestABCDE(arr, 2);
	CopyFrom(arr, WString(L"abcde"), false);
	TestABCDE(arr, 1);
	string = L"";
	CopyFrom(string, list, true);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, list, false);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, list, true);
	TEST_ASSERT(string == L"abcdeabcde");
	CopyFrom(string, list, false);
	TEST_ASSERT(string == L"abcde");
	string = L"";
	CopyFrom(string, arr, true);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, arr, false);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, arr, true);
	TEST_ASSERT(string == L"abcdeabcde");
	CopyFrom(string, arr, false);
	TEST_ASSERT(string == L"abcde");
	string = L"";
	CopyFrom(string, (IEnumerable<wchar_t>&)list, true);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)list, false);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)list, true);
	TEST_ASSERT(string == L"abcdeabcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)list, false);
	TEST_ASSERT(string == L"abcde");
	string = L"";
	CopyFrom(string, (IEnumerable<wchar_t>&)arr, true);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)arr, false);
	TEST_ASSERT(string == L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)arr, true);
	TEST_ASSERT(string == L"abcdeabcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)arr, false);
	TEST_ASSERT(string == L"abcde");
}

TEST_CASE(TestForEach)
{
	NList<nint> a;
	NList<nint> b;
	NList<nint> c;
	
	for (nint i = 0; i < 3; i++)
	{
		a.Add(i);
		b.Add(i);
	}
	
	FOREACH(nint, i, a)
	FOREACH(nint, j, b)
	{
		c.Add(i + j);
	}
	CHECK_LIST_ITEMS(c, { 0 _ 1 _ 2 _ 1 _ 2 _ 3 _ 2 _ 3 _ 4 });
}

TEST_CASE(TestForEachWithIndexer)
{
	NList<nint> a;
	
	for (nint i = 0; i < 10; i++)
	{
		a.Add(i * i);
	}
	
	NDictionary<nint, nint> b;
	FOREACH_INDEXER(nint, num, i, a)
	{
		b.Add(i, num);
	}
	CHECK_DICTIONARY_ITEMS(b, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 }, { 0 _ 1 _ 4 _ 9 _ 16 _ 25 _ 36 _ 49 _ 64 _ 81 });
}

nint Compare(nint a, nint b)
{
	return a - b;
}

TEST_CASE(TestOrderBy)
{
	nint numbers[] = { 7, 1, 12, 2, 8, 3, 11, 4, 9, 5, 13, 6, 10 };
	NList<nint> list;
	CopyFrom(list, From(numbers).OrderBy(Compare));
	CHECK_LIST_ITEMS(list, { 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });
}


