#pragma once

#include <memory>

#include "IComponentBase.h"

template <class ComponentAllocator>
class ComponentsContainer
{
public:
	ComponentsContainer()
	{}
	~ComponentsContainer()
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
private:
	//std::vector
};