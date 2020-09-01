#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint> // Needed for intptr_t
using namespace std;

enum TypeID {
	TYPE_INT,
	TYPE_CHAR_PTR,
	TYPE_MYFIELD
};

struct MyField {
	int typeId;
	void * data;
};

int main() {

	std::unordered_map<std::string, MyField> map;

	MyField anInt = { TYPE_INT, reinterpret_cast<void*>(42) };

	char cstr[] = "Jolly good";
	MyField aCString = { TYPE_CHAR_PTR, cstr };

	MyField aStruct = { TYPE_MYFIELD, &anInt };

	map.emplace("Int", anInt);
	map.emplace("C String", aCString);
	map.emplace("MyField", aStruct);

	int         intval = static_cast<int>(reinterpret_cast<intptr_t>(map["Int"].data));
	const char *cstr2 = reinterpret_cast<const char *>(map["C String"].data);
	MyField*    myStruct = reinterpret_cast<MyField*>(map["MyField"].data);

	cout << intval << '\n'
		<< cstr << '\n'
		<< myStruct->typeId << ": " << static_cast<int>(reinterpret_cast<intptr_t>(myStruct->data)) << endl;
}