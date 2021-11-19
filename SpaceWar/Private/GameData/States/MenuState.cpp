#include "stdafx.h"

#include <filesystem>

#include "GameData/States/MenuState.h"
#include "GameData/States/GameState.h"
#include "GameData/States/StateManager.h"

MainMenuState::MainMenuState(StateManager* InStateManager) : 
	BaseState(InStateManager)
{}

MainMenuState::~MainMenuState() {}

void MainMenuState::OnCreate() 
{
	mFont.loadFromFile(std::filesystem::current_path().string() + std::string("\\Resources\\Fonts\\Sansation.ttf"));
	mText.setFont(mFont);
	mText.setString(sf::String("MAIN MENU:"));
	mText.setCharacterSize(18);

	sf::FloatRect textRect = mText.getLocalBounds();
	mText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	auto WindowSize = mStateMgr->GetContext()->mWind->GetSize();
	float InitX = WindowSize.x / 2;
	float InitY = WindowSize.y / 2;

	mText.setPosition(InitX, InitY - 100);
	InitY += textRect.top + textRect.height / 2.0f;
	
	mButtonSize = sf::Vector2f(300.0f, 32.0f);
	mButtonPos = sf::Vector2f(InitX, InitY);
	mButtonPadding = 4; // 4px.


	std::string str[2];
	str[0] = "PLAY";
	str[1] = "EXIT";

	for (int i = 0; i < 2; ++i) {
		sf::Vector2f buttonPosition(
			mButtonPos.x, mButtonPos.y +
			(i * (mButtonSize.y + mButtonPadding)));
		mRects[i].setSize(mButtonSize);
		mRects[i].setFillColor(sf::Color::Red);

		mRects[i].setOrigin(
			mButtonSize.x / 2.0f, mButtonSize.y / 2.0f);
		mRects[i].setPosition(buttonPosition);

		mLabels[i].setFont(mFont);
		mLabels[i].setString(sf::String(str[i]));
		mLabels[i].setCharacterSize(12);

		sf::FloatRect rect = mLabels[i].getLocalBounds();
		mLabels[i].setOrigin(
			rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);

		mLabels[i].setPosition(buttonPosition);
	}

	EventManager* evMgr = mStateMgr->GetContext()->mEventManager;
	evMgr->AddCallback(EStateType::MainMenu, "Mouse_Left", &MainMenuState::MouseClick, this);
}

void MainMenuState::OnDestroy() 
{
	EventManager* evMgr = mStateMgr->GetContext()->mEventManager;
	evMgr->RemoveCallback(EStateType::MainMenu, "Mouse_Left");
}

void MainMenuState::Activate() {
	if (mStateMgr->HasState(EStateType::Game)
		&& static_cast<GameState&>(mStateMgr->GetState(EStateType::Game)).GetRoundTime() > sf::Time::Zero)
	{
		mLabels[0].setString(sf::String("RESUME"));
	}
	else {
		if (mStateMgr->HasState(EStateType::Game))
		{
			static_cast<GameState&>(mStateMgr->GetState(EStateType::Game)).Reset();
		}
		mLabels[0].setString(sf::String("PLAY"));
	}

	sf::FloatRect rect = mLabels[0].getLocalBounds();
	mLabels[0].setOrigin(
		rect.left + rect.width / 2.0f,
		rect.top + rect.height / 2.0f);
}

void MainMenuState::MouseClick(EventDetails* Details) 
{
	SharedContext* context = mStateMgr->GetContext();
	sf::Vector2i mousePos = Details->mMouse;

	float halfX = mButtonSize.x / 2.0f;
	float halfY = mButtonSize.y / 2.0f;
	for (int i = 0; i < 3; ++i) {
		if (mousePos.x >= mRects[i].getPosition().x - halfX &&
			mousePos.x <= mRects[i].getPosition().x + halfX &&
			mousePos.y >= mRects[i].getPosition().y - halfY &&
			mousePos.y <= mRects[i].getPosition().y + halfY)
		{
			if (i == 0) {
				mStateMgr->SwitchTo(EStateType::Game);
			}
			else if (i == 1) {
				mStateMgr->GetContext()->mWind->Close();
			}
		}
	}
}

void MainMenuState::Draw() 
{
	sf::RenderWindow& window = mStateMgr->GetContext()->mWind->GetRenderWindow();
	window.draw(mText);
	for (int i = 0; i < 3; ++i) {
		window.draw(mRects[i]);
		window.draw(mLabels[i]);
	}
}

void MainMenuState::Update(const sf::Time& DeltaTime) {}
void MainMenuState::Deactivate() {}