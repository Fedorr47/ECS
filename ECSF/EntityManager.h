#pragma once

#include "IEntityManager.h"
#include "ComponentAdder.h"
#include "ComponentsContainer.h"

using ComponentsContainerType = ComponentsContainer<std::allocator<IComponentBase>>;

class ECSF_API EntityManager : public IEntityManager
{
protected:
	EntityManager() :
		IEntityManager(),
		mMaxEntitiesCount{ DEFAULT_MAX_ENTITIES_COUNT }
	{
		mFreeEntityIDs.resize(DEFAULT_MAX_ENTITIES_COUNT);
		mComponentsContainer = std::make_unique<ComponentsContainerType>();
		mComponentAdder = std::make_unique<ComponentAdder>();
	}
public:
	virtual ~EntityManager() {
		ObjectEntityManager = nullptr;
	}

	static IEntityManager* GetInstance();

	/*****************************************************************************/
	/*					Entity Logic											 */
	/*****************************************************************************/
	const EntityID AddEntity() override;
	bool RemoveEntity(const EntityID InEntityId) override;
	bool HasEntity(const EntityID InEntityId) override;

	const size_t GetMaxEntitiesCount() const override
	{ return mMaxEntitiesCount; }
	virtual void SetMaxEntitiesCount(const size_t InMaxEntitiesCount) override
	{ mMaxEntitiesCount = InMaxEntitiesCount; }

	/*****************************************************************************/
	/*					ComponentAdder Logic									 */
	/*****************************************************************************/
	virtual bool RegisterComponent(const ComponentType InComponentType) override;
	template <class TComponentType>
	bool RegisterComponent(const ComponentType InComponentType) { return mComponentAdder->RegisterComponentType<TComponentType>(InComponentType); }
	virtual bool UnregisterComponent(const ComponentType InComponentType) override;
	virtual const size_t GetComponentsCount() const override { return mComponentAdder->GetComponentsCount(); }
	virtual void SetComponentsCount(const size_t InComponentsCount) override { mComponentAdder->SetComponentsCount(InComponentsCount); }

	/*****************************************************************************/
	/*					Component Logic											 */
	/*****************************************************************************/
	bool AddComponent(const EntityID InEntityId, const ComponentType InComponentType) override;
	bool RemoveComponent(const EntityID InEntityId, const ComponentType InComponentType) override;
	bool HasComponent(const EntityID InEntityId, const ComponentType InComponentType) override;
	std::optional<IComponentBase*> GetComponent(const EntityID InEntityId, const ComponentType InComponentType) override;

private:

	EntityManager(EntityManager& other) = delete;
	void operator=(const EntityManager&) = delete;

	size_t mMaxEntitiesCount;
	std::vector<bool> mFreeEntityIDs;

	std::unique_ptr<ComponentsContainerType> mComponentsContainer;
	std::unique_ptr<ComponentAdder> mComponentAdder;

	static IEntityManager* ObjectEntityManager;
};

extern "C" ECSF_API IEntityManager* CreateEntityManager();