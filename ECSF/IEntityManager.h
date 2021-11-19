#pragma once

#include <vector>

#include "ECS_utils.h"
#include "ComponentContainer.h"

#ifdef ECSF_EXPORTS
#define ECSF_API __declspec(dllexport)
#else
#define ECSF_API __declspec(dllimport)
#endif

using EntityData = std::pair<EntityID, ComponentContainer<std::allocator<ComponentBase>>>;

class ECSF_API IEntityManager
{
public:

	IEntityManager() {};
	virtual ~IEntityManager() {};

	virtual EntityID AddEntity() = 0;
	virtual bool RemoveEntity() = 0;
};