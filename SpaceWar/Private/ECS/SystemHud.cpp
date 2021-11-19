#include "stdafx.h"

#include "ECS/Systems/SystemHud.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentHud.h"

SystemHud::SystemHud(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Health, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Health));
	mRequiredComponents.push_back(req);
	
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::InitHud, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Damaged, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemHud::~SystemHud(){}

void SystemHud::Update(float DeltaTime)
{
}

void SystemHud::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemHud::Notify(const Message& msg)
{	
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
	case EntityMessage::InitHud:

		break;
	}
}

void SystemHud::InitHud()
{

}




