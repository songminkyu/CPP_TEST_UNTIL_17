#pragma once
#include <stdexcept>

template<typename T> class YY {
};

/*
1. type ==> typename T
2. non-type ==> int N
3. template => template<typename T> class ZZ;
*/

template <typename T, int MAXSIZE = 20>  // 타입파라미터에서  non타입 파라미터로
class Stack {
private:
	T elems[MAXSIZE];        // elements  배열사용
	int numElems;            // current number of elements

public:
	Stack();                  // constructor
	void push(T const&);      // push element
	void pop();               // pop element
	T top() const;            // return top element
	bool empty() const {      // return whether the stack is empty
		return numElems == 0;
	}
	bool full() const {       // return whether the stack is full
		return numElems == MAXSIZE;
	}
};

// constructor
template <typename T, int MAXSIZE>
Stack<T, MAXSIZE>::Stack()
	: numElems(0)               // start with no elements
{
	// nothing else to do
}

template <typename T, int MAXSIZE>
void Stack<T, MAXSIZE>::push(T const& elem)
{
	if (numElems == MAXSIZE) {
		throw std::out_of_range("Stack<>::push(): stack is full");
	}
	elems[numElems] = elem;   // append element
	++numElems;               // increment number of elements
}

template<typename T, int MAXSIZE>
void Stack<T, MAXSIZE>::pop()
{
	if (numElems <= 0) {
		throw std::out_of_range("Stack<>::pop(): empty stack");
	}
	--numElems;               // decrement number of elements
}

template <typename T, int MAXSIZE>
T Stack<T, MAXSIZE>::top() const
{
	if (numElems <= 0) {
		throw std::out_of_range("Stack<>::top(): empty stack");
	}
	return elems[numElems - 1];  // return last element
}
