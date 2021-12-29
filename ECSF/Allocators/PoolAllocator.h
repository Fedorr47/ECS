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
	PoolAllocator(const size_t InTotalAllocSize);
	virtual ~PoolAllocator();

	void* Allocate(const size_t InAllocSize, const size_t InAligment) override;
	void Free(void* InPointer) override;
	void Init() override {};
	void* GetData() override;
	size_t GetHeaderSize() override { return sizeof(Node); };

	template <class ChunckType>
	void Init()
	{
		if (mStartPointer != nullptr)
		{
			free(mStartPointer);
		}
		mChunckSize = sizeof(ChunckType) + sizeof(Node);
		mTotalAllocSize = mTotalAllocSize * mChunckSize;
		mStartPointer = malloc(mTotalAllocSize);
		Reset();
	}
	void Reset() override;
};