#include "stdafx.h"

#include <filesystem>

#include "GameData\HUD.h"

void HUD::Init(
	const sf::Vector2u& WindowSize, 
	sf::Time InRoundTime,
	int InOutBorderSpace /*= 5*/, 
	int InCharacterSize /*= 18*/)
{
	mWindowSize = WindowSize;
	mOutBorderSpace = InOutBorderSpace;
	mCharacterSize = InCharacterSize;

	sf::Vector2f HpBarSize = sf::Vector2f(WindowSize.x * 0.25, WindowSize.y * 0.05);
	float HpBarPositionX = mOutBorderSpace;
	float HpBarPositionY = mOutBorderSpace;

	FirstPlayerHealthBar.setFillColor(sf::Color::Green);
	FirstPlayerHealthBar.setSize(HpBarSize);
	FirstPlayerHealthBar.setPosition(HpBarPositionX, HpBarPositionY);
	mFirstHealthBarSize = FirstPlayerHealthBar.getSize();

	HpBarPositionX = (WindowSize.x - HpBarSize.x - mOutBorderSpace);

	SecondPlayerHealthBar.setFillColor(sf::Color::Green);
	SecondPlayerHealthBar.setSize(HpBarSize);
	SecondPlayerHealthBar.setPosition(HpBarPositionX, HpBarPositionY);
	mSecondHealthBarSize = SecondPlayerHealthBar.getSize();

	mRoundTime = InRoundTime;
	SetRoundTimerText();
}

void HUD::SetRoundTimerText()
{
	mRoundFont.loadFromFile(std::filesystem::current_path().string() + std::string("\\Resources\\Fonts\\Sansation.ttf"));
	mRoundText.setFont(mRoundFont);
	auto RoundTimeText = std::to_string(static_cast<int>(std::floor(mRoundTime.asSeconds())));
	mRoundText.setString(RoundTimeText);
	mRoundText.setCharacterSize(mCharacterSize);

	sf::FloatRect textRect = mRoundText.getLocalBounds();
	mRoundText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	float InitX = mWindowSize.x / 2;
	float InitY = mCharacterSize;

	mRoundText.setPosition(InitX, InitY);
}

void HUD::Draw(sf::RenderWindow& Window)
{
	Window.draw(FirstPlayerHealthBar);
	Window.draw(SecondPlayerHealthBar);
	Window.draw(mRoundText);
}

void HUD::Update(float InFirstHealthPercent, float InSecondHealthPercent, sf::Time InCurrentTime)
{
	mRoundTime = InCurrentTime;
	auto RoundTimeText = std::to_string(static_cast<int>(std::floor(mRoundTime.asSeconds())));
	mRoundText.setString(RoundTimeText);

	FirstPlayerHealthBar.setSize(sf::Vector2f(mFirstHealthBarSize.x * InFirstHealthPercent, mFirstHealthBarSize.y));
	SecondPlayerHealthBar.setSize(sf::Vector2f(mSecondHealthBarSize.x * InSecondHealthPercent, mSecondHealthBarSize.y));
}
