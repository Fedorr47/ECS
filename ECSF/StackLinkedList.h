#pragma once

template <class T>
class StackLinkedList
{
public:
	struct Node
	{
		T data;
		Node* next;
	};

	Node* head;

	StackLinkedList() = default;
	void push(Node* InNode)
	{
		InNode->next = head;
		head = InNode;
	}
	Node* pop()
	{
		Node* top = head;
		head = head->next;
		return top;
	}
private:
	StackLinkedList(StackLinkedList& OtherStackLinkedList) = delete;
};