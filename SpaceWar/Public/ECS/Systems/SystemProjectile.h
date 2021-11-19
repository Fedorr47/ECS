#pragma once

#include <unordered_map>

#include "ECS/Systems/SystemBase.h"

enum class EProjectileType : uint8_t;

class SystemProjectile : public SystemBase {

	std::unordered_map<EntityID, EProjectileType> ProjectilesToSpawn;
	
public:
	SystemProjectile(SystemManager* InSystemManager);
	~SystemProjectile();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventType);
	void Notify(const Message& msg);
private:
	void NotifyToSpawnProjectile(const EntityID EntityId, EProjectileType EventType);
	void ChangePosition(const EntityID EntityId, const sf::Vector2f& InPosVec);
	void ChangeRotation(const EntityID EntityId, const float InAngle);
	void ChangeDefaultSpawnRate(const EntityID EntityId, const float InRate);
	void ChangeSpawnRate(const EntityID EntityId, const float InRate);
	void RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds);
};