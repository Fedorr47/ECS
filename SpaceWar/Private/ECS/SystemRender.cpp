#include "stdafx.h"

#include "GameData/Window.h"
#include "ECS/Systems/SystemRender.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentDrawble.h"
#include "ECS/Components/ComponentSprite.h"
#include "GameData/SharedContext.h"


SystemRender::SystemRender(SystemManager* InSystemManager) : 
	SystemBase(ESystemType::Renderer, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Sprite));
	mRequiredComponents.push_back(req);
	req.reset();
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedSprite, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedRenderPriority, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedPosition, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedRotation, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Despawn, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Respawn, this);
}

SystemRender::~SystemRender() {}

void SystemRender::Update(float DeltaTime)
{
	EntityManager* Entities = mSystemManager->GetEntityManager();
	for (auto& Entity : mEntities)
	{
		ComponentDrawable* ComponentDrawablePtr = Entities->GetComponent<ComponentDrawable>(Entity, EComponentType::Sprite);
		auto Bounds = ComponentDrawablePtr->GetGlobalBounds();
		UpdateBounds(Entity, Bounds);
	}
}

void SystemRender::HandleEvent(
	const EntityID EntityId, 
	const EntityEvent EventType) 
{
	switch (EventType)
	{
	case EntityEvent::Spawned:
	case EntityEvent::Respawned:
		SortDrawables();
		break;
	case EntityEvent::Despawned:
		SortDrawables();
		break;
	}
}

void SystemRender::Notify(const Message& msg) 
{
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType) 
	{
		case EntityMessage::ChangedRenderPriority:
			ChangeRenderPriority(msg.mSender, static_cast<ERenderPriority>(msg.m_int));
			break;
		case EntityMessage::Despawn:
			RemoveEntity(msg.mSender);
			break;
		case EntityMessage::Respawn:
			AddEntity(msg.mSender);
			break;
		case EntityMessage::ChangedPosition:
			ChangePosition(msg.mSender, sf::Vector2f(msg.m_2f.m_x, msg.m_2f.m_y));
			break;
		case EntityMessage::ChangedRotation:
			ChangeRotation(msg.mSender, msg.m_float);
			break;
		case EntityMessage::ChangedSprite:
			ChangeSprite(msg.mSender, static_cast<EResourceTag>(msg.m_int));
			break;
	}
}

void SystemRender::ChangeSprite(
	const EntityID EntityId, 
	EResourceTag ResourceTag)
{
	ComponentSprite* ComponentSpritePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentSprite>(EntityId, EComponentType::Sprite);
	if (ComponentSpritePtr != nullptr)
	{
		ComponentSpritePtr->Create(mSystemManager->GetEntityManager()->GetTextureManager(), ResourceTag);
		if (ResourceTag == EResourceTag::BackGround)
		{
			ComponentSpritePtr->SetRepeated(true);
			auto WindowSize = mSystemManager->GetSharedConxtext()->mWind->GetSize();
			ComponentSpritePtr->SetTextureRect(sf::IntRect(0.0f, 0.0f, WindowSize.x*10, WindowSize.y*10));
		}
	}
}

void SystemRender::UpdateBounds(const EntityID EntityId, const sf::FloatRect& Bounds)
{
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedEntityBounds);
	TmpMsg.m_4f.x = Bounds.left;
	TmpMsg.m_4f.y = Bounds.top;
	TmpMsg.m_4f.w = Bounds.width;
	TmpMsg.m_4f.h = Bounds.height;
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemRender::ChangeRenderPriority(
	const EntityID EntityId, 
	ERenderPriority RenderPriority)
{
	ComponentSprite* ComponentSpritePtr = mSystemManager->GetEntityManager()->GetComponent<ComponentSprite>(EntityId, EComponentType::Sprite);
	if (ComponentSpritePtr != nullptr)
		ComponentSpritePtr->SetRenderPriority(RenderPriority);
}

void SystemRender::ChangePosition(
	const EntityID EntityId,
	const sf::Vector2f& InPosVec)
{
	ComponentDrawable* ComponentDrawPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentDrawable>(EntityId, EComponentType::Sprite);
	if (ComponentDrawPtr != nullptr)
		ComponentDrawPtr->UpdatePosition(InPosVec);
}

void SystemRender::ChangeRotation(
	const EntityID EntityId,
	const float InAngle)
{
	ComponentDrawable* ComponentDrawPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentDrawable>(EntityId, EComponentType::Sprite);
	if (ComponentDrawPtr != nullptr)
		ComponentDrawPtr->UpdateAngle(InAngle);
}


void SystemRender::Render(Window* InWindow)
{
	EntityManager* Entities = mSystemManager->GetEntityManager();
	for (auto& Entity : mEntities) {

		ComponentDrawable* ComponentDrawablePtr = nullptr;
		if (Entities->HasComponent(Entity, EComponentType::Sprite))
		{
			ComponentDrawablePtr = Entities->GetComponent<ComponentDrawable>(Entity, EComponentType::Sprite);

			sf::FloatRect drawableBounds;
			drawableBounds.left = ComponentDrawablePtr->GetLocalBoundsPosition().x;
			drawableBounds.top = ComponentDrawablePtr->GetLocalBoundsPosition().y;
			drawableBounds.width = ComponentDrawablePtr->GetSize().x;
			drawableBounds.height = ComponentDrawablePtr->GetSize().y;
			ComponentDrawablePtr->DrawBoundingRect(InWindow->GetRenderWindow());

			ComponentDrawablePtr->Draw(InWindow->GetRenderWindow());
			if (Wraparound(ComponentDrawablePtr, InWindow))
			{
				NotifyWraparound(Entity, ComponentDrawablePtr->GetPosition());
			}
		}
		if (Entities->HasComponent(Entity, EComponentType::Particle))
		{
			ComponentDrawablePtr = Entities->GetComponent<ComponentDrawable>(Entity, EComponentType::Particle);
			CalculateParticlesVertices(Entity);
			ComponentDrawablePtr->Draw(InWindow->GetRenderWindow());
		}
	}
}

bool SystemRender::Wraparound(
	class ComponentDrawable* InComponentDrawable, 
	class Window* InWindow)
{
	auto CurrentPosition = InComponentDrawable->GetPosition();
	sf::FloatRect Bounds = InComponentDrawable->GetGlobalBounds();
	bool IsWrap = false;
	
	float LeftBorder = CurrentPosition.x - Bounds.width / 2;
	float RightBorder = CurrentPosition.x + Bounds.width / 2;
	float TopBorder = CurrentPosition.y - Bounds.height / 2;
	float BottomBorder = CurrentPosition.y + Bounds.height / 2;

	auto windowSize = InWindow->GetSize();

	if (LeftBorder < 0.0f)
	{
		CurrentPosition.x += windowSize.x;
		IsWrap = true;
	}
	if (RightBorder >= windowSize.x)
	{
		CurrentPosition.x -= windowSize.x;
		IsWrap = true;
	}
	if (TopBorder < 0.0f)
	{
		CurrentPosition.y += windowSize.y;
		IsWrap = true;
	}
	if (BottomBorder > windowSize.y)
	{
		CurrentPosition.y -= windowSize.y;
		IsWrap = true;
	}

	if (IsWrap)
	{
		InComponentDrawable->UpdatePosition(CurrentPosition);
		InComponentDrawable->DrawBoundingRect(InWindow->GetRenderWindow());
		InComponentDrawable->Draw(InWindow->GetRenderWindow());
	}

	return IsWrap;
}

void SystemRender::NotifyWraparound(const EntityID EntityId, const sf::Vector2f& InVecPos)
{
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::Wraparound);
	TmpMsg.m_2f.m_x = InVecPos.x;
	TmpMsg.m_2f.m_y = InVecPos.y;
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemRender::CalculateParticlesVertices(const EntityID EntityId)
{
	Message TmpMsg;
	TmpMsg.mType = static_cast<MessageType>(EntityMessage::CalculateParticles);
	TmpMsg.mSender = EntityId;
	mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
}

void SystemRender::SortDrawables() {
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	std::sort(mEntities.begin(), mEntities.end(),
		[eMgr](unsigned int FirstEntity, unsigned int SecondEntity)
		{
			auto drawble1 = eMgr->GetComponent<ComponentDrawable>(FirstEntity, EComponentType::Sprite);
			auto drawble2 = eMgr->GetComponent<ComponentDrawable>(SecondEntity, EComponentType::Sprite);
			return drawble1->GetRenderPriority() < drawble2->GetRenderPriority();
		});
}
