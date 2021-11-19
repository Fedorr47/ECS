#include "stdafx.h"

#include "ECS/Systems/SystemPosses.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentPossesive.h"

SystemPosses::SystemPosses(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Posses, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Posses));
	mRequiredComponents.push_back(req);
	
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Damage, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::NotifyDestroy, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::AddOwner, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::AddChild, this);
}

SystemPosses::~SystemPosses(){}

void SystemPosses::Update(float DeltaTime)
{}

void SystemPosses::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
	switch (EventType)
	{
	case EntityEvent::Hit:
		NotifyChildAboutHit(EntityId);
		break;
	case EntityEvent::Destroyed:
		NotifyChildAboutDestroyed(EntityId);
		break;
	}
}

void SystemPosses::Notify(const Message& msg)
{	
	if (!HasEntity(msg.mSender))
	{
		return;
	}
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
	case EntityMessage::AddOwner:
		AddOwnerEntity(msg.mSender, msg.mReceiver);
		break;
	case EntityMessage::AddChild:
		AddChildEntity(msg.mSender, msg.mReceiver);
		break;
	case EntityMessage::Damage:
		CheckDamagePossability(msg.mSender, msg.mReceiver, msg.m_int);
		break;
	case EntityMessage::NotifyDestroy:
		NotifyOwnerAboutDestroy(msg.mSender, msg.m_int);
		break;
	}
}

void SystemPosses::AddOwnerEntity(
	const EntityID EntityId, 
	const EntityID OwnerEntityId)
{
	ComponentPossesive* ComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(EntityId, EComponentType::Posses);
	if (ComponentPossesivePtr != nullptr)
		ComponentPossesivePtr->SetOwnerEntity(OwnerEntityId);
}

void SystemPosses::AddChildEntity(const EntityID EntityId, const EntityID ChildEntityId)
{
	ComponentPossesive* ComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(EntityId, EComponentType::Posses);
	if (ComponentPossesivePtr != nullptr)
	{
		ComponentPossesivePtr->AddChild(ChildEntityId);
	}
}

void SystemPosses::NotifyOwnerAboutDestroy(const EntityID EntityId, int InValue)
{
	ComponentPossesive* ComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(EntityId, EComponentType::Posses);
	if (ComponentPossesivePtr != nullptr)
	{
		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::AsdScore);
		TmpMsg.m_int = InValue;
		TmpMsg.mSender = ComponentPossesivePtr->GetOwnerEntity();
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

		TmpMsg.mType = static_cast<MessageType>(EntityMessage::AsdScore);
		TmpMsg.m_int = InValue;
		TmpMsg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}

void SystemPosses::NotifyChildAboutHit(const EntityID EntityId)
{
	ComponentPossesive* OwnerComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(EntityId, EComponentType::Posses);
	for (auto Child : OwnerComponentPossesivePtr->GetChilds())
	{
		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::Hit);
		TmpMsg.mSender = EntityId;
		TmpMsg.mReceiver = Child;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}

void SystemPosses::NotifyChildAboutDestroyed(const EntityID EntityId)
{
	ComponentPossesive* OwnerComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(EntityId, EComponentType::Posses);
	for (auto Child : OwnerComponentPossesivePtr->GetChilds())
	{
		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::Destroyed);
		TmpMsg.mSender = EntityId;
		TmpMsg.mReceiver = Child;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}

void SystemPosses::CheckDamagePossability(
	const EntityID FirstEntityId,
	const EntityID SecondEntityId, 
	int InDamageValue)
{
	ComponentPossesive* FirstComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(FirstEntityId, EComponentType::Posses);
	ComponentPossesive* SecondComponentPossesivePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPossesive>(SecondEntityId, EComponentType::Posses);
	bool IsOwned = false;
	if (FirstComponentPossesivePtr != nullptr)
	{
		if (FirstComponentPossesivePtr->GetOwnerEntity() == SecondEntityId)
			IsOwned = true;
	}
	if (SecondComponentPossesivePtr != nullptr)
	{
		if (SecondComponentPossesivePtr->GetOwnerEntity() == FirstEntityId)
			IsOwned = true;
	}

	if (!IsOwned)
	{
		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::Damaged);
		TmpMsg.m_int = InDamageValue;
		TmpMsg.mSender = FirstEntityId;
		TmpMsg.mReceiver = SecondEntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}


