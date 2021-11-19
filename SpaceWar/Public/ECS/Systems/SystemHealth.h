#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemHealth : public SystemBase {
public:
	SystemHealth(SystemManager* InSystemManager);
	~SystemHealth();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
	float GetHealthPercent(const EntityID EntityId);
private:
	void PerformDamage(const EntityID FirstEntityId, const EntityID SecondEntityId, int InDamage);
	void ChangeMaxHealthPoints(const EntityID EntityId, const int InHealthPoints);
	void ChangeCurrentHealthPoints(const EntityID EntityId, const int InHealthPoints);
	void RestoreHealthPoints(const EntityID EntityId);
};