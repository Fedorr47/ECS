#pragma once

#ifdef ECSF_EXPORTS
#define ECSF_API __declspec(dllexport)
#else
#define ECSF_API __declspec(dllimport)
#endif

#include <vector>
#include <optional>

#include "ECS_utils.h"

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

	virtual bool RegisterComponent(const ComponentType InComponentType) = 0;
	virtual bool UnregisterComponent(const ComponentType InComponentType) = 0;
	virtual const size_t GetComponentsCount() const = 0;
	virtual void SetComponentsCount(const size_t InComponentsCount) = 0;

	virtual bool AddComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
	virtual bool RemoveComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
	virtual bool HasComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
	virtual std::optional<class IComponentBase*> GetComponent(const EntityID InEntityId, const ComponentType InComponentType) = 0;
};