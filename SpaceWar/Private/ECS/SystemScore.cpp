#include "stdafx.h"

#include "ECS/Systems/SystemScore.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentScore.h"

SystemScore::SystemScore(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Score, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Score));
	mRequiredComponents.push_back(req);

	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::AsdScore, this);
}

SystemScore::~SystemScore(){}

void SystemScore::Update(float DeltaTime)
{
	EntityManager* entities = mSystemManager->GetEntityManager();
	for(auto& Entity : mEntities){

	}
}

void SystemScore::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
}

void SystemScore::Notify(const Message& msg)
{	
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
	case EntityMessage::AsdScore:
		AddScore(msg.mSender, static_cast<EScoredType>(msg.m_int));
		break;
	}
}

int SystemScore::GetScore(const EntityID EntityId)
{
	ComponentScore* ComponentScorePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentScore>(EntityId, EComponentType::Score);
	if (ComponentScorePtr != nullptr)
	{
		return ComponentScorePtr->GetScore();
	}
	return 0;
}

void SystemScore::AddScore(const EntityID EntityId, const EScoredType ScoreType)
{
	ComponentScore* ComponentScorePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentScore>(EntityId, EComponentType::Score);
	if (ComponentScorePtr != nullptr)
	{
		if (ScoreType == EScoredType::Destroy)
		{
			ComponentScorePtr->SetScore(ComponentScorePtr->GetScore() + 1);
		}
	}
}



