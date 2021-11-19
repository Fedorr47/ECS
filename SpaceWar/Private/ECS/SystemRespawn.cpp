#include "stdafx.h"

#include "ECS/Systems/SystemRespawn.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentRespawn.h"

SystemRespawn::SystemRespawn(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Respawn, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Respawn));
	mRequiredComponents.push_back(req);

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedDefaultRespawnCounter, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::MarkedToRemove, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemRespawn::~SystemRespawn(){}

void SystemRespawn::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for (auto& Entity : mEntities) 
	{
		if (EntitiesToRespawn.count(Entity) != 0)
		{
			ComponentRespawn* CheckRespawnPossabilityPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentRespawn>(Entity, EComponentType::Respawn);
			if (CheckRespawnPossabilityPtr != nullptr && CheckRespawnPossabilityPtr->GetCurrentRespawnCounter() <= sf::Time::Zero)
			{
				NotifyRespawn(Entity);
				CheckRespawnPossabilityPtr->SetCurrentRespawnCounter(CheckRespawnPossabilityPtr->GetDefaultRespawnCounter());
				EntitiesToRespawn.erase(Entity);
			}
			else
			{
				CheckRespawnPossabilityPtr->SetCurrentRespawnCounter(CheckRespawnPossabilityPtr->GetCurrentRespawnCounter() - sf::seconds(DeltaTime));
			}
		}
	}
}

void SystemRespawn::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemRespawn::Notify(const Message& msg)
{	
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
	case EntityMessage::MarkedToRemove:
		CheckRespawnPossability(msg.mSender);
		break;
	case EntityMessage::ChangedDefaultRespawnCounter:
		ChangeDefaultReSpawnCounter(msg.mSender, msg.m_float);
		break;
	}
}

void SystemRespawn::NotifyRespawn(const EntityID EntityId)
{
	Message msg;

	msg.mType = static_cast<MessageType>(EntityMessage::Respawn);
	msg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(msg);
}

void SystemRespawn::CheckRespawnPossability(const EntityID EntityId)
{
	ComponentRespawn* CheckRespawnPossabilityPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentRespawn>(EntityId, EComponentType::Respawn);
	Message msg;
	if (CheckRespawnPossabilityPtr != nullptr)
	{
		msg.mType = static_cast<MessageType>(EntityMessage::Despawn);
		msg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(msg);

		EntitiesToRespawn.insert(EntityId);
	}
	else
	{
		mSystemManager->GetEntityManager()->RemoveEntity(EntityId);
	}
}

void SystemRespawn::ChangeDefaultReSpawnCounter(const EntityID EntityId, const float InRespawnCounter)
{
	ComponentRespawn* ComponentRespawnPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentRespawn>(EntityId, EComponentType::Respawn);
	if (ComponentRespawnPtr != nullptr)
		ComponentRespawnPtr->SetDefaultRespawnCounter(sf::seconds(InRespawnCounter));
}



