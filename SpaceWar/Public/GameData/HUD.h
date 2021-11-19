#pragma once

class HUD
{
	int mOutBorderSpace{0};
	int mCharacterSize{0};

	float mFirstHealthPercent{1.0f};
	float mSecondHealthPercent{1.0f};

	sf::Vector2u mWindowSize;

	sf::Font	mRoundFont;
	sf::Text	mRoundText;

	sf::Time	mRoundTime;

	sf::RectangleShape FirstPlayerHealthBar;
	sf::RectangleShape SecondPlayerHealthBar;

	sf::Vector2f mFirstHealthBarSize;
	sf::Vector2f mSecondHealthBarSize;

public:
	explicit HUD(){}
	~HUD() {}

	void Init(const sf::Vector2u& WindowSize, sf::Time InRoundTime, int InOutBorderSpace = 5, int InCharacterSize = 18);
	void Draw(sf::RenderWindow& Window);
	void Update(float InFirstHealthPercent, float InSecondHealthPercent, sf::Time InCurrentTime);
	void SetRoundTimerText();
};