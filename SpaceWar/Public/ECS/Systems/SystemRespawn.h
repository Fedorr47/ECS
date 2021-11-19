#pragma once

#include <set>

#include "ECS/Systems/SystemBase.h"

class SystemRespawn : public SystemBase{

	std::set<EntityID> EntitiesToRespawn;

public:
	SystemRespawn(SystemManager* InSystemManager);
	~SystemRespawn();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
private:
	void NotifyRespawn(const EntityID EntityId);
	void CheckRespawnPossability(const EntityID EntityId);
	void ChangeDefaultReSpawnCounter(const EntityID EntityId, const float InRespawnCounter);
};