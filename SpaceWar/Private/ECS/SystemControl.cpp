#include "stdafx.h"

#include "ECS/Systems/SystemControl.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"

SystemControl::SystemControl(SystemManager* InSystemManager)
	: SystemBase(ESystemType::Control, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Controller));
	mRequiredComponents.push_back(req);
	req.reset();
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ControlInput, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemControl::~SystemControl() {}

void SystemControl::Update(float DeltaTime) {}

void SystemControl::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemControl::Notify(const Message& msg) 
{
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
		case EntityMessage::ControlInput:
			PerformMove(msg.mSender, static_cast<EDirection>(msg.m_int));
		break;
		case EntityMessage::Despawn:
			RemoveEntity(msg.mSender);
			break;
		case EntityMessage::Respawn:
			AddEntity(msg.mSender);
			break;
	}
}

void SystemControl::PerformMove(const EntityID EntityId, const EDirection Direction)
{
	switch (Direction)
	{
	case EDirection::Up:
		MoveEntity(EntityId, Direction);
		break;
	case EDirection::Left:
	case EDirection::Right:
		RotateEntity(EntityId, Direction);
		break;
	}
}

void SystemControl::MoveEntity(
	const EntityID EntityId,
	const EDirection Direction
)
{
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::Move);
	TmpMsg.m_int = static_cast<int>(Direction);
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemControl::RotateEntity(
	const EntityID EntityId, 
	const EDirection Direction)
{
	Message TmpMsg;
	switch (Direction)
	{
		case EDirection::Left:
			TmpMsg.mType = static_cast<MessageType>(EntityMessage::YawLeft);
			break;
		case EDirection::Right:
			TmpMsg.mType = static_cast<MessageType>(EntityMessage::YawRight);
			break;
	}
	
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}
