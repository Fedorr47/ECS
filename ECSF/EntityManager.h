#pragma once

#include "IEntityManager.h"

class ECSF_API EntityManager : public IEntityManager
{
protected:
	EntityManager() :
		IEntityManager(),
		mMaxEntitiesCount(DEFAULT_MAX_ENTITIES_COUNT)
	{
		mFreeEntityIDs.resize(DEFAULT_MAX_ENTITIES_COUNT);
	}
public:
	virtual ~EntityManager() {
		ObjectEntityManager = nullptr;
	}

	static IEntityManager* GetInstance();

	const EntityID AddEntity() override;
	bool RemoveEntity(const EntityID InEntityId) override;
	bool HasEntity(const EntityID InEntityId) override;

	const size_t GetMaxEntitiesCount() const override
	{ return mMaxEntitiesCount; }
	virtual void SetMaxEntitiesCount(const size_t InMaxEntitiesCount) override
	{ mMaxEntitiesCount = InMaxEntitiesCount; }

	bool AddComponent(const EntityID InEntityId, const ComponentType InComponentType) override;
	bool RemoveComponent(const EntityID InEntityId, const ComponentType InComponentType) override;
	bool HasComponent(const EntityID InEntityId, const ComponentType InComponentType) override;
	IComponentBase* GetComponent(const EntityID InEntityId, const ComponentType InComponentType) override;

private:

	EntityManager(EntityManager& other) = delete;
	void operator=(const EntityManager&) = delete;

	size_t mMaxEntitiesCount;
	std::vector<bool> mFreeEntityIDs;

	static IEntityManager* ObjectEntityManager;
};

extern "C" ECSF_API IEntityManager* CreateEntityManager();