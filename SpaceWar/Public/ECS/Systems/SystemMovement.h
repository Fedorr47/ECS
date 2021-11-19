#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemMovement : public SystemBase {
public:
	SystemMovement(SystemManager* InSystemManager);
	~SystemMovement();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventtType);
	void Notify(const Message& msg);
	void Move(const EntityID EntityId);
	void Rotate(const EntityID EntityId, const EDirection Direction);
	
private:
	void ChangeSpeed(const EntityID EntityId, float InSpeed);
	void ChangeMaxVelocity(const EntityID EntityId, float InMaxVelocity);
	void ChangeInertion(const EntityID EntityId, float InIsInertion);
	void ChangeRotation(const EntityID EntityId, const float InAngle);
	void MovementStep(class ComponentMovable* InComponentMovable);
	void SendVelocity(const EntityID EntityId, const sf::Vector2f& InVelVec);
	void SendRotate(const EntityID EntityId, const float InAngle);
	void RespawnEnttity(const EntityID EntityId);
};