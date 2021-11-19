#include "stdafx.h"

#include <filesystem>

#include "GameData/States/GameOverState.h"
#include "GameData/States/GameState.h"
#include "GameData/States/StateManager.h"
#include "ECS/Systems/SystemScore.h"

GameOverState::GameOverState(StateManager* InStateManager) :
	BaseState(InStateManager),
	ScoreSystem(*mStateMgr->GetContext()->mSystemManager->GetSystem<SystemScore>(ESystemType::Score))
{
}

GameOverState::~GameOverState() {}

void GameOverState::OnCreate()
{
	mOutBorderSpace = 10;
	mCharacterSize = 18;

	mFont.loadFromFile(std::filesystem::current_path().string() + std::string("\\Resources\\Fonts\\Sansation.ttf"));
	mText.setFont(mFont);
	mText.setString(sf::String("Score:"));
	mText.setCharacterSize(mCharacterSize);

	sf::FloatRect textRect = mText.getLocalBounds();
	mText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	auto WindowSize = mStateMgr->GetContext()->mWind->GetSize();
	float InitX = WindowSize.x / 2;
	float InitY = WindowSize.y / 2;

	mText.setPosition(InitX, InitY - 100);
	InitY += textRect.top + textRect.height / 2.0f;


	std::string str[2];
	str[0] = "FIRST PLAYER :";
	str[1] = "SECOND PLAYER:";

	for (int i = 0; i < 2; ++i) 
	{
		mLabels[i].setFont(mFont);
		mLabels[i].setString(sf::String(str[i]));
		mLabels[i].setCharacterSize(12);
	}

	int PlayerScore = 0;
	int EnemyrScore = 0;

	if (mStateMgr->HasState(EStateType::MainMenu))
	{
		int PlayerID = static_cast<GameState&>(mStateMgr->GetState(EStateType::Game)).GetPlayerID();
		int EnemyID = static_cast<GameState&>(mStateMgr->GetState(EStateType::Game)).GetEnemyID();
		
		PlayerScore = ScoreSystem.GetScore(PlayerID);
		EnemyrScore = ScoreSystem.GetScore(EnemyID);
	}

	mLabels[0].setPosition(mOutBorderSpace + 100, WindowSize.y / 2);
	mLabels[0].setString(mLabels[0].getString() + " " + std::to_string(PlayerScore));
	mLabels[1].setPosition(WindowSize.x - 200, WindowSize.y / 2);
	mLabels[1].setString(mLabels[1].getString() + " " + std::to_string(EnemyrScore));

	EventManager* evMgr = mStateMgr->GetContext()->mEventManager;
	evMgr->AddCallback(EStateType::GameOver, "Mouse_Left", &GameOverState::MouseClick, this);
}

void GameOverState::OnDestroy()
{
	EventManager* evMgr = mStateMgr->GetContext()->mEventManager;
	evMgr->RemoveCallback(EStateType::GameOver, "Mouse_Left");
}

void GameOverState::Activate() {
}

void GameOverState::MouseClick(EventDetails* Details)
{
	mStateMgr->SwitchTo(EStateType::MainMenu);
}

void GameOverState::Draw()
{
	sf::RenderWindow& window = mStateMgr->GetContext()->mWind->GetRenderWindow();
	window.draw(mText);
	for (int i = 0; i < 2; ++i) {
		window.draw(mLabels[i]);
	}
}

void GameOverState::Update(const sf::Time& DeltaTime) {}
void GameOverState::Deactivate() {}