#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemControl : public SystemBase {
public:
	SystemControl(SystemManager* InSystemManager);
	~SystemControl();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventType);
	void Notify(const Message& msg);
private:
	void PerformMove(const EntityID EntityId, const EDirection Direction);
	void MoveEntity(const EntityID EntityId, const EDirection Direction);
	void RotateEntity(const EntityID EntityId, const EDirection Direction);
};