#pragma once
#include "ECS/Systems/SystemBase.h"

class SystemCollision : public SystemBase {
public:
	SystemCollision(SystemManager* InSystemManager);
	~SystemCollision();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventType);
	void Notify(const Message& msg);
private:
	void EntityCollisions();
	void RegisterCollision(
		const EntityID FirstEntityId,
		const EntityID SecondEntityId
	);
	void RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds);
};