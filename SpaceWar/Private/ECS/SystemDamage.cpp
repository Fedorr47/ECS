#include "stdafx.h"

#include "ECS/Systems/SystemDamage.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentDamage.h"

SystemDamage::SystemDamage(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Damage, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Damage));
	mRequiredComponents.push_back(req);

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangeDefaultDamageValue, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangeDamageValue, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Collided, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemDamage::~SystemDamage(){}

void SystemDamage::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for(auto& Entity : mEntities){

	}
}

void SystemDamage::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemDamage::Notify(const Message& msg)
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
	case EntityMessage::Collided:
		CheckDamage(msg.mReceiver, msg.mSender);
		break;
	case EntityMessage::ChangeDefaultDamageValue:
		ChangeDefaultDamageValue(msg.mSender, msg.m_int);
		break;
	case EntityMessage::ChangeDamageValue:
		ChangeCurrentDamageValue(msg.mSender, msg.m_int);
		break;
	}
}

void SystemDamage::CheckDamage(const EntityID FirstEntityId, const EntityID SecondEntityId)
{
	ComponentDamage* FirstComponentDamagePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentDamage>(FirstEntityId, EComponentType::Damage);
	ComponentDamage* SecondComponentDamagePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentDamage>(SecondEntityId, EComponentType::Damage);
	
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::Damage);

	if (FirstComponentDamagePtr != nullptr && FirstComponentDamagePtr->GetCurrentDamageValue() > 0)
	{
		TmpMsg.m_int = FirstComponentDamagePtr->GetCurrentDamageValue();
		TmpMsg.mSender = FirstEntityId;
		TmpMsg.mReceiver = SecondEntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
	if (SecondComponentDamagePtr != nullptr && SecondComponentDamagePtr->GetCurrentDamageValue() > 0)
	{
		TmpMsg.m_int = SecondComponentDamagePtr->GetCurrentDamageValue();
		TmpMsg.mSender = SecondEntityId;
		TmpMsg.mReceiver = FirstEntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}

void SystemDamage::ChangeDefaultDamageValue(const EntityID EntityId, const int InDamageValue)
{
	ComponentDamage* ComponentDamagePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentDamage>(EntityId, EComponentType::Damage);
	if (ComponentDamagePtr != nullptr)
		ComponentDamagePtr->SetDefaultDamageValue(InDamageValue);
}

void SystemDamage::ChangeCurrentDamageValue(const EntityID EntityId, const int InDamageValue)
{
	ComponentDamage* ComponentDamagePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentDamage>(EntityId, EComponentType::Damage);
	if (ComponentDamagePtr != nullptr)
		ComponentDamagePtr->SetCurrentDamageValue(InDamageValue);
}



