#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemLifetime : public SystemBase{
public:
	SystemLifetime(SystemManager* InSystemManager);
	~SystemLifetime();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
private:
	void SetDefaultLifetime(const EntityID EntityId, const float InLifetime);
	void CheckLifeTime(const EntityID EntityId, float DeltaTime);
};