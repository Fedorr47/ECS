#include "stdafx.h"

#include "ECS/Systems/SystemLifetime.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"

#include "ECS/Components/ComponentLifetime.h"

SystemLifetime::SystemLifetime(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Lifetime,InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Lifetime));
	mRequiredComponents.push_back(req);

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedDefaultLifetime, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemLifetime::~SystemLifetime(){}

void SystemLifetime::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for(auto& Entity : mEntities)
	{
		CheckLifeTime(Entity, DeltaTime);
	}
}

void SystemLifetime::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemLifetime::Notify(const Message& msg)
{	
	if (!HasEntity(msg.mSender))
	{ 
		return; 
	}
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
		case EntityMessage::ChangedDefaultLifetime:
			SetDefaultLifetime(msg.mSender, msg.m_float);
		break;
		case EntityMessage::Despawn:
			RemoveEntity(msg.mSender);
			break;
		case EntityMessage::Respawn:
			AddEntity(msg.mSender);
			break;
	}
}

void SystemLifetime::SetDefaultLifetime(
	const EntityID EntityId, 
	const float InLifetime)
{
	ComponentLifetime* ComponentLifetimePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentLifetime>(EntityId, EComponentType::Lifetime);
	if (ComponentLifetimePtr != nullptr)
		ComponentLifetimePtr->SetDefaultLifeTime(sf::seconds(InLifetime));
}

void SystemLifetime::CheckLifeTime(
	const EntityID EntityId,
	float DeltaTime)
{
	ComponentLifetime* ComponentLifetimePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentLifetime>(EntityId, EComponentType::Lifetime);
	if (ComponentLifetimePtr != nullptr)
	{
		if (ComponentLifetimePtr->GetDefaultLifeTime() > sf::Time::Zero)
		{
			if (ComponentLifetimePtr->GetCurrentLifeTime() > sf::Time::Zero)
			{
				ComponentLifetimePtr->SetCurrentLifeTime(ComponentLifetimePtr->GetCurrentLifeTime() - sf::seconds(DeltaTime));
			}
			else
			{
				Message msg;
				msg.mType = static_cast<MessageType>(EntityMessage::SwitchState);
				msg.m_int = static_cast<int>(EEntityState::MarkToRemove);
				msg.mSender = EntityId;
				mSystemManager->GetMessageHandler()->Dispatch(msg);
			}
		}
	}
}


