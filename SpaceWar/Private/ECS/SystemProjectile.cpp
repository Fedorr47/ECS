#include "stdafx.h"

#include "ECS/Systems/SystemProjectile.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "GameData/Utils.h"

#include "ECS/Components/ComponentProjectile.h"

SystemProjectile::SystemProjectile(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Projectile, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Projectile));
	mRequiredComponents.push_back(req);

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedEntityBounds, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedDefaultProjectileSpawnRate, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedProjectileSpawnRate, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedPosition, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedRotation, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::AttackAction, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemProjectile::~SystemProjectile(){}

void SystemProjectile::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for(auto& Entity : mEntities)
	{
		ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(Entity, EComponentType::Projectile);
		if (ComponentProjectilePtr != nullptr)
		{
			auto Projectile = ProjectilesToSpawn.find(Entity);
			if (Projectile != ProjectilesToSpawn.end())
			{

				if (ComponentProjectilePtr != nullptr && ComponentProjectilePtr->GetCurrentSpawnTimeRate() <= sf::Time::Zero)
				{
					NotifyToSpawnProjectile(Entity, (*Projectile).second);
					ComponentProjectilePtr->SetCurrentSpawnTimeRate(ComponentProjectilePtr->GetDefaultSpawnTimeRate());
					ProjectilesToSpawn.erase(Projectile);
				}
				else
				{
					ProjectilesToSpawn.erase(Projectile);
				}
			}
			ComponentProjectilePtr->SetCurrentSpawnTimeRate(ComponentProjectilePtr->GetCurrentSpawnTimeRate() - sf::seconds(DeltaTime));
		}
	}
}

void SystemProjectile::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
	
	switch (EventType)
	{
	case EntityEvent::BeganAttack:
		ProjectilesToSpawn.emplace(EntityId, EProjectileType::Laser);
		break;
	}
}

void SystemProjectile::Notify(const Message& msg)
{	
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
	case EntityMessage::Despawn:
		RemoveEntity(msg.mSender);
		break;
	case EntityMessage::Respawn:
		AddEntity(msg.mSender);
		break;
	case EntityMessage::ChangedPosition:
		ChangePosition(msg.mSender, sf::Vector2f(msg.m_2f.m_x, msg.m_2f.m_y));
		break;
	case EntityMessage::ChangedRotation:
		ChangeRotation(msg.mSender, msg.m_float);
		break;
	case EntityMessage::ChangedEntityBounds:
		RefreshEntityBounds(msg.mSender, sf::FloatRect(msg.m_4f.x, msg.m_4f.y, msg.m_4f.w, msg.m_4f.h));
		break;
	case EntityMessage::ChangedDefaultProjectileSpawnRate:
		ChangeDefaultSpawnRate(msg.mSender, msg.m_float);
		break;
	case EntityMessage::ChangedProjectileSpawnRate:
		ChangeSpawnRate(msg.mSender, msg.m_float);
		break;
	}
}

void SystemProjectile::ChangePosition(
	const EntityID EntityId,
	const sf::Vector2f& InPosVec)
{
	ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(EntityId, EComponentType::Projectile);
	if (ComponentProjectilePtr != nullptr)
		ComponentProjectilePtr->SetPosition(InPosVec);
}

void SystemProjectile::ChangeRotation(
	const EntityID EntityId,
	const float InAngle)
{
	ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(EntityId, EComponentType::Projectile);
	if (ComponentProjectilePtr != nullptr)
		ComponentProjectilePtr->SetRotation(InAngle);
}

void SystemProjectile::ChangeDefaultSpawnRate(const EntityID EntityId, const float InRate)
{
	ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(EntityId, EComponentType::Projectile);
	if (ComponentProjectilePtr != nullptr)
		ComponentProjectilePtr->SetDefaultSpawnTimeRate(sf::seconds(InRate));
}

void SystemProjectile::ChangeSpawnRate(const EntityID EntityId, const float InRate)
{
	ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(EntityId, EComponentType::Projectile);
	if (ComponentProjectilePtr != nullptr)
		ComponentProjectilePtr->SetCurrentSpawnTimeRate(sf::seconds(InRate));
}

void SystemProjectile::RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds)
{
	ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(EntityId, EComponentType::Projectile);
	if (ComponentProjectilePtr != nullptr)
		ComponentProjectilePtr->SetBounds(InBounds);
}

void SystemProjectile::NotifyToSpawnProjectile(const EntityID EntityId, EProjectileType EventType)
{
	ComponentProjectile* ComponentProjectilePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentProjectile>(EntityId, EComponentType::Projectile);
	if (ComponentProjectilePtr != nullptr)
	{
		auto CurrentPosition = ComponentProjectilePtr->GetPosition();
		auto CurrentRotation = ComponentProjectilePtr->GetRotation();

		auto OwnerRotation = Utils::toRadian(CurrentRotation + 90.0f);
		auto OwnerBounds = ComponentProjectilePtr->GetBounds();
		float vx = OwnerBounds.width / 2 * std::cos(OwnerRotation);
		float vy = OwnerBounds.height / 2 * std::sin(OwnerRotation);
		CurrentPosition.x += vx * -1.0f;
		CurrentPosition.y += vy * -1.0f;

		Message msg(static_cast<MessageType>(EntityMessage::ChangedProjectilePosition));
		msg.mSender = EntityId;
		msg.m_2f.m_x = CurrentPosition.x;
		msg.m_2f.m_y = CurrentPosition.y;
		mSystemManager->GetMessageHandler()->Dispatch(msg);

		msg.mType = static_cast<MessageType>(EntityMessage::ChangedProjectileRotation);
		msg.mSender = EntityId;
		msg.m_float = CurrentRotation;
		mSystemManager->GetMessageHandler()->Dispatch(msg);

		msg.mType = static_cast<MessageType>(EntityMessage::Spawn);
		msg.m_int = static_cast<int>(ECombatEntityType::Projectile);
		msg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(msg);
	}
}
