#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>

#include "ECS/ECS_Types.h"
#include "GameData/TextureManager.h"
#include "ECS/Systems/SystemManager.h"

using ComponentContainer = std::vector<class ComponentBase*>;
using EntityData = std::pair<Bitmask, ComponentContainer>;
using EntityContainer = std::unordered_map<EntityID, EntityData>;
using ComponentFactory = std::unordered_map<EComponentType, std::function<ComponentBase* (void)>>;

class EntityManager
{
	unsigned int mIdCounter{ 0 };
	EntityContainer mEntities;
	ComponentFactory mComponentFactory;

	class SystemManager*	mSystems;
	class TextureManager*	mTextureManager;

	template <class T>
	void AddComponentType(const EComponentType ComponentType)
	{
		mComponentFactory[ComponentType] = []()->ComponentBase* { return new T(); };
	}

public:

	EntityManager(
		SystemManager* InSystemManager,
		TextureManager* InTextureManager
	);

	~EntityManager() 
	{
		Purge();
	}

	TextureManager* GetTextureManager() const { return mTextureManager; }

	EntityID AddEntity(const Bitmask& Mask);

	bool RemoveEntity(const EntityID EntityId);

	bool AddComponent(
		const EntityID EntityId,
		const EComponentType ComponentType);

	template <class T>
	T* GetComponent(
		const EntityID EntityId,
		const EComponentType ComponentType
	);

	bool RemoveComponent(
		const EntityID EntityId,
		const EComponentType ComponentType
	);

	bool HasComponent(
		const EntityID EntityId,
		const EComponentType ComponentType
	);

	void Purge();
};

template<class T>
inline T* EntityManager::GetComponent(const EntityID EntityId, const EComponentType ComponentType)
{
	auto EntityItr = mEntities.find(EntityId);

	if (EntityItr == mEntities.end())
	{
		return nullptr;
	}

	// Found the entity.
	Bitmask Mask = EntityItr->second.first;

	if (!Mask.test(static_cast<unsigned int>(ComponentType)))
	{ 
		return nullptr; 
	}
	// Component exists.

	auto& container = EntityItr->second.second;
	auto component = std::find_if(container.begin(), container.end(),
		[&ComponentType](ComponentBase* c) { return c->GetType() == ComponentType; });
	return (component != container.end() ? dynamic_cast<T*>(*component) : nullptr);
}
