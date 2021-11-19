#include "stdafx.h"

#include <iostream>

#include "ECS/Systems/SystemCollision.h"

#include "ECS/EntityManager.h"
#include "ECS/Systems/SystemManager.h"

#include "ECS/Components/ComponentCollidable.h"

SystemCollision::SystemCollision(SystemManager* InSystemManager)
	: SystemBase(ESystemType::Collision, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Collidable));
	mRequiredComponents.push_back(req);
	req.reset();
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedEntityBounds, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemCollision::~SystemCollision() {}


void SystemCollision::Update(float DeltaTime) 
{
	EntityManager* Entities = mSystemManager->GetEntityManager();
	for (auto& Entity : mEntities)
	{
		ComponentCollidable* EntityCol = Entities->GetComponent<ComponentCollidable>(Entity, EComponentType::Collidable);
		EntityCol->ResetCollisionFlags();
	}
	EntityCollisions();
}

void SystemCollision::HandleEvent(
	const EntityID EntityId, 
	const EntityEvent EventType) 
{}

void SystemCollision::Notify(const Message& msg)
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
	case EntityMessage::ChangedEntityBounds:
		RefreshEntityBounds(msg.mSender, sf::FloatRect(msg.m_4f.x, msg.m_4f.y, msg.m_4f.w, msg.m_4f.h));
		break;
	}
}

void SystemCollision::EntityCollisions() 
{
	EntityManager* EntityManager = mSystemManager->GetEntityManager();
	for (auto FirstEntityItr = mEntities.begin(); FirstEntityItr != mEntities.end(); ++FirstEntityItr)
	{
		for (auto SecondEntityItr = std::next(FirstEntityItr); SecondEntityItr != mEntities.end(); ++SecondEntityItr) {

			ComponentCollidable* EntityFirstCol = EntityManager->GetComponent<ComponentCollidable>(*FirstEntityItr, EComponentType::Collidable);
			ComponentCollidable* EntitySecondCol = EntityManager->GetComponent<ComponentCollidable>(*SecondEntityItr, EComponentType::Collidable);

			assert((EntityFirstCol != nullptr || EntitySecondCol != nullptr));
			
			sf::FloatRect FirstCol = EntityFirstCol->GetCollidable();
			sf::FloatRect SecondCol = EntitySecondCol->GetCollidable();

			if (FirstCol.intersects(SecondCol))
			{
				RegisterCollision(*FirstEntityItr, *SecondEntityItr);
			}
		}
	}
}

void SystemCollision::RegisterCollision(
	const EntityID FirstEntityId,
	const EntityID SecondEntityId)
{
	Message msg;

	msg.mType = static_cast<MessageType>(EntityMessage::Collided);
	msg.mSender = FirstEntityId;
	msg.mReceiver = SecondEntityId;
	mSystemManager->GetMessageHandler()->Dispatch(msg);
}

void SystemCollision::RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds)
{
	ComponentCollidable* ComponentCollidablePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentCollidable>(EntityId, EComponentType::Collidable);
	if (ComponentCollidablePtr != nullptr)
		ComponentCollidablePtr->SetCollidable(InBounds);
}

