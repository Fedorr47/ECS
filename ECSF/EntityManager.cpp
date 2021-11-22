#include "pch.h"

#include "EntityManager.h"
#include "ComponentAdder.h"

IEntityManager* EntityManager::ObjectEntityManager = nullptr;

IEntityManager* EntityManager::GetInstance()
{
	if (ObjectEntityManager == nullptr)
	{
		ObjectEntityManager = new EntityManager();
	}
	return ObjectEntityManager;
}

/*****************************************************************************/
/*					Entity Logic											 */
/*****************************************************************************/

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
	bool Result = false;

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

/*****************************************************************************/
/*					ComponentAdder Logic									 */
/*****************************************************************************/

bool EntityManager::RegisterComponent(const ComponentType InComponentType)
{
	return false;
}

bool EntityManager::UnregisterComponent(const ComponentType InComponentType)
{
	return mComponentAdder->UnregisterComponentType(InComponentType);
}

/*****************************************************************************/
/*					Component Logic											 */
/*****************************************************************************/

bool EntityManager::AddComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	bool Result = false;
	OptionalComponent pComponent = mComponentAdder->HasComponent(InComponentType);
	if (pComponent)
	{
		//mComponentsContainer->AddComponent(pComponent.get());
		Result = true;
	}
	return Result;
}

bool EntityManager::RemoveComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	bool Result = false;
	return Result;
}

bool EntityManager::HasComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	bool Result = false;
	return Result;
}

std::optional<IComponentBase*> EntityManager::GetComponent(const EntityID InEntityId, const ComponentType InComponentType)
{
	return {};
}

IEntityManager* CreateEntityManager()
{
	return EntityManager::GetInstance();
}