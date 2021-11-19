#pragma once
#include "GameData/States/BaseState.h"
#include "GameData/EventManager.h"

class MainMenuState : public BaseState {
	
	sf::Font mFont;
	sf::Text mText;

	sf::Vector2f mButtonSize;
	sf::Vector2f mButtonPos;
	unsigned int mButtonPadding;

	sf::RectangleShape mRects[3];
	sf::Text mLabels[3];

public:
	MainMenuState(StateManager* InStateManager);
	~MainMenuState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& InTime);
	void Draw();

	void MouseClick(EventDetails* Dtatils);
};