#include "stdafx.h"
#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void map_intkey_to_stringvalue();
void map_stringkey_to_intvalue();
void pair_and_map_use_by_the_vector_1();
void pair_and_map_use_by_the_vector_2();

int main() {


	map_intkey_to_stringvalue();
	map_stringkey_to_intvalue();
	pair_and_map_use_by_the_vector_1();
	pair_and_map_use_by_the_vector_2();
	

	return 0;
}

void map_intkey_to_stringvalue()
{
	// greater 정렬 기준의 key:int, value:string 타입의 map 생성
	map<int, string, less<int>> m;

	m[5] = "five";
	m[3] = "three";
	m[8] = "eight";
	m[4] = "four";
	m[1] = "one";
	m[7] = "seven";
	m[9] = "nine";

	/*
	less : 첫번째 인자가 두번째 인자보다 작으면 true 반환 (bool)
	greater : 첫번째 인자가 두번째 인자보다 크면 true 반환 (bool)
	*/
	map<int, string, less<int>>::iterator iter;
	for (iter = m.begin(); iter != m.end(); ++iter)
		cout << "(" << iter->first << ',' << iter->second << ") ";
	cout << endl;


	// key가 5인 요소 찾기
	iter = m.find(5);
	if (iter != m.end())
		cout << "key 5에 매핑된 value : " << iter->second << endl;

	// key가 5인 요소의 첫번째 요소 / 마지막의 다음 요소 찾기
	map<int, string>::iterator lower_iter;
	map<int, string>::iterator upper_iter;
	lower_iter = m.lower_bound(5);
	upper_iter = m.upper_bound(5);

	cout << "lower_iter : " << lower_iter->second << endl;
	cout << "upper_iter : " << upper_iter->second << endl;

	// key가 5인 요소의 범위  찾기
	pair<map<int, string>::iterator, map<int, string>::iterator> iter_pair;
	iter_pair = m.equal_range(5);

	for (iter = iter_pair.first; iter != iter_pair.second; ++iter)
		cout << "(" << iter->first << ',' << iter->second << ") ";
	cout << endl;
}

void map_stringkey_to_intvalue()
{
	map<string, int, greater<string>> _m;

	_m["five"] = 5;
	_m["three"] = 3;
	_m["eight"] = 8;
	_m["four"] = 4;
	_m["one"] = 1;
	_m["seven"] = 7;
	_m["nine"] = 9;

	/*
	less : 첫번째 인자가 두번째 인자보다 작으면 true 반환 (bool)
	greater : 첫번째 인자가 두번째 인자보다 크면 true 반환 (bool)
	*/
	map<string, int, greater<string>>::iterator _iter;
	for (_iter = _m.begin(); _iter != _m.end(); ++_iter)
		cout << "(" << _iter->first << ',' << _iter->second << ") ";
	cout << endl;


	// key가 5인 요소 찾기
	_iter = _m.find("three");
	if (_iter != _m.end())
		cout << "key three 에 매핑된 value : " << _iter->second << endl;

	// key가 5인 요소의 첫번째 요소 / 마지막의 다음 요소 찾기
	map<string, int>::iterator _lower_iter;
	map<string, int>::iterator _upper_iter;
	_lower_iter = _m.lower_bound("five");
	_upper_iter = _m.upper_bound("five");

	cout << "lower_iter : " << _lower_iter->second << endl;
	cout << "upper_iter : " << _upper_iter->second << endl;

	// key가 5인 요소의 범위  찾기
	pair<map<string, int>::iterator, map<string, int>::iterator> _iter_pair;
	_iter_pair = _m.equal_range("five");

	for (_iter = _iter_pair.first; _iter != _iter_pair.second; ++_iter)
		cout << "(" << _iter->first << ',' << _iter->second << ") ";
	cout << endl;
}

void pair_and_map_use_by_the_vector_1()
{
	map<int, std::string>    mapTest;
	mapTest.insert(pair< int, std::string >(1, "Red"));
	mapTest.insert(pair< int, std::string >(2, "Orange"));
	mapTest.insert(pair< int, std::string >(3, "Yellow"));
	mapTest.insert(pair< int, std::string >(4, "Green"));
	mapTest.insert(pair< int, std::string >(5, "blue"));

	vector<map<int, std::string>> vmVectorTest;
	vmVectorTest.push_back(mapTest);

	vector<map<int, std::string>>::iterator vitr;
	vitr = vmVectorTest.begin();

	map<int, std::string>::iterator mapitr;
	mapitr = vitr->begin();

	do
	{
		cout << mapitr->first << " , " << mapitr->second << endl;
		++mapitr;
	} while (mapitr != vitr->end());
}

void pair_and_map_use_by_the_vector_2()
{
	std::string name[3] = { "song","min","kyu" };
	int Y_M_D_to_Born[3] = { 1988,11,25 };

	std::vector<std::pair<std::string, int> > myvector;

	for (int i = 0; i < 3; i++)
		myvector.push_back(std::make_pair(name[i], Y_M_D_to_Born[i]));

	// print pair object  
	for (size_t i = 0; i < myvector.size(); i++)
		std::cout << myvector[i].first << ", " << myvector[i].second << '\n';
	std::cout << '\n';

	// ordering pair object  
	std::sort(myvector.begin(), myvector.end());

	// print pair object after ordering pair object  
	for (size_t i = 0; i < myvector.size(); i++)
		std::cout << myvector[i].first << ", " << myvector[i].second << '\n';
}
