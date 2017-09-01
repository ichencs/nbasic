// ntest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <memory>
#include <iostream>
#include "TestBasic.h"
#include <afxwin.h>
#include <cmath>

using namespace std;


 int main()
 {
// 	 int num[] = {1 _ 2 _ 3};
	 List<int> list;
	 list.Add(1);
	 list.Add(2);
	 list.Add(1);
	 list.Add(2);
	 for (int i = 0; i < list.Count(); i++)
	 {
		 cout<< list.Get(i) <<endl;
	 }
	 cout<< endl <<endl;

// 	 throw("123");

	 SortedList<int> slist;
	 slist.Add(9);
	 slist.Add(8);
	 slist.Add(8);
	 slist.Add(9);
	 slist.Add(7);

	 for (int i = 0; i < slist.Count(); i++)
	 {
		 cout<< slist.Get(i) <<endl;
	 }

	 int k = 0;
	 double d = 1.0;

	 Dictionary<int,double> dict;
	 dict.Add(k++ ,d++);
	 dict.Add(k++ ,d++);
	 dict.Add(k++ ,d++);
//  	 dict.Add(k-- ,d++);
	 dict.Add(k++ ,d++);
	 dict.Add(k++ ,d++);
	 dict.Add(k++ ,d++);
//  	 dict.Add(k-- ,d++);
//  	 dict.Add(k-- ,d++);
	 cout<< endl <<endl;

	 for (int i = 0; i < dict.Keys().Count(); i++)
	 {
		 int key = dict.Keys().Get(i);
		 cout<<"key:" << key<<dict.Get(key)<<endl;
	 }

	 system("pause");
     return 0;
 }

