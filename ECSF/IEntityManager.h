#pragma once

#ifdef ECSF_EXPORTS
#define ECSF_API __declspec(dllexport)
#else
#define ECSF_API __declspec(dllimport)
#endif

#include <vector>

#include "ECS_utils.h"
#include "ComponentContainer.h"

class ECSF_API IEntityManager
{
public:

	IEntityManager() {}
	virtual ~IEntityManager() {}

	virtual const EntityID AddEntity() = 0;
	virtual bool RemoveEntity(const EntityID InEntityId) = 0;
	virtual bool HasEntity(const EntityID InEntityId) = 0;

	virtual const size_t GetMaxEntitiesCount() const = 0;
	virtual void SetMaxEntitiesCount(const size_t InMaxEntitiesCount) = 0;

	virtual bool AddComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
	virtual bool RemoveComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
	virtual bool HasComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
	virtual IComponentBase* GetComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
};