#include "stdafx.h"

#include "ECS/Systems/SystemState.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"

SystemState::SystemState(SystemManager* InSystemManager) :
	SystemBase(ESystemType::State,InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::State));
	mRequiredComponents.push_back(req);

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Move, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::SwitchState, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemState::~SystemState(){}

void SystemState::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for(auto& Entity : mEntities){
		ComponentState* ComponentStatePtr = entities->GetComponent<ComponentState>(Entity, EComponentType::State);
		if (ComponentStatePtr == nullptr)
			break;
		Message msg;

		switch (ComponentStatePtr->GetState())
		{
			case EEntityState::MarkToRemove:
				NotifyRespawn(Entity, EEntityState::MarkToRemove);
				break;
		}
	}
}

void SystemState::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{

}

void SystemState::Notify(const Message& msg)
{	
	if (!HasEntity(msg.mSender))
	{ 
		return; 
	}
	EntityMessage msgType = static_cast<EntityMessage>(msg.mType);
	switch(msgType)
	{
		case EntityMessage::Move:
			{
				ComponentState* state = mSystemManager->GetEntityManager()->
					GetComponent<ComponentState>(msg.mSender,EComponentType::State);

				if (state->GetState() == EEntityState::WaitRespawn)
				{ 
					return; 
				}
				EntityEvent event(EntityEvent::MAX);
				if (msg.m_int == static_cast<int>(EDirection::Up))
				{
					event = EntityEvent::Moving_Up;
				} 
				else if(msg.m_int == static_cast<int>(EDirection::Left))
				{
					event = EntityEvent::Moving_Left;
				} 
				else if (msg.m_int == static_cast<int>(EDirection::Right)){
					event = EntityEvent::Moving_Right;
				}

				mSystemManager->AddEvent(msg.mSender, static_cast<EventID>(event));
				//ChangeState(msg.mSender, EEntityState::Moving);
			}
			break;
		case EntityMessage::SwitchState: 
			ChangeState(msg.mSender, static_cast<EEntityState>(msg.m_int));
			break;
		case EntityMessage::Despawn:
			//RemoveEntity(msg.mSender);
			ChangeState(msg.mSender, EEntityState::WaitRespawn);
			break;
		case EntityMessage::Respawn:
			//AddEntity(msg.mSender);
			ChangeState(msg.mSender, EEntityState::Idle);
			break;
	}
}

void SystemState::ChangeState(
	const EntityID EntityId,
	const EEntityState EntityState)
{
	ComponentState* state = mSystemManager->GetEntityManager()->GetComponent<ComponentState>(EntityId, EComponentType::State);
	if (state != nullptr)
	{
		state->SetState(EntityState);
	}
}

void SystemState::NotifyRespawn(const EntityID EntityId, const EEntityState EntityState)
{
	Message msg;
	if (EntityState == EEntityState::MarkToRemove)
	{
		msg.mType = static_cast<MessageType>(EntityMessage::MarkedToRemove);
		msg.mSender = EntityId;
		mSystemManager->GetMessageHandler()->Dispatch(msg);
	}
}

