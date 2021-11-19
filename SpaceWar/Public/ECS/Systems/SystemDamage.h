#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemDamage : public SystemBase{
public:
	SystemDamage(SystemManager* InSystemManager);
	~SystemDamage();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
private:
	void CheckDamage(const EntityID FirstEntityId, const EntityID SecondEntityId);
	void ChangeDefaultDamageValue(const EntityID EntityId, const int InDamageValue);
	void ChangeCurrentDamageValue(const EntityID EntityId, const int InDamageValue);
};