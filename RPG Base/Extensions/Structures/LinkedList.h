#pragma once
#include "Nodes.h"

template<class T> class LLNode;

template <class T> 
class LinkedList
{
public:
	LLNode<T>* first;
	LLNode<T>* last;
	int size;

	LinkedList(T data);

	void insert(T data, int index);
	void append(T data);

	void insert(LLNode<T>* node, int index);
	void append(LLNode<T>* node);

	T getData(int index);
	void setData(T data, int index);
		
	T getData(LLNode<T>* node);
	void setData(T data, LLNode<T>* node);

	LLNode<T>* getNext(int index);
	LLNode<T>* getNext(LLNode<T>* node);

	void remove(int index);
	void remove(LLNode<T>* node);
};


template <class T> 
LinkedList<T>::LinkedList(T data)
{
	first = new LLNode<T>(data);
	last = first;
	size = 1;
}

template <class T> 
void LinkedList<T>::insert(T data, int index)
{
	if (index < 0 || index > size)
	{
		return;
	}

	int i = 1;
	LLNode<T>* c = first;
	LLNode<T>* node = new LLNode<T>(data);
	
	while (i != index && index != 0)
	{
		c = c->next;
		i++;
	}

	index == 0 ? node->next = c : node->next = c->next;
	index == 0 ? first = node : c->next = node;
	if (index == size) last = node;

	size++;
}
template <class T>
void LinkedList<T>::insert(LLNode<T>* node, int index)
{
	if (index < 0 || index > size)
	{
		return;
	}

	int i = 1;
	LLNode<T>* c = first;

	while (i != index && index != 0)
	{
		c = c->next;
		i++;
	}

	index == 0 ? node->next = c : node->next = c->next;
	index == 0 ? first = node : c->next = node;
	if (index == size) last = node;

	size++;
}

template <class T>
void LinkedList<T>::append(T data)
{
	LLNode<T>* node = new LLNode<T>(data);

	size == 0 ? first = node : last->next = node;
	last = node;
	size++;
}
template <class T>
void LinkedList<T>::append(LLNode<T>* node)
{
	size == 0 ? first = node : last->next = node;
	last = node;
	size++;
}

template <class T>
T LinkedList<T>::getData(int index) 
{
	if (index < 0 || index > size)
	{
		T t = T();
		return t;
	}

	int i = 0;
	LLNode<T>* c = first;
	while (i != index)
	{
		c = c->next;
		i++;
	}
	return  c->data;
}
template <class T>
T LinkedList<T>::getData(LLNode<T>* node) 
{
	if (node != nullptr)
		return node->data;
	else
	{
		T t = T();
		return t;
	}
}

template <class T>
void LinkedList<T>::setData(T data, int index)
{
	if (index < 0 || index > size)
	{
		return;
	}

	int i = 0;
	LLNode<T>* c = first;
	while (i != index)
	{
		c = c->next;
		i++;
	}
	c->data = data;
}
template <class T>
void LinkedList<T>::setData(T data, LLNode<T>* node)
{
	if (node == nullptr)
		return;

	node->data = data;
}

template <class T>
LLNode<T>* LinkedList<T>::getNext(int index)
{
	int i = 1;
	LLNode<T>* c = first;
	LLNode<T>* node = new LLNode<T>(data);

	while (i != index && index != 0)
	{
		c = c->next;
		i++;
	}
	
	return c->next;
}
template <class T>
LLNode<T>* LinkedList<T>::getNext(LLNode<T>* node)
{
	return node->next;
}

template <class T>
void LinkedList<T>::remove(int index)
{
	if (index < 0 || index > size || size == 0)
	{
		return;
	}

	int i = 0;
	LLNode<T>* c = first;
	LLNode<T>* temp = first->next;
	while (i != index - 1 && index != 0)
	{
		c = c->next;
		i++;
	}
	temp = c->next;

	if (temp == last) last = c;
	index == 0 ? (size == 1 ? (first = nullptr, last = nullptr) : first = first->next, temp = c) : c->next = temp->next;
	delete temp;
	size--;
}

template <class T>
void LinkedList<T>::remove(LLNode<T>* node)
{
	int i = 0;
	LLNode<T>* c = first;
	LLNode<T>* temp = first->next;

	while (c->next != node && c != node && i < size)
	{
		c = c->next;
		i++;
	}

	temp = c->next;
	if (i < size)
	{
		c == node ? (size == 1 ? (first = nullptr, last = nullptr) : first = first->next) : c->next = temp->next;
		delete temp;
		size--;
	}
}