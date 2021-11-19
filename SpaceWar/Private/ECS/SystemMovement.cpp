#include "stdafx.h"

#include "ECS/EntityManager.h"
#include "ECS/Systems/SystemMovement.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/Components/ComponentMovable.h"
#include "GameData/Utils.h"

#include <cmath>

SystemMovement::SystemMovement(SystemManager* InSystemManager)
	: SystemBase(ESystemType::Movement, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Movable));
	mRequiredComponents.push_back(req);
	req.reset();
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedSpeed, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedRotation, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedMaxVelocity, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedInertion, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Move, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::YawLeft, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::YawRight, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemMovement::~SystemMovement() {}

void SystemMovement::Update(float DeltaTime)
{
	EntityManager* Entities = mSystemManager->GetEntityManager();
	for (auto& Entity : mEntities) 
	{
		ComponentMovable* ComponentMovablePtr = Entities->GetComponent<ComponentMovable>(Entity, EComponentType::Movable);
		assert(ComponentMovablePtr != nullptr);
		
		if (ComponentMovablePtr->GetImpulse())
		{
			Message msg(static_cast<MessageType>(EntityMessage::SwitchState));
			msg.m_int = static_cast<int>(EEntityState::Idle);
			msg.mSender = Entity;
			mSystemManager->GetMessageHandler()->Dispatch(msg);
		}
		MovementStep(ComponentMovablePtr);
		SendVelocity(Entity, ComponentMovablePtr->GetVelocity() * DeltaTime);
		SendRotate(Entity, ComponentMovablePtr->GetRotation());
		if (!ComponentMovablePtr->IsInertion())
		{
			ComponentMovablePtr->SetVelocity(0.0f, 0.0f);
		}
	}
}

void SystemMovement::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{}

void SystemMovement::Notify(const Message& msg) 
{
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
		case EntityMessage::ChangedSpeed:
			ChangeSpeed(msg.mSender, msg.m_float);
			break;
		case EntityMessage::ChangedMaxVelocity:
			ChangeMaxVelocity(msg.mSender, msg.m_float);
			break;
		case EntityMessage::ChangedInertion:
			ChangeInertion(msg.mSender, msg.m_bool);
			break;
		case EntityMessage::ChangedRotation:
			ChangeRotation(msg.mSender, msg.m_float);
			break;
		case EntityMessage::Despawn:
			RemoveEntity(msg.mSender);
			break;
		case EntityMessage::Respawn:
			RespawnEnttity(msg.mSender);
			AddEntity(msg.mSender);
			break;
		case EntityMessage::Move:
			Move(msg.mSender);
			break;
		case EntityMessage::YawLeft:
			Rotate(msg.mSender, EDirection::Left);
			break;
		case EntityMessage::YawRight:
			Rotate(msg.mSender, EDirection::Right);
			break;
	}
}

void SystemMovement::MovementStep(ComponentMovable* InComponentMovable)
{
	auto Shift = InComponentMovable->GetVelocity();
	bool Impulse = InComponentMovable->GetImpulse();

	if (Impulse)
	{
		float radians = Utils::toRadian(InComponentMovable->GetRotation() + 90.0f);

		float vx = InComponentMovable->GetSpeed() * std::cos(radians);
		float vy = InComponentMovable->GetSpeed() * std::sin(radians);

		Shift = Shift + sf::Vector2f(-1.0f * vx, -1.0f * vy);

		InComponentMovable->SetImpulse(false);
		InComponentMovable->SetVelocity(Shift);
	}
}

void SystemMovement::ChangeSpeed(
	const EntityID EntityId,
	float InSpeed)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
		ComponentMovablePtr->SetSpeed(InSpeed);
}

void SystemMovement::ChangeMaxVelocity(
	const EntityID EntityId,
	float InMaxVelocity)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
		ComponentMovablePtr->SetMaxVelocity(InMaxVelocity);
}

void SystemMovement::ChangeInertion(
	const EntityID EntityId,
	float InIsInertion)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
		ComponentMovablePtr->SetInertion(InIsInertion);
}

void SystemMovement::ChangeRotation(const EntityID EntityId, const float InAngle)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
	{
		ComponentMovablePtr->SetRotation(InAngle);
	}
	
}

void SystemMovement::Move(const EntityID EntityId)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
		ComponentMovablePtr->SetImpulse();
}

void SystemMovement::Rotate(const EntityID EntityId, const EDirection Direction)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
		ComponentMovablePtr->SetRotation(Direction);
}

void SystemMovement::SendVelocity(const EntityID EntityId, const sf::Vector2f& InVelVec)
{
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::Moved);
	TmpMsg.m_2f.m_x = InVelVec.x;
	TmpMsg.m_2f.m_y = InVelVec.y;
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemMovement::SendRotate(const EntityID EntityId, const float InAngle)
{
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::Rotated);
	TmpMsg.m_float = InAngle;
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemMovement::RespawnEnttity(const EntityID EntityId)
{
	ComponentMovable* ComponentMovablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentMovable>(EntityId, EComponentType::Movable);
	if (ComponentMovablePtr != nullptr)
	{
		ComponentMovablePtr->SetVelocity(sf::Vector2f(0.0f, 0.0f));
		ComponentMovablePtr->SetImpulse(false);
	}
}
