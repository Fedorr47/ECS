#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemPosition : public SystemBase {
public:
	SystemPosition(SystemManager* InSystemManager);
	~SystemPosition();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventType);
	void Notify(const Message& msg);
private:
	void MovePosition(const EntityID EntityId, const sf::Vector2f& InPosVec);
	void SetPosition(const EntityID EntityId, const sf::Vector2f& InPosVec);
	void ChangeRotation(const EntityID EntityId, const float InAngle);
	void RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds);
	void RespawnEnttity(const EntityID EntityId);
};