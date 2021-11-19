#include "stdafx.h"

#include "GameData/States/StateManager.h"
#include "GameData/States/MenuState.h"
#include "GameData/States/GameState.h"
#include "GameData/States/GameOverState.h"

#include "GameData/States/StateManager.h"

StateManager::StateManager(SharedContext* Shared)
	: mSharedContext(Shared)
{
	RegisterState<MainMenuState>(EStateType::MainMenu);
	RegisterState<GameState>(EStateType::Game);
	RegisterState<GameOverState>(EStateType::GameOver);
}

StateManager::~StateManager() {
	for (auto& itr : mStates) {
		itr.second->OnDestroy();
	}
}

void StateManager::Update(const sf::Time& DeltaTime) {
	if (mStates.empty()) { return; }
	if (mStates.back().second->IsTranscendent() && mStates.size() > 1) {
		auto itr = mStates.end();
		while (itr != mStates.begin()) {
			if (itr != mStates.end()) {
				if (!itr->second->IsTranscendent()) {
					break;
				}
			}
			--itr;
		}
		for (; itr != mStates.end(); ++itr) {
			itr->second->Update(DeltaTime);
		}
	}
	else {
		mStates.back().second->Update(DeltaTime);
	}
}

void StateManager::Draw() {
	if (mStates.empty()) { return; }
	if (mStates.back().second->IsTransparent() && mStates.size() > 1) {
		auto itr = mStates.end();
		while (itr != mStates.begin()) {
			if (itr != mStates.end()) {
				if (!itr->second->IsTransparent()) {
					break;
				}
			}
			--itr;
		}
		for (; itr != mStates.end(); ++itr) {
			mSharedContext->mWind->GetRenderWindow().setView(itr->second->GetView());
			itr->second->Draw();
		}
	}
	else {
		mStates.back().second->Draw();
	}
}

SharedContext* StateManager::GetContext() { return mSharedContext; }

bool StateManager::HasState(const EStateType StateType) {
	for (auto itr = mStates.begin();
		itr != mStates.end(); ++itr)
	{
		if (itr->first == StateType) {
			auto removed = std::find(mToRemove.begin(), mToRemove.end(), StateType);
			if (removed == mToRemove.end()) 
			{ 
				return true; 
			}
			return false;
		}
	}
	return false;
}

BaseState& StateManager::GetState(const EStateType StateType)
{
	for (auto itr = mStates.begin();
		itr != mStates.end(); ++itr)
	{
		if (itr->first == StateType) {
			auto removed = std::find(mToRemove.begin(), mToRemove.end(), StateType);
			if (removed == mToRemove.end()) 
			{ 
				return *itr->second;
			}
		}
	}
}

void StateManager::ProcessRequests() {
	while (mToRemove.begin() != mToRemove.end()) {
		RemoveState(*mToRemove.begin());
		mToRemove.erase(mToRemove.begin());
	}
}

void StateManager::SwitchTo(const EStateType StateType) {
	mSharedContext->mEventManager->SetCurrentState(StateType);

	for (auto itr = mStates.begin();
		itr != mStates.end(); ++itr)
	{
		if (itr->first == StateType) {
			mStates.back().second->Deactivate();
			std::swap(itr->first, mStates.back().first);
			std::swap(itr->second, mStates.back().second);
			(*itr->second).Activate();
			mSharedContext->mWind->GetRenderWindow().setView((*itr->second).GetView());
			return;
		}
	}

	if (!mStates.empty()) 
	{ 
		mStates.back().second->Deactivate(); 
	}
	CreateState(StateType);
	mStates.back().second->Activate();
	mSharedContext->mWind->GetRenderWindow().setView(mStates.back().second->GetView());
}

void StateManager::Remove(const EStateType StateType) {
	mToRemove.push_back(StateType);
}

void StateManager::CreateState(const EStateType StateType) {
	auto newState = mStateFactory.find(StateType);
	if (newState == mStateFactory.end()) { return; }
	auto state = newState->second();
	state->mView = mSharedContext->mWind->GetRenderWindow().getDefaultView();
	state->OnCreate();
	mStates.emplace_back(StateType, std::move(state));
}

void StateManager::RemoveState(const EStateType StateType) {
	for (auto itr = mStates.begin();
		itr != mStates.end(); ++itr)
	{
		if (itr->first == StateType) {
			itr->second->OnDestroy();
			mStates.erase(itr);
			return;
		}
	}
}