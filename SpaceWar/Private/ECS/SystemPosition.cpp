#include "stdafx.h"

#include "ECS/Systems/SystemPosition.h"
#include "ECS/Components/ComponentPosition.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "GameData/SharedContext.h"
#include "GameData/Utils.h"

SystemPosition::SystemPosition(SystemManager* InSystemManager)
	: SystemBase(ESystemType::Position, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Position));
	mRequiredComponents.push_back(req);
	req.reset();
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedPosition, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedEntityBounds, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Wraparound, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Moved, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Rotated, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemPosition::~SystemPosition() {}

void SystemPosition::Update(float DeltaTime) 
{}

void SystemPosition::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{}

void SystemPosition::Notify(const Message& msg) 
{
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
		case EntityMessage::ChangedPosition:
		case EntityMessage::Wraparound:
			SetPosition(msg.mSender, sf::Vector2f(msg.m_2f.m_x, msg.m_2f.m_y));
			break;
		case EntityMessage::ChangedEntityBounds:
			RefreshEntityBounds(msg.mSender, sf::FloatRect(msg.m_4f.x, msg.m_4f.y, msg.m_4f.w, msg.m_4f.h));
			break;
		case EntityMessage::Despawn:
			RemoveEntity(msg.mSender);
			break;
		case EntityMessage::Respawn:
			RespawnEnttity(msg.mSender);
			AddEntity(msg.mSender);	
			break;
		case EntityMessage::Moved:
			MovePosition(msg.mSender, sf::Vector2f(msg.m_2f.m_x, msg.m_2f.m_y));
			break;
		case EntityMessage::Rotated:
			ChangeRotation(msg.mSender, msg.m_float);
			break;
	}
}

void SystemPosition::RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds)
{
	ComponentPosition* ComponentPositionPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPosition>(EntityId, EComponentType::Position);
	if (ComponentPositionPtr != nullptr)
		ComponentPositionPtr->SetBounds(InBounds);
}

void SystemPosition::RespawnEnttity(const EntityID EntityId)
{
	ComponentPosition* ComponentPositionPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPosition>(EntityId, EComponentType::Position);
	if (ComponentPositionPtr != nullptr)
	{
		EntityManager* Entities = mSystemManager->GetEntityManager();	
		
		auto WindowSize = mSystemManager->GetSharedConxtext()->mWind->GetSize();

		int RandomX = Utils::getRandom(WindowSize.x);
		int RandomY = Utils::getRandom(WindowSize.y);

		ComponentPositionPtr->SetPosition(sf::Vector2f(static_cast<float>(RandomX), static_cast<float>(RandomY)));

		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedPosition);
		TmpMsg.m_2f.m_x = ComponentPositionPtr->GetCurrentPosition().x;
		TmpMsg.m_2f.m_y = ComponentPositionPtr->GetCurrentPosition().y;
		TmpMsg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}

void SystemPosition::SetPosition(
	const EntityID EntityId,
	const sf::Vector2f& InPosVec)
{
	ComponentPosition* ComponentPositionPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPosition>(EntityId, EComponentType::Position);
	if (ComponentPositionPtr != nullptr)
		ComponentPositionPtr->SetPosition(InPosVec);
}

void SystemPosition::MovePosition(
	const EntityID EntityId,
	const sf::Vector2f& InPosVec)
{
	ComponentPosition* ComponentPositionPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPosition>(EntityId, EComponentType::Position);
	if (ComponentPositionPtr != nullptr)
	{
		ComponentPositionPtr->Move(InPosVec);
		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedPosition);
		TmpMsg.m_2f.m_x = ComponentPositionPtr->GetCurrentPosition().x;
		TmpMsg.m_2f.m_y = ComponentPositionPtr->GetCurrentPosition().y;
		TmpMsg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}

void SystemPosition::ChangeRotation(
	const EntityID EntityId,
	const float InAngle)
{
	ComponentPosition* ComponentPositionPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentPosition>(EntityId, EComponentType::Position);
	if (ComponentPositionPtr != nullptr)
	{
		ComponentPositionPtr->SetRotation(InAngle);
		Message TmpMsg;
		TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedRotation);
		TmpMsg.m_float = InAngle;
		TmpMsg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
}
