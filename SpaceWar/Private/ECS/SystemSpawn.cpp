#include "stdafx.h"

#include "ECS/Systems/SystemSpawn.h"

#include "ECS/EntityManager.h"
#include "ECS/Systems/SystemManager.h"

#include "GameData/TextureManager.h"

#define PLAYER_ACTOR_MASK 16367
#define PROJECTILE_MASK 4783
#define PARTICLE_MASK 8259

SystemSpawn::SystemSpawn(SystemManager* InSystemManager) : 
	SystemBase(ESystemType::Spawn, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Sprite));
	req.set(static_cast<unsigned int>(EComponentType::Position));
	mRequiredComponents.push_back(req);
	req.reset();

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedProjectilePosition, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedProjectileRotation, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Spawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemSpawn::~SystemSpawn() {}

void SystemSpawn::Update(float DeltaTime) 
{}

void SystemSpawn::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{}

void SystemSpawn::Notify(const Message& msg) {
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
	case EntityMessage::Spawn:
		if (static_cast<ECombatEntityType>(msg.m_int) == ECombatEntityType::Projectile)
		{
			SpawnProjectile(msg.mSender);
		}
		break;
	case EntityMessage::ChangedProjectilePosition:
		ChangeEntityInitPosition(msg.mSender, sf::Vector2f(msg.m_2f.m_x, msg.m_2f.m_y));
		break;
	case EntityMessage::ChangedProjectileRotation:
		ChangeEntityInitRotation(msg.mSender, msg.m_float);
		break;
	}
}


void SystemSpawn::AddSpawnOptions(
	const EntityID OwnerEntityId,
	ECombatEntityType EntityType, 
	ActorOptions& SpawnOptions)
{
	auto ptr = std::make_shared<ActorOptions>(SpawnOptions);
	if (EntitiesSpawnOptions.find(OwnerEntityId) != EntitiesSpawnOptions.end())
	{
		auto exist_second = (*EntitiesSpawnOptions.find(OwnerEntityId)).second.find(EntityType);

		if (exist_second != (*EntitiesSpawnOptions.find(OwnerEntityId)).second.end())
		{
			(*exist_second).second = ptr;
		}
		else
		{
			(*EntitiesSpawnOptions.find(OwnerEntityId)).second.emplace(EntityType, ptr);
		}
	}
	else
	{
		std::unordered_map<ECombatEntityType, std::shared_ptr<struct ActorOptions>> second_map;
		second_map.emplace(EntityType, ptr);
		EntitiesSpawnOptions.emplace(OwnerEntityId, second_map);
	}
}

const EntityID SystemSpawn::SpawnActor(
	const EntityID OwnerEntityId, 
	ActorOptions& SpawnOptions)
{
	EntityManager* entityManager = mSystemManager->GetEntityManager();
	EntityID entityId = entityManager->AddEntity(PLAYER_ACTOR_MASK);

	assert(entityId != -1);

	InitPosition(entityId, SpawnOptions);
	InitMovable(entityId, SpawnOptions);
	InitDrawble(entityId, SpawnOptions);
	InitLifetime(entityId, SpawnOptions);
	InitProjectileRate(entityId, SpawnOptions);
	InitHealth(entityId, SpawnOptions);
	InitRespawn(entityId, SpawnOptions);
	InitDamage(entityId, SpawnOptions);

	return entityId;
}

void SystemSpawn::SpawnProjectile(
	const EntityID OwnerEntityId)
{
	EntityManager* entityManager = mSystemManager->GetEntityManager();
	EntityID entityId = entityManager->AddEntity(PROJECTILE_MASK);

	assert(entityId != -1);

	ActorOptions Options;
	if (EntitiesSpawnOptions.find(OwnerEntityId) != EntitiesSpawnOptions.end())
	{
		auto OptionHolder = (*EntitiesSpawnOptions.find(OwnerEntityId)).second.find(ECombatEntityType::Projectile);
		if (OptionHolder != (*EntitiesSpawnOptions.find(OwnerEntityId)).second.end())
		{
			Options = *(*OptionHolder).second;
		}
	}

	InitPosition(entityId, Options);
	InitMovable(entityId, Options);
	InitDrawble(entityId, Options);
	InitLifetime(entityId, Options);
	InitDamage(entityId, Options);
	InitRespawn(entityId, Options);

	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::AddOwner);
	TmpMsg.mSender = entityId;
	TmpMsg.mReceiver = OwnerEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::Move);
	TmpMsg.mSender = entityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::AttachParticle(
	const EntityID OwnerEntityId,
	ActorOptions& SpawnOptions)
{
	EntityManager* entityManager = mSystemManager->GetEntityManager();
	entityManager->AddComponent(OwnerEntityId, EComponentType::Particle);
	
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::AttachParticle);
	TmpMsg.m_int = static_cast<int>(SpawnOptions.ResourceTag);
	TmpMsg.mSender = OwnerEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::SpawnParticle(const EntityID OwnerEntityId, ActorOptions& SpawnOptions)
{
	EntityManager* entityManager = mSystemManager->GetEntityManager();
	EntityID entityId = entityManager->AddEntity(PARTICLE_MASK);

	assert(entityId != -1);

	InitPosition(entityId, SpawnOptions);
	InitDrawble(entityId, SpawnOptions);
	InitDamage(entityId, SpawnOptions);
	InitRespawn(entityId, SpawnOptions);

	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::AddChild);
	TmpMsg.mSender = OwnerEntityId;
	TmpMsg.mReceiver = entityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::AttachParticle);
	TmpMsg.m_int = static_cast<int>(SpawnOptions.ResourceTag);
	TmpMsg.mSender = entityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitPosition(
	const EntityID SenderEntityId,
	ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedPosition);
	TmpMsg.m_2f.m_x = SpawnOptions.Position.x;
	TmpMsg.m_2f.m_y = SpawnOptions.Position.y;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedRotation);
	TmpMsg.m_float = SpawnOptions.Angle;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitMovable(
	const EntityID SenderEntityId,
	ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedSpeed);
	TmpMsg.m_float = SpawnOptions.Speed;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedMaxVelocity);
	TmpMsg.m_float = SpawnOptions.MaxVelocity;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedInertion);
	TmpMsg.m_bool = SpawnOptions.IsInertion;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

}

void SystemSpawn::InitDrawble(
	const EntityID SenderEntityId,
	ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedSprite);
	TmpMsg.m_int = static_cast<int>(SpawnOptions.ResourceTag);
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedRenderPriority);
	TmpMsg.m_int = static_cast<int>(SpawnOptions.RenderPriority);
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitLifetime(const EntityID SenderEntityId, ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedDefaultLifetime);
	TmpMsg.m_float = SpawnOptions.LifeTime;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitProjectileRate(
	const EntityID SenderEntityId, 
	ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedDefaultProjectileSpawnRate);
	TmpMsg.m_float = SpawnOptions.ProjectileSpawnRate;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitRespawn(const EntityID SenderEntityId, ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedDefaultRespawnCounter);
	TmpMsg.m_float = SpawnOptions.RespawnCounter;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitHealth(const EntityID SenderEntityId, ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangeMaxHealthPoints);
	TmpMsg.m_int = SpawnOptions.HealthPoints;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::InitDamage(const EntityID SenderEntityId, ActorOptions& SpawnOptions)
{
	Message TmpMsg;

	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangeDefaultDamageValue);
	TmpMsg.m_int = SpawnOptions.DamageValue;
	TmpMsg.mSender = SenderEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemSpawn::ChangeEntityInitPosition(const EntityID SenderEntityId, sf::Vector2f InInitPos)
{
	if (EntitiesSpawnOptions.find(SenderEntityId) != EntitiesSpawnOptions.end())
	{
		auto OptionHolder = (*EntitiesSpawnOptions.find(SenderEntityId)).second.find(ECombatEntityType::Projectile);
		if (OptionHolder != (*EntitiesSpawnOptions.find(SenderEntityId)).second.end())
		{
			OptionHolder->second->Position = InInitPos;
		}
	}
}

void SystemSpawn::ChangeEntityInitRotation(const EntityID SenderEntityId, float InAngle)
{
	if (EntitiesSpawnOptions.find(SenderEntityId) != EntitiesSpawnOptions.end())
	{
		auto OptionHolder = (*EntitiesSpawnOptions.find(SenderEntityId)).second.find(ECombatEntityType::Projectile);
		if (OptionHolder != (*EntitiesSpawnOptions.find(SenderEntityId)).second.end())
		{
			OptionHolder->second->Angle = InAngle;
		}
	}
}


