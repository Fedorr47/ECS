#pragma once

#include <memory>

#include "ComponentBase.h"

template <class ComponentAllocator>
class ComponentContainer
{
public:
	ComponentContainer()
	{}
	~ComponentContainer()
	{}

	bool AddComponent(ComponentBase* Component)
	{
		bool Result = false;
		return Result;
	}
	bool RemoveComponent(ComponentBase* Component)
	{
		bool Result = false;
		return Result;
	}
};