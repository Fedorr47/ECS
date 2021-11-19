#include "stdafx.h"

#include "GameData/States/GameState.h"
#include "GameData/States/StateManager.h"
#include "GameData/HUD.h"
#include "GameData/Utils.h"
#include "ECS/Systems/SystemSpawn.h"
#include "ECS/Systems/SystemHealth.h"


GameState::GameState(StateManager* InStateManager) : 
	BaseState(InStateManager),
	SpawnSystem(*mStateMgr->GetContext()->mSystemManager->GetSystem<SystemSpawn>(ESystemType::Spawn)),
	HealthSystem(*mStateMgr->GetContext()->mSystemManager->GetSystem<SystemHealth>(ESystemType::Health))
{
	mFireCountdown = sf::Time::Zero;
	mFireInterval = sf::seconds(1.5f);

	mSpawnPlayerOptions			= std::make_shared<ActorOptions>();
	mSpawnEnemyrOptions			= std::make_shared<ActorOptions>();
	mSpawnParticleOptions		= std::make_shared<ActorOptions>();
	mSpawnProjectilesOptions	= std::make_shared<ActorOptions>();

	mSpawnCrashParticleOptions	= std::make_shared<ActorOptions>();
	mSpawnHitParticleOptions	= std::make_shared<ActorOptions>();

	mHud = std::make_unique<HUD>();
}

GameState::~GameState(){}

void GameState::Reset()
{
	mRoundTime = sf::seconds(ONE_MINUTE);
	mStateMgr->GetContext()->mSystemManager->PurgeEntities();
	mStateMgr->GetContext()->mSystemManager->GetEntityManager()->Purge();

	InitLevel();
}

void GameState::OnCreate()
{
	EventManager* evMgr = mStateMgr->GetContext()->mEventManager;
	mStateMgr->GetContext()->mSystemManager->SetSharedConxtext(mStateMgr->GetContext());

	evMgr->AddCallback<GameState>(EStateType::Game, "Key_Escape", &GameState::MainMenu, this);

	evMgr->AddCallback(EStateType::Game, "Player_MoveLeft", &GameState::PlayerMove, this);
	evMgr->AddCallback(EStateType::Game, "Player_MoveRight", &GameState::PlayerMove, this);
	evMgr->AddCallback(EStateType::Game, "Player_MoveUp", &GameState::PlayerMove, this);
	evMgr->AddCallback(EStateType::Game, "Player_Attack", &GameState::PlayerAction, this);

	evMgr->AddCallback(EStateType::Game, "Second_Player_MoveLeft", &GameState::PlayerMove, this);
	evMgr->AddCallback(EStateType::Game, "Second_Player_MoveRight", &GameState::PlayerMove, this);
	evMgr->AddCallback(EStateType::Game, "Second_Player_MoveUp", &GameState::PlayerMove, this);
	evMgr->AddCallback(EStateType::Game, "Second_Player_Attack", &GameState::PlayerAction, this);

	sf::Vector2u WindSize = mStateMgr->GetContext()->mWind->GetSize();
	mView.setSize(WindSize.x, WindSize.y);
	mStateMgr->GetContext()->mWind->GetRenderWindow().setView(mView);

	mStateMgr->GetContext()->mTextureManager->LoadResource(EResourceTag::PlayerTexture, "Eagle.png");
	mStateMgr->GetContext()->mTextureManager->LoadResource(EResourceTag::BackGround, "sky.png");
	mStateMgr->GetContext()->mTextureManager->LoadResource(EResourceTag::Projectile, "Laser.png");
	mStateMgr->GetContext()->mTextureManager->LoadResource(EResourceTag::EnemyTexture, "Raptor.png");
	mStateMgr->GetContext()->mTextureManager->LoadResource(EResourceTag::Particle, "Particle.png");

	
	Reset();
}

void GameState::OnDestroy() {
	EventManager* evMgr = mStateMgr->GetContext()->mEventManager;
	evMgr->RemoveCallback(EStateType::Game, "Key_Escape");
	evMgr->RemoveCallback(EStateType::Game, "Player_MoveLeft");
	evMgr->RemoveCallback(EStateType::Game, "Player_MoveRight");
	evMgr->RemoveCallback(EStateType::Game, "Player_MoveUp");
	evMgr->RemoveCallback(EStateType::Game, "Player_Attack");

	evMgr->RemoveCallback(EStateType::Game, "Second_Player_MoveLeft");
	evMgr->RemoveCallback(EStateType::Game, "Second_Player_MoveRight");
	evMgr->RemoveCallback(EStateType::Game, "Second_Player_MoveUp");
	evMgr->RemoveCallback(EStateType::Game, "Second_Player_Attack");
}

void GameState::InitLevel()
{
	mSpawnEnemyrOptions->Speed					= mSpawnPlayerOptions->Speed = 10.0f;
	mSpawnEnemyrOptions->MaxVelocity			= mSpawnPlayerOptions->MaxVelocity = 50.0f;
	mSpawnEnemyrOptions->RenderPriority			= mSpawnPlayerOptions->RenderPriority = ERenderPriority::Scene;
	mSpawnEnemyrOptions->Angle					= mSpawnPlayerOptions->Angle = 90.0f;
	mSpawnEnemyrOptions->IsInertion				= mSpawnPlayerOptions->IsInertion	= true;
	mSpawnEnemyrOptions->ProjectileSpawnRate	= mSpawnPlayerOptions->ProjectileSpawnRate = 1.0f;
	mSpawnEnemyrOptions->HealthPoints			= mSpawnPlayerOptions->HealthPoints = 10;
	mSpawnEnemyrOptions->RespawnCounter			= mSpawnPlayerOptions->RespawnCounter = 2.0f;
	mSpawnEnemyrOptions->DamageValue			= mSpawnPlayerOptions->DamageValue = 10;

	mSpawnProjectilesOptions->LifeTime = 4.0f;
	mSpawnProjectilesOptions->ResourceTag = EResourceTag::Particle;;

	mSpawnProjectilesOptions->Speed = 400.0f;
	mSpawnProjectilesOptions->MaxVelocity = 400.0f;
	mSpawnProjectilesOptions->IsInertion = true;
	mSpawnProjectilesOptions->LifeTime = 1.0f;
	mSpawnProjectilesOptions->ResourceTag = EResourceTag::Projectile;
	mSpawnProjectilesOptions->RenderPriority = ERenderPriority::Scene;
	mSpawnProjectilesOptions->DamageValue = 5;

	CreateBackGround();
	CreatePlayer();
	CreateEnemy();
	//CreateParticles();

	SpawnSystem.AddSpawnOptions(mPlayer, ECombatEntityType::Player, *mSpawnPlayerOptions);
	SpawnSystem.AddSpawnOptions(mEnemy, ECombatEntityType::Enemy, *mSpawnEnemyrOptions);
	SpawnSystem.AddSpawnOptions(mPlayer, ECombatEntityType::Projectile, *mSpawnProjectilesOptions);
	SpawnSystem.AddSpawnOptions(mEnemy, ECombatEntityType::Projectile, *mSpawnProjectilesOptions);

	mHud->Init(mStateMgr->GetContext()->mWind->GetSize(), mRoundTime);
}

void GameState::CreateBackGround()
{
	try
	{
		int entityId = mStateMgr->GetContext()->mEntityManager->AddEntity(3);
		
		Message TmpMsg;

		TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedSprite);
		TmpMsg.m_int = static_cast<int>(EResourceTag::BackGround);
		TmpMsg.mSender = entityId;
		mStateMgr->GetContext()->mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);

		TmpMsg.mType = static_cast<MessageType>(EntityMessage::ChangedRenderPriority);
		TmpMsg.m_int = static_cast<int>(ERenderPriority::BackGround);
		TmpMsg.mSender = entityId;
		mStateMgr->GetContext()->mSystemManager->GetMessageHandler()->Dispatch(TmpMsg);
	}
	catch (const std::exception& ex)
	{
		throw;
	}
}

void GameState::CreatePlayer()
{
	try
	{
		mSpawnPlayerOptions->Position = sf::Vector2f(50.0f, 50.0f);
		mSpawnPlayerOptions->Angle = 90.0f;
		mSpawnPlayerOptions->ResourceTag = EResourceTag::PlayerTexture;
		mPlayer = CreateActorObject(*mSpawnPlayerOptions);

		SpawnSystem.AttachParticle(mPlayer, *mSpawnParticleOptions);
	}
	catch(const std::exception& ex)
	{
		throw;
	}

	if (mPlayer == -1)
		throw std::exception("Player hasn't been created");
}

void GameState::CreateEnemy()
{
	try
	{
		auto WindowSize = mStateMgr->GetContext()->mWind->GetSize();

		mSpawnEnemyrOptions->Position = sf::Vector2f(static_cast<float>(WindowSize.x - 50), static_cast<float>(WindowSize.y - 50));
		mSpawnEnemyrOptions->Angle = 120.0f;
		mSpawnEnemyrOptions->ResourceTag = EResourceTag::EnemyTexture;
		mEnemy = CreateActorObject(*mSpawnEnemyrOptions);//(580.0f, 400.0f));
		
		SpawnSystem.AttachParticle(mEnemy, *mSpawnParticleOptions);
	}
	catch (const std::exception& ex)
	{
		throw;
	}

	if (mEnemy == -1)
		throw std::exception("Player hasn't been created");
}

void GameState::CreateParticles()
{
	mSpawnHitParticleOptions->ResourceTag		= mSpawnCrashParticleOptions->ResourceTag	 = EResourceTag::Particle;
	mSpawnHitParticleOptions->RenderPriority	= mSpawnCrashParticleOptions->RenderPriority = ERenderPriority::Scene;

	SpawnSystem.SpawnParticle(mPlayer, *mSpawnHitParticleOptions);
	SpawnSystem.SpawnParticle(mPlayer, *mSpawnCrashParticleOptions);

	SpawnSystem.SpawnParticle(mEnemy, *mSpawnHitParticleOptions);
	SpawnSystem.SpawnParticle(mEnemy, *mSpawnCrashParticleOptions);
}

const EntityID GameState::CreateActorObject(ActorOptions& SpawnOptions)
{
	return SpawnSystem.SpawnActor(DEFAULT_ENTITY, SpawnOptions);
}

void GameState::Update(const sf::Time& DeltaTime){
	SharedContext* context = mStateMgr->GetContext();
	mHud->Update(HealthSystem.GetHealthPercent(mPlayer), HealthSystem.GetHealthPercent(mEnemy), mRoundTime);
	context->mSystemManager->Update(DeltaTime.asSeconds());
	mRoundTime -= DeltaTime;
	if (mRoundTime <= sf::Time::Zero)
	{
		mStateMgr->SwitchTo(EStateType::GameOver);
	}
}

void GameState::Draw()
{
	mStateMgr->GetContext()->mSystemManager->Draw(
		mStateMgr->GetContext()->mWind);
	mHud->Draw(mStateMgr->GetContext()->mWind->GetRenderWindow());
}

void GameState::MainMenu(EventDetails* Details)
{
	mStateMgr->SwitchTo(EStateType::MainMenu);
}

void GameState::Pause(EventDetails* Details)
{
	mStateMgr->SwitchTo(EStateType::Paused);
}

void GameState::Activate(){}

void GameState::Deactivate(){}

void GameState::PlayerMove(EventDetails* Details){
	Message msg(static_cast<MessageType>(EntityMessage::ControlInput));

	if (Details->mName == "Player_MoveLeft" || Details->mName == "Second_Player_MoveLeft"){
		msg.m_int = static_cast<int>(EDirection::Left);
	} else if (Details->mName == "Player_MoveRight" || Details->mName == "Second_Player_MoveRight"){
		msg.m_int = static_cast<int>(EDirection::Right);
	} else if (Details->mName == "Player_MoveUp" || Details->mName == "Second_Player_MoveUp"){
		msg.m_int = static_cast<int>(EDirection::Up);
	}

	if (Details->mName.find("Second_Player") != std::string::npos)
	{
		msg.mSender = mEnemy;
	}
	else
	{
		msg.mSender = mPlayer;
	}

	mStateMgr->GetContext()->mSystemManager->GetMessageHandler()->Dispatch(msg);
}

void GameState::PlayerAction(EventDetails* Details)
{
	if (mFireCountdown <= sf::Time::Zero)
	{
		Message msg(static_cast<MessageType>(EntityMessage::AttackAction));

		if (Details->mName == "Second_Player_Attack")
			mStateMgr->GetContext()->mSystemManager->AddEvent(mEnemy, static_cast<EventID>(EntityEvent::BeganAttack));
		else if (Details->mName == "Player_Attack")
			mStateMgr->GetContext()->mSystemManager->AddEvent(mPlayer, static_cast<EventID>(EntityEvent::BeganAttack));
	}
}

