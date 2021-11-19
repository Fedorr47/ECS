#pragma once
#include "ECS/ECS_Types.h"
#include "GameData/States/BaseState.h"
#include "GameData/EventManager.h"

#define ONE_MINUTE 60
#define RESPAWM_TIME 2.0f

enum class EResourceTag : uint8_t;
enum class ETag : uint8_t;

class GameState : public BaseState{
	int mPlayer;
	int mEnemy;
	sf::View	mWorldView;
	sf::Time	mFireCountdown;
	sf::Time	mFireInterval;
	sf::Time	mRoundTime;

	std::shared_ptr<struct ActorOptions> mSpawnPlayerOptions;
	std::shared_ptr<struct ActorOptions> mSpawnEnemyrOptions;
	std::shared_ptr<struct ActorOptions> mSpawnProjectilesOptions;
	std::shared_ptr<struct ActorOptions> mSpawnParticleOptions;

	std::shared_ptr<struct ActorOptions> mSpawnHitParticleOptions;
	std::shared_ptr<struct ActorOptions> mSpawnCrashParticleOptions;

	std::unique_ptr<class HUD> mHud;

	class SystemSpawn& SpawnSystem;
	class SystemHealth& HealthSystem;

	void InitLevel();
	void CreatePlayer();
	void CreateEnemy();
	void CreateParticles();
	const EntityID CreateActorObject(struct ActorOptions& SpawnOptions);

public:
	GameState(StateManager* InStateManager);
	~GameState();

	int GetPlayerID() { return mPlayer; }
	int GetEnemyID() { return mEnemy; }

	void OnCreate();
	void CreateBackGround();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& DeltaTime);
	void Draw();

	void MainMenu(EventDetails* Details);
	void Pause(EventDetails* Details);
	void PlayerMove(EventDetails* Details);
	void PlayerAction(EventDetails* Details);
	sf::Time GetRoundTime() const { return mRoundTime; }
	void Reset();
};