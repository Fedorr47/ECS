#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

#include "GameData/EventQueue.h"
#include "ECS/MessageHandler.h"

#include "ECS/ECS_Types.h"
#include "ECS/Systems/SystemBase.h"

using SystemContainer = std::unordered_map<ESystemType, std::unique_ptr<SystemBase>>;
using EntityEventContainer = std::unordered_map<EntityID, EventQueue>;

class EntityManager;
class SystemManager {
	SystemContainer mSystems;
	EntityManager* mEntityManager;
	EntityEventContainer mEvents;
	MessageHandler m_messages;
	struct SharedContext* mSharedContext;

public:
	SystemManager();
	~SystemManager();

	void SetEntityManager(EntityManager* InEntityManager);
	EntityManager* GetEntityManager();
	class MessageHandler* GetMessageHandler();

	template<class T>
	T* GetSystem(const ESystemType& SystemType) 
	{
		auto System = mSystems.find(SystemType);
		return dynamic_cast<T*>(System->second.get());
	}

	void AddEvent(const EntityID& EntityId, const EventID& EventType);

	void Update(float DeltaTime);
	void HandleEvents();
	void Draw(class Window* InWindow);

	void EntityModified(const EntityID& EntityId, const Bitmask& Bits);
	void RemoveEntity(const EntityID& EntityId);

	void PurgeEntities();
	void PurgeSystems();

	void SetSharedConxtext(struct SharedContext* InSharedContext);
	SharedContext* GetSharedConxtext();

};