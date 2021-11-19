#pragma once

#include <unordered_map>

#include "ECS/Systems/SystemBase.h"

enum class ESpawnedType : uint8_t
{
	Actor,
	Projectile,
	MAX
};

enum class ERenderPriority : uint8_t;
enum class EResourceTag : uint8_t;
enum class ECombatEntityType : uint8_t;

class SystemSpawn : public SystemBase 
{
	std::unordered_map<EntityID, std::unordered_map<ECombatEntityType, std::shared_ptr<struct ActorOptions>>> EntitiesSpawnOptions;

public:
	SystemSpawn(SystemManager* InSystemManager);
	~SystemSpawn();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventtType);
	void Notify(const Message& msg);

	void AddSpawnOptions(const EntityID OwnerEntityId, ECombatEntityType EntityType, struct ActorOptions& SpawnOptions);
	const EntityID SpawnActor(const EntityID OwnerEntityId, struct ActorOptions& SpawnOptions);
	void SpawnProjectile(const EntityID OwnerEntityId);
	void AttachParticle(const EntityID OwnerEntityId, struct ActorOptions& SpawnOptions);
	void SpawnParticle(const EntityID OwnerEntityId, struct ActorOptions& SpawnOptions);
private:
	void InitPosition(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitMovable(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitDrawble(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitLifetime(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitProjectileRate(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitRespawn(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitHealth(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);
	void InitDamage(const EntityID SenderEntityId, struct ActorOptions& SpawnOptions);

	void ChangeEntityInitPosition(const EntityID SenderEntityId, sf::Vector2f InInitPos);
	void ChangeEntityInitRotation(const EntityID SenderEntityId, float InAngle);
};