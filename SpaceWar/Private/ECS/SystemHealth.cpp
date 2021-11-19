#include "stdafx.h"

#include "ECS/Systems/SystemHealth.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentHealth.h"

SystemHealth::SystemHealth(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Health, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Health));
	mRequiredComponents.push_back(req);
	
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangeMaxHealthPoints, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Damaged, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemHealth::~SystemHealth(){}

void SystemHealth::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for (auto& Entity : mEntities)
	{
	}
}

void SystemHealth::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemHealth::Notify(const Message& msg)
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
		RestoreHealthPoints(msg.mSender);
		break;
	case EntityMessage::ChangeMaxHealthPoints:
		ChangeMaxHealthPoints(msg.mSender, msg.m_int);
		break;
	case EntityMessage::ChangeHealthpoints:
		ChangeCurrentHealthPoints(msg.mSender, msg.m_int);
		break;
	case EntityMessage::Damaged:
		PerformDamage(msg.mSender, msg.mReceiver, msg.m_int);
		break;
	}
}

float SystemHealth::GetHealthPercent(const EntityID EntityId)
{
	ComponentHealth* ComponentHealthPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentHealth>(EntityId, EComponentType::Health);
	if (ComponentHealthPtr != nullptr)
	{
		return static_cast<float>(ComponentHealthPtr->GetHealth()) / static_cast<float>(ComponentHealthPtr->GetMaxHealth());
	}
	return 0.0f;
}

void SystemHealth::PerformDamage(const EntityID FirstEntityId, const EntityID SecondEntityId, int InDamage)
{
	ComponentHealth* SecondComponentHealthPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentHealth>(SecondEntityId, EComponentType::Health);
	Message msg;
	if (SecondComponentHealthPtr != nullptr)
	{
 		SecondComponentHealthPtr->SetHealth(SecondComponentHealthPtr->GetHealth() - InDamage);
		if (SecondComponentHealthPtr->GetHealth() <= 0)
		{
			msg.mType = static_cast<MessageType>(EntityMessage::SwitchState);
			msg.m_int = static_cast<int>(EEntityState::MarkToRemove);
			msg.mSender = SecondEntityId;
			mSystemManager->GetMessageHandler()->Dispatch(msg);

			msg.mType = static_cast<MessageType>(EntityMessage::NotifyDestroy);
			msg.m_int = static_cast<int>(EScoredType::Destroy);
			msg.mSender = FirstEntityId;
			mSystemManager->GetMessageHandler()->Dispatch(msg);

			SecondComponentHealthPtr->SetHealth(0);

			EntityEvent event(EntityEvent::Destroyed);
			mSystemManager->AddEvent(SecondEntityId, static_cast<EventID>(event));
		}
		else
		{
			EntityEvent event(EntityEvent::Hit);
			mSystemManager->AddEvent(SecondEntityId, static_cast<EventID>(event));
		}
	}
	ComponentHealth* FirstComponentHealthPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentHealth>(FirstEntityId, EComponentType::Health);
	if (FirstComponentHealthPtr == nullptr)
	{
		msg.mType = static_cast<MessageType>(EntityMessage::SwitchState);
		msg.m_int = static_cast<int>(EEntityState::MarkToRemove);
		msg.mSender = FirstEntityId;
		mSystemManager->GetMessageHandler()->Dispatch(msg);
	}
}

void SystemHealth::ChangeMaxHealthPoints(const EntityID EntityId, const int InHealthPoints)
{
	ComponentHealth* ComponentHealthPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentHealth>(EntityId, EComponentType::Health);
	if (ComponentHealthPtr != nullptr)
		ComponentHealthPtr->SetMaxHealth(InHealthPoints);
}

void SystemHealth::ChangeCurrentHealthPoints(const EntityID EntityId, const int InHealthPoints)
{
	ComponentHealth* ComponentHealthPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentHealth>(EntityId, EComponentType::Health);
	if (ComponentHealthPtr != nullptr)
		ComponentHealthPtr->SetHealth(InHealthPoints);
}

void SystemHealth::RestoreHealthPoints(const EntityID EntityId)
{
	ComponentHealth* ComponentHealthPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentHealth>(EntityId, EComponentType::Health);
	if (ComponentHealthPtr != nullptr)
		ComponentHealthPtr->SetHealth(ComponentHealthPtr->GetMaxHealth());
}


