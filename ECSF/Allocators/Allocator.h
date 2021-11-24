#pragma once

class Allocator
{
public:
	Allocator(const size_t InTotalAllocSize) :
		mTotalAllocSize{ InTotalAllocSize },
		mUsed{ 0 },
		mPeak{ 0 }
	{}
	virtual ~Allocator() { mTotalAllocSize = 0; }

	virtual void* Allocate(const size_t InAllocSize, const size_t InAligment = 0) = 0;
	virtual void Free(void* InPointer) = 0;
	virtual void Init() = 0;
	virtual void* GetData() = 0;

protected:
	size_t mTotalAllocSize;
	size_t mUsed;
	size_t mPeak;
};
