#pragma once

#include "ECS/Components/ComponentBase.h"
#include "ECS/Components/ComponentDrawble.h"
#include "GameData/TextureManager.h"

class ComponentSprite : public ComponentDrawable 
{
	ERenderPriority mRenderPriority;
	std::unique_ptr<sf::Texture> mTexture;
	sf::Sprite mSprite;
	sf::Vector2u mSpriteSize;
	sf::Vector2f mSpriteScale;

public:
	ComponentSprite() : 
		ComponentDrawable(EComponentType::Sprite),
		mRenderPriority(ERenderPriority::MAX)
	{}
	~ComponentSprite() {
	}

	void Create(
		TextureManager* InTextureManager,
		const EResourceTag& Tag)
	{
		mTexture = std::make_unique<sf::Texture>(InTextureManager->GetResource(Tag));
		mSprite.setTexture(*mTexture);
		mSpriteSize = sf::Vector2u(mTexture->getSize().x, mTexture->getSize().y);
		sf::FloatRect bounds = mSprite.getLocalBounds();
		mSprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	void SetRepeated(bool InState) { mTexture->setRepeated(InState); }
	void SetTextureRect(const sf::IntRect& InRect) { mSprite.setTextureRect(InRect); }
	sf::Vector2f GetLocalBoundsSize() const { return mSprite.getLocalBounds().getSize(); }
	sf::Vector2f GetLocalBoundsPosition() const override { return mSprite.getLocalBounds().getPosition(); }

	const sf::Vector2u GetSize() const override { return mSpriteSize; }
	const sf::FloatRect GetGlobalBounds() const override { return mSprite.getGlobalBounds(); }

	void Draw(sf::RenderWindow& Window) override { Window.draw(mSprite); }
	void UpdatePosition(const sf::Vector2f& UpdateVec) override { mSprite.setPosition(UpdateVec); }
	void UpdateAngle(const float InAngle) override { mSprite.setRotation(InAngle); }
	
	virtual sf::Vector2f GetPosition() const override { return mSprite.getPosition(); }
	void SetRenderPriority(ERenderPriority Priority) { mRenderPriority = Priority; };
	const ERenderPriority GetRenderPriority() const override { return mRenderPriority; };
	
	/*Debug draw*/
	void DrawBoundingRect(sf::RenderWindow& InWindow) override 
	{
		//sf::FloatRect rect = mSprite.getGlobalBounds();

		//sf::RectangleShape shape;
		//shape.setPosition(sf::Vector2f(rect.left, rect.top));
		//shape.setSize(sf::Vector2f(rect.width, rect.height));
		//shape.setFillColor(sf::Color::Transparent);
		//shape.setOutlineColor(sf::Color::Green);
		//shape.setOutlineThickness(1.f);

		//InWindow.draw(shape);
	}
};