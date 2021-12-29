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
	
	Node* head = nullptr;
	Node* tail = nullptr;
	
	StackLinkedList() = default;
	void push(Node* InNode)
	{
		if (head == nullptr)
		{
			head = InNode;
			tail = head;
		}
		else
		{
			InNode->next = nullptr;
			tail->next = InNode;
			tail = InNode;
		}
	}
	Node* pop()
	{
		Node* top = head;
		head = head->next;
		if (head == nullptr)
		{
			tail = nullptr;
		}
		return top;
	}
private:
	StackLinkedList(StackLinkedList& OtherStackLinkedList) = delete;
};