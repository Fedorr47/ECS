#pragma once

#include <memory>

#include "IComponentBase.h"

template <class ComponentAllocator>
class ComponentContainer
{
public:
	ComponentContainer()
	{}
	~ComponentContainer()
	{}

	bool AddComponent(IComponentBase* Component)
	{
		bool Result = false;
		return Result;
	}
	bool RemoveComponent(IComponentBase* Component)
	{
		bool Result = false;
		return Result;
	}
};