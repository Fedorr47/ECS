#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemPosses : public SystemBase{
public:
	SystemPosses(SystemManager* InSystemManager);
	~SystemPosses();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
private:
	void AddOwnerEntity(const EntityID EntityId, const EntityID OwnerEntityId);
	void AddChildEntity(const EntityID EntityId, const EntityID ChildEntityId);
	void CheckDamagePossability(const EntityID FirstEntityId, const EntityID SecondEntityId, int InDamageValue);
	void NotifyOwnerAboutDestroy(const EntityID EntityId, int InValue);
	void NotifyChildAboutHit(const EntityID EntityId);
	void NotifyChildAboutDestroyed(const EntityID EntityId);
};