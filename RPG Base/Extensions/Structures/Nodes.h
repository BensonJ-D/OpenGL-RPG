#pragma once
#include "LinkedList.h"

template <class T> class LinkedList;

template <class T>
class LLNode
{
public:
	LLNode(T _data);

private:
	T data;
	LLNode<T>* next;
	LinkedList<T>* parent;

	friend class LinkedList<T>;
};

template <class T>
LLNode<T>::LLNode(T _data)
{
	data = _data;
	next = nullptr;
}
