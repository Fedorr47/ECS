#pragma once
#include "ECS/Systems/SystemBase.h"
#include "ECS/Components/ComponentState.h"

class SystemState : public SystemBase{
public:
	SystemState(SystemManager* InSystemManager);
	~SystemState();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
private:
	void ChangeState(const EntityID EntityId, const EEntityState EntityState);
	void NotifyRespawn(const EntityID EntityId, const EEntityState EntityState);
};