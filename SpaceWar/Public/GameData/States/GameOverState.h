#pragma once
#include "GameData/States/BaseState.h"
#include "GameData/EventManager.h"

class GameOverState : public BaseState {

	sf::Font mFont;
	sf::Text mText;

	int mOutBorderSpace{ 0 };
	int mCharacterSize{ 0 };

	sf::Text mLabels[4];

	class SystemScore& ScoreSystem;

public:
	GameOverState(StateManager* InStateManager);
	~GameOverState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& InTime);
	void Draw();

	void MouseClick(EventDetails* Dtatils);
};