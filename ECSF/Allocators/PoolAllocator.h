#pragma once

#include "Allocator.h"
#include "StackLinkedList.h"

class PoolAllocator : public Allocator
{
protected:

	void* mStartPointer = nullptr;
	size_t mChunckSize;

	struct FreeHeader{};

	using Node = StackLinkedList<FreeHeader>::Node;
	StackLinkedList<FreeHeader> mFreeBlocksList;	

public:
	PoolAllocator(const size_t InTotalAllocSize, const size_t InChunckSize);
	virtual ~PoolAllocator();

	void* Allocate(const size_t InAllocSize, const size_t InAligment) override;
	void Free(void* InPointer) override;
	void Init() override;
	void* GetData() override { return nullptr; };
	void Reset();
};