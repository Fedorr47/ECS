#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

#include "GameData/States/BaseState.h"
#include "GameData/SharedContext.h"

enum class EStateType : uint8_t
{ 
	MainMenu = 1, 
	Game, 
	Paused, 
	GameOver, 
	Scored 
};


using StateContainer = std::vector<std::pair<EStateType, std::unique_ptr<BaseState>>>;
using TypeContainer = std::vector<EStateType>;
using StateFactory = std::unordered_map<EStateType, std::function<std::unique_ptr<BaseState> (void)>>;

class StateManager {
	
	SharedContext* mSharedContext;
	StateContainer mStates;
	TypeContainer mToRemove;
	StateFactory mStateFactory;

	void CreateState(const EStateType StateType);
	void RemoveState(const EStateType StateType);

	template<class T>
	void RegisterState(const EStateType& StateType) {
		mStateFactory[StateType] = [this]() -> std::unique_ptr<BaseState>
		{
			return std::make_unique<T>(this);
		};
	}

public:
	StateManager(SharedContext* Shared);
	~StateManager();

	void Update(const sf::Time& DeltaTime);
	void Draw();

	void ProcessRequests();

	SharedContext* GetContext();
	bool HasState(const EStateType StateType);
	BaseState& GetState(const EStateType StateType);

	void SwitchTo(const EStateType StateType);
	void Remove(const EStateType StateType);
	
};
