#pragma once

#include "Allocator.h"

static const size_t CalculatePadding(
	const size_t InBaseAddress, 
	const size_t InAlignment)
{
	const size_t lMultiplier = (InBaseAddress / InAlignment) + 1;
	const size_t lAlignmentAddress = InAlignment * lMultiplier;
	const size_t lPadding = lAlignmentAddress - InBaseAddress;
	return lPadding;
}

static const size_t CalculatePaddingWithHeader(
	const size_t InBaseAddress,
	const size_t InAlignment,
	const size_t InHeaderSize)
{
	size_t lPadding = CalculatePadding(InBaseAddress, InAlignment);
	size_t lNeedSpace = InHeaderSize;

	if (lPadding < lNeedSpace)
	{
		lNeedSpace -= lPadding;

		if (lNeedSpace % InAlignment > 0)
		{
			lPadding += InAlignment * (1 + (lNeedSpace / InAlignment));
		}
		else
		{
			lPadding += InAlignment * (lNeedSpace / InAlignment);
		}
	}
	return lPadding;
}

class StackAllocator : public Allocator
{
protected:
	void* mStartPointer = nullptr;
	size_t mOffset;

	struct AllocationHeader
	{
		char mPadding;
	};

public:
	explicit StackAllocator(const size_t InTotalAllocSize);
	virtual ~StackAllocator();

	void* Allocate(const size_t InAllocSize, const size_t InAligment) override;
	void Free(void* InPointer) override;
	void Init() override;
	void* GetData() override { return reinterpret_cast<void*>(reinterpret_cast<size_t>(mStartPointer) + mOffset); };
	size_t GetHeaderSize() override { return 0; };
	void Reset() override;
};