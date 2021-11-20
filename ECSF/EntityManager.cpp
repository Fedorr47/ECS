#include "pch.h"
#include "EntityManager.h"

const EntityID EntityManager::AddEntity()
{
	EntityID EntityId = 0;
	for (EntityID Entity = 0; Entity < mFreeEntityIDs.size(); ++Entity)
	{
		if (!mFreeEntityIDs[Entity])
		{
			mFreeEntityIDs[Entity] = true;
			EntityId = ++Entity;
			break;
		}
	}
	return EntityId;
}

bool EntityManager::RemoveEntity(const EntityID InEntityId)
{
	bool Result = true;

	if (HasEntity(InEntityId))
	{
		mFreeEntityIDs[InEntityId-1] = false;
		Result = true;
	}

	return Result;
}

bool EntityManager::HasEntity(const EntityID InEntityId)
{
	return InEntityId <= mMaxEntitiesCount && mFreeEntityIDs[InEntityId-1];
}

bool EntityManager::AddComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	bool Result = true;
	return Result;
}

bool EntityManager::RemoveComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	bool Result = true;
	return Result;
}

bool EntityManager::HasComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	bool Result = true;
	return Result;
}

IComponentBase* EntityManager::GetComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	return nullptr;
}

IEntityManager* CreateEntityManager()
{
	IEntityManager* ObjEntityManager = new EntityManager();
	return ObjEntityManager;
}