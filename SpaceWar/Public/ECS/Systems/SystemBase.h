#pragma once
#include <vector>

#include "GameData/EntityEvents.h"
#include "ECS/Observer.h"
#include "ECS/ECS_Types.h"

using EntityList = std::vector<EntityID>;
using Requirements = std::vector<Bitmask>;

class SystemManager;
class SystemBase : public Observer{

public:

	SystemBase(const ESystemType& SystemType, SystemManager* InSystemManager);
	virtual ~SystemBase();

	bool AddEntity(const EntityID EntityId);
	bool HasEntity(const EntityID EntityId);
	bool RemoveEntity(const EntityID EntityId);

	ESystemType GetId();

	bool FitsRequirements(const Bitmask& bits);
	void Purge();

	virtual void Update(float DeltaTime) = 0;
	virtual void HandleEvent(const EntityID EntityId, const EntityEvent EventType) = 0;
protected:

	ESystemType mId;
	Requirements mRequiredComponents;
	EntityList mEntities;
	SystemManager* mSystemManager;
};