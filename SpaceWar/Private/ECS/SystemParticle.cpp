#include "stdafx.h"

#include "ECS/Systems/SystemParticle.h"

#include "ECS/Systems/SystemState.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentParticleEmmiter.h"
#include "GameData/Utils.h"

SystemParticle::SystemParticle(SystemManager* InSystemManager) :
	SystemBase(ESystemType::Particle, InSystemManager)
{
	Bitmask req;
	req.set(static_cast<unsigned int>(EComponentType::Particle));
	mRequiredComponents.push_back(req);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::AttachParticle, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedEntityBounds, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedPosition, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::ChangedRotation, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::CalculateParticles, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Hit, this);
	mSystemManager->GetMessageHandler()->Subscribe(EntityMessage::Destroyed, this);
}

SystemParticle::~SystemParticle() {}

void SystemParticle::Update(float DeltaTime)
{
	EntityManager* Entities = mSystemManager->GetEntityManager();
	while (!EntityParticlies.empty())
	{
		auto ParticleToEmmit = EntityParticlies.front();
		EntityParticlies.pop();
		// Spawn Particles
		EmitParticles(DeltaTime, ParticleToEmmit.first, ParticleToEmmit.second);
	}

	for (auto& Entity : mEntities)
	{
		ComponentParticleEmmiter* ComponentParticleEmmiterPtr = Entities->GetComponent<ComponentParticleEmmiter>(Entity, EComponentType::Particle);

		while (!ComponentParticleEmmiterPtr->GetAllParticles().empty() && 
			ComponentParticleEmmiterPtr->GetParticle().lifetime <= sf::Time::Zero)
			ComponentParticleEmmiterPtr->RemoveFrontParticle();

		for (Particle& particle : ComponentParticleEmmiterPtr->GetAllParticles())
		{
			particle.lifetime -= sf::seconds(DeltaTime);
		}
	}
}

void SystemParticle::HandleEvent(
	const EntityID EntityId,
	const EntityEvent EventType)
{
	int z = 0;
	EntityManager* EntityManager = mSystemManager->GetEntityManager();
	switch (EventType)
	{
	case EntityEvent::Moving_Up:
		EntityParticlies.push(std::pair<EntityID, EParticleType>(EntityId, EParticleType::Propellant));
		break;
	}
}

void SystemParticle::Notify(const Message& msg)
{
	EntityManager* eMgr = mSystemManager->GetEntityManager();
	EntityMessage MsgType = static_cast<EntityMessage>(msg.mType);
	switch (MsgType)
	{
		case EntityMessage::ChangedPosition:
			ChangePosition(msg.mSender, sf::Vector2f(msg.m_2f.m_x, msg.m_2f.m_y));
			break;
		case EntityMessage::ChangedRotation:
			ChangeRotation(msg.mSender, msg.m_float);
			break;
		case EntityMessage::CalculateParticles:
			ComputeVertices(msg.mSender);
			break;
		case EntityMessage::AttachParticle:
			AttachParticle(msg.mSender, static_cast<EResourceTag>(msg.m_int));
			break;
		case EntityMessage::ChangedEntityBounds:
			RefreshEntityBounds(msg.mSender, sf::FloatRect(msg.m_4f.x, msg.m_4f.y, msg.m_4f.w, msg.m_4f.h));
			break;
		case EntityMessage::Despawn:
			RemoveEntity(msg.mSender);
			break;
		case EntityMessage::Respawn:
			AddEntity(msg.mSender);
			break;
		case EntityMessage::Hit:
			EntityParticlies.push(std::pair<EntityID, EParticleType>(msg.mReceiver, EParticleType::Hit));
			break;
		case EntityMessage::Destroyed:
			EntityParticlies.push(std::pair<EntityID, EParticleType>(msg.mReceiver, EParticleType::Crash));
			break;
	}
}

void SystemParticle::ChangePosition(
	const EntityID EntityId,
	const sf::Vector2f& InPosVec)
{
	ComponentParticleEmmiter* ComponentParticleEmmiterPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentParticleEmmiter>(EntityId, EComponentType::Particle);
	if (ComponentParticleEmmiterPtr != nullptr)
		ComponentParticleEmmiterPtr->UpdatePosition(InPosVec);
}

void SystemParticle::ChangeRotation(
	const EntityID EntityId,
	const float InAngle)
{
	ComponentParticleEmmiter* ComponentParticleEmmiterPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentParticleEmmiter>(EntityId, EComponentType::Particle);
	if (ComponentParticleEmmiterPtr != nullptr)
		ComponentParticleEmmiterPtr->UpdateAngle(InAngle);
}

void SystemParticle::RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds)
{
	ComponentParticleEmmiter* ComponentParticleEmmiterPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentParticleEmmiter>(EntityId, EComponentType::Particle);
	if (ComponentParticleEmmiterPtr != nullptr)
		ComponentParticleEmmiterPtr->SetBounds(InBounds);
}

void SystemParticle::AttachParticle(const EntityID EntityId, EResourceTag ResourceTag)
{
	ComponentParticleEmmiter* ComponentParticleEmmiterPtr = mSystemManager->GetEntityManager()->GetComponent<ComponentParticleEmmiter>(EntityId, EComponentType::Particle);
	if (ComponentParticleEmmiterPtr != nullptr)
		ComponentParticleEmmiterPtr->Create(mSystemManager->GetEntityManager()->GetTextureManager(), EResourceTag::Particle);
}

void SystemParticle::EmitPropellant(float DeltaTime, const EntityID EntityId, ComponentParticleEmmiter* InComponentParticleEmmiterPtr)
{
	auto Position = InComponentParticleEmmiterPtr->GetPosition();
	auto RotationAngle = Utils::toRadian(InComponentParticleEmmiterPtr->GetRotation() + 90.0f);
	auto Bounds = InComponentParticleEmmiterPtr->GetBounds();
	float vx = Bounds.width / 2 * std::cos(RotationAngle);
	float vy = Bounds.height / 2 * std::sin(RotationAngle);
	Position.x += vx;
	Position.y += vy;

	Particle particle;
	particle.position = Position;
	particle.color = InComponentParticleEmmiterPtr->GetColor();
	particle.lifetime = InComponentParticleEmmiterPtr->GetLifetime();

	InComponentParticleEmmiterPtr->AddParticle(particle);
}

void SystemParticle::EmitHit(float DeltaTime, const EntityID EntityId, ComponentParticleEmmiter* InComponentParticleEmmiterPtr)
{
	Particle particle;
	particle.position = InComponentParticleEmmiterPtr->GetPosition();
	particle.color = InComponentParticleEmmiterPtr->GetColor();
	particle.lifetime = InComponentParticleEmmiterPtr->GetLifetime();

	InComponentParticleEmmiterPtr->AddParticle(particle);
}

void SystemParticle::EmitCrash(float DeltaTime, const EntityID EntityId, ComponentParticleEmmiter* InComponentParticleEmmiterPtr)
{
	Particle particle;
	particle.position = InComponentParticleEmmiterPtr->GetPosition();
	particle.color = InComponentParticleEmmiterPtr->GetColor();
	particle.lifetime = sf::seconds(10.0f);//InComponentParticleEmmiterPtr->GetLifetime();

	InComponentParticleEmmiterPtr->AddParticle(particle);
}

void SystemParticle::EmitParticles(float DeltaTime, const EntityID EntityId, EParticleType ParticleType)
{
	EntityManager* EntityManager = mSystemManager->GetEntityManager();
	ComponentParticleEmmiter* ComponentParticleEmmiterPtr = EntityManager->GetComponent<ComponentParticleEmmiter>(EntityId, EComponentType::Particle);

	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	ComponentParticleEmmiterPtr->SetAccumulatedTime(ComponentParticleEmmiterPtr->GetAccumulatedTime() + sf::seconds(DeltaTime));

	while (ComponentParticleEmmiterPtr->GetAccumulatedTime() > interval)
	{
		ComponentParticleEmmiterPtr->SetAccumulatedTime(ComponentParticleEmmiterPtr->GetAccumulatedTime() - interval);
	
		switch (ParticleType)
		{
		case EParticleType::Propellant:
			EmitPropellant(DeltaTime, EntityId, ComponentParticleEmmiterPtr);
			break;
		case EParticleType::Crash:
			EmitCrash(DeltaTime, EntityId, ComponentParticleEmmiterPtr);
			break;
		case EParticleType::Hit:
			EmitHit(DeltaTime, EntityId, ComponentParticleEmmiterPtr);
			break;
		}
		
	}
}

void SystemParticle::ComputeVertices(const EntityID EntityId)
{
	EntityManager* EntityManager = mSystemManager->GetEntityManager();
	ComponentParticleEmmiter* ComponentParticleEmmiterPtr = EntityManager->GetComponent<ComponentParticleEmmiter>(EntityId, EComponentType::Particle);

	if (ComponentParticleEmmiterPtr != nullptr)
	{
		sf::Vector2f size(ComponentParticleEmmiterPtr->GetTextureSize());
		sf::Vector2f half = size / 2.f;

		ComponentParticleEmmiterPtr->ClearVertex();
		for (Particle& particle : ComponentParticleEmmiterPtr->GetAllParticles())
		{
			sf::Vector2f pos = particle.position;
			sf::Color color = particle.color;

			float ratio = particle.lifetime.asSeconds() / ComponentParticleEmmiterPtr->GetLifetime().asSeconds();
			color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

			ComponentParticleEmmiterPtr->AddVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
			ComponentParticleEmmiterPtr->AddVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
			ComponentParticleEmmiterPtr->AddVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
			ComponentParticleEmmiterPtr->AddVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
		}
	}
}
