#include "pch.h"
#include "EntityManager.h"

EntityID EntityManager::AddEntity()
{
	EntityID EntityId = 1;
	for (EntityID Entity = 0; Entity < mFreeEntityIDs.size(); ++Entity)
	{

	}
	return EntityId;
}

bool EntityManager::RemoveEntity()
{
	bool Result = false;
	return Result;
}

IEntityManager* CreateEntityManager()
{
	IEntityManager* ObjEntityManager = new EntityManager();
	return ObjEntityManager;
}