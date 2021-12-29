#define max(a,b) ((a)>(b)?(a):(b))

#include <assert.h>
#include <stdlib.h>
#include <algorithm>

#include "StackAllocator.h"

StackAllocator::StackAllocator(const size_t InTotalAllocSize) :
	Allocator{ InTotalAllocSize },
	mStartPointer{ nullptr },
	mOffset{0}
{
}

void StackAllocator::Init()
{
	if (mStartPointer != nullptr)
	{
		free(mStartPointer);
	}
	mStartPointer = malloc(mTotalAllocSize);
	mOffset = 0;
}

StackAllocator::~StackAllocator()
{
	if (mStartPointer != nullptr)
	{
		free(mStartPointer);
	}
}

void* StackAllocator::Allocate(const size_t InAllocSize, const size_t InAligment)
{
	const size_t AlllocationHeaderSize = sizeof(AllocationHeader);
	const size_t lCurrentAddress = reinterpret_cast<size_t>(mStartPointer) + mOffset;
	const size_t lAligment = InAligment == 0 ? 1 : InAligment;

	size_t lPadding = CalculatePaddingWithHeader(lCurrentAddress, lAligment, sizeof(AlllocationHeaderSize));

	if (mOffset + lPadding + InAllocSize > mTotalAllocSize)
	{
		return nullptr;
	}
	mOffset += lPadding;

	const size_t lNextAddress = lCurrentAddress + lPadding;
	const size_t lHeaderAddress = lNextAddress - AlllocationHeaderSize;
	AllocationHeader lAllocationHeader{ lPadding };
	AllocationHeader* ptrHeader = reinterpret_cast<AllocationHeader*>(lHeaderAddress);
	ptrHeader = &lAllocationHeader;

	mOffset += InAllocSize;
	mUsed = mOffset;
	mPeak = max(mPeak, mUsed);

	return reinterpret_cast<void*>(lNextAddress);
}

void StackAllocator::Free(void* InPointer)
{
	const size_t AlllocationHeaderSize = sizeof(AllocationHeader);
	const size_t lCurrentAddress = reinterpret_cast<size_t>(InPointer);
	const size_t lHeaderAddress = lCurrentAddress - AlllocationHeaderSize;
	const AllocationHeader* ptrAllocationHeader{ reinterpret_cast<AllocationHeader*>(lHeaderAddress)};

	mOffset = lCurrentAddress - ptrAllocationHeader->mPadding - reinterpret_cast<size_t>(mStartPointer);
	mUsed = mOffset;
}

void StackAllocator::Reset()
{
	mOffset = 0;
	mUsed = 0;
	mPeak = 0;
}
