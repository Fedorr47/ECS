#pragma once

#include <assert.h>
#include <memory.h>
#include <utility>
#include <initializer_list>
#include "Allocator.h"
#include "Utils.h"

#define FORCE_INLINE __forceinline

template <class TElementType, typename TAllocator>
class FArray
{
private:

	friend class Iterator;

	using ElementType = TElementType;
	using Allocator = TAllocator;
	using ReferenceType = ElementType&;
	using PointerType = ElementType*;
	using ConstPointerType = const ElementType*;

public:
	struct Iterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = ElementType;
		using pointer = ElementType*;
		using reference = ElementType&;

		Iterator(pointer InPtr, size_t InOffset = 0) :
			mPtr(InPtr),
			mOffset(InOffset)
		{}

		FORCE_INLINE reference operator*() const 
		{ 
			return *mPtr; 
		}
		FORCE_INLINE pointer operator->() { 
			return mPtr; 
		}
		FORCE_INLINE Iterator& operator++()
		{ 
			++mPtr;
			if (mOffset)
			{
				mPtr = reinterpret_cast<pointer>(reinterpret_cast<size_t>(mPtr) + mOffset);
			}
			return *this; 
		}
		FORCE_INLINE Iterator& operator++(int)
		{ 
			Iterator tmp = *this; 
			++mPtr;
			if (mOffset)
			{
				mPtr = reinterpret_cast<pointer>(reinterpret_cast<size_t>(mPtr) + mOffset);
			}
			return tmp; 
		}
		FORCE_INLINE friend	bool operator==(const Iterator& first, const Iterator& second) { return first.mPtr == second.mPtr; }
		FORCE_INLINE friend	bool operator!=(const Iterator& first, const Iterator& second) { 
			return first.mPtr != second.mPtr; 
		}
	private:
		pointer mPtr;
		size_t mOffset;
	};

	FORCE_INLINE explicit FArray(size_t InElementCount) :
		mNum{0},
		mMax{0}
	{
		mAllocatorInstance = std::make_unique<Allocator>(InElementCount);
		mAllocatorInstance->Init<ElementType>();
		mMax = InElementCount;
	}


	FORCE_INLINE FArray(std::initializer_list<ElementType> InList) {};

	template <
		class OtherElementType,
		class OtherAllocator,
		FArrayElementsAreCompatible T
	>
	FORCE_INLINE FArray(const FArray<OtherElementType, OtherAllocator>& InOther) {}

	FORCE_INLINE ElementType* GetData()
	{
		return reinterpret_cast<ElementType*>(mAllocatorInstance->GetData());
	}

	FORCE_INLINE const ElementType* GetData() const
	{
		return reinterpret_cast<const ElementType*>(mAllocatorInstance->GetData());
	}

	FORCE_INLINE const size_t GetTypeSize() const
	{
		return sizeof(ElementType);
	}
	
	FORCE_INLINE const size_t GetOffset() const
	{
		return mMax - mNum;
	}

	FORCE_INLINE void CheckInvariants() const
	{
		assert(mNum >= 0 && mMax >= mNum);
	}

	FORCE_INLINE bool RangeCheck(size_t InIndex) const
	{
		return (InIndex >= 0 && InIndex < mNum);
	}

	FORCE_INLINE size_t Num() const 
	{
		return mNum;
	}

	template <class... ArgsType>
	FORCE_INLINE size_t Emplace(ArgsType&& ...Args)
	{
		CheckInvariants();
		auto ptrAllocatedAddress = mAllocatorInstance->Allocate(GetTypeSize(), 0);
		new(ptrAllocatedAddress) ElementType(std::forward<ArgsType>(Args)...);
		++mNum;
		return mNum;
	}

	FORCE_INLINE ElementType& operator[](size_t InIndex)
	{
		size_t ptrElementAddress = reinterpret_cast<size_t>(&GetData()[InIndex]) + (InIndex * mAllocatorInstance->GetHeaderSize());
		return static_cast<ElementType&>(*reinterpret_cast<ElementType*>(ptrElementAddress));
	}

	template <class ComporationType>
	FORCE_INLINE bool Contains(const ComporationType& Item)
	{
		for (ElementType* Data = GetData(), *DataEnd = GetLast(); Data != DataEnd; Next(Data))
		{
			if (*Data == Item)
			{
				return true;
			}
		}
		return false;
	}

	FORCE_INLINE void ClearAll()
	{
		mAllocatorInstance->Reset();
		mNum = 0;
	}

	template <class IsPositiveInteger>
	FORCE_INLINE void RemoveByindex(size_t InIndex)
	{
		return RemoveByIndexImpl(InIndex, 1);
	}


	FORCE_INLINE size_t Find(const ElementType& InItem)
	{
		const ElementType* __restrict lStart = GetData();
		for (ElementType* Data = GetData(), *DataEnd = GetLast(); Data != DataEnd; Next(Data))
		{
			if (*Data == InItem)
			{
				return static_cast<size_t>(Data - lStart);
			}
		}
		return INDEX_NONE;
	}

	FORCE_INLINE bool Find(const ElementType& InItem, size_t& OutIndex)
	{
		OutIndex = Find(InItem);
		return OutIndex != INDEX_NONE;
	}

	void Sort()
	{

	}

private:
	template <class IsPositiveInteger>
	void RemoveByIndexImpl(size_t InIndex, IsPositiveInteger InCount)
	{
		CheckInvariants();

		while (InCount)
		{
			mAllocatorInstance->Free(GetByIndex(InIndex));
			--InCount;
			++InIndex;
		}
	}

	FORCE_INLINE ElementType* GetLast() const
	{
		size_t lElementAddress = reinterpret_cast<size_t>(&GetData()[mMax]) + (mMax * mAllocatorInstance->GetHeaderSize());
		return reinterpret_cast<ElementType*>(lElementAddress);
	}

	FORCE_INLINE void Next(ElementType*& InElement) const
	{
		size_t lElementAddress = reinterpret_cast<size_t>(++InElement) + (mAllocatorInstance->GetHeaderSize());
		InElement = reinterpret_cast<ElementType*>(lElementAddress);
	}

	FORCE_INLINE ElementType* GetByIndex(size_t InIndex) const
	{
		size_t lElementAddress = reinterpret_cast<size_t>(&GetData()[InIndex]) + (mAllocatorInstance->GetHeaderSize() * InIndex);
		return reinterpret_cast<ElementType*>(lElementAddress);
	}

public:
	
	Iterator begin() { return Iterator(&GetData()[0], mAllocatorInstance->GetHeaderSize()); }
	Iterator end() {

		return Iterator(GetLast(), mAllocatorInstance->GetHeaderSize());
	}

protected:
	std::unique_ptr<Allocator> mAllocatorInstance;
	size_t mNum;
	size_t mMax;
};