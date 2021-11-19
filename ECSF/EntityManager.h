#pragma once

#include "IEntityManager.h"

class ECSF_API EntityManager : public IEntityManager
{
public:
	EntityManager() {}
	virtual ~EntityManager() {}

	virtual EntityID AddEntity();
	virtual bool RemoveEntity();
private:
	std::vector<bool> mFreeEntityIDs;
};

extern "C" ECSF_API IEntityManager* CreateEntityManager();