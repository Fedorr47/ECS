#pragma once

#include <deque>

#include "ECS/Components/ComponentDrawble.h"
#include "GameData/TextureManager.h"

struct Particle
{
	sf::Vector2f	position;
	sf::Color		color;
	sf::Time		lifetime;
};

enum class ERenderPriority : uint8_t;

class ComponentParticleEmmiter : public ComponentDrawable
{
	mutable sf::VertexArray mVertexArray;
	mutable bool mNeedsVertexUpdate;
	sf::Time mAccumulatedTime;

	std::unique_ptr<const sf::Texture> mTexture;
	EParticleType mType;
	sf::Vector2f mPosition;
	float mAngle;
	sf::FloatRect mChacedBounds;
	sf::Color mColor;
	sf::Time mLifetime;

	std::deque<Particle> mParticles;

public:
	ComponentParticleEmmiter() :
		ComponentDrawable(EComponentType::Particle),
		mVertexArray(sf::Quads)
	{
		mColor = sf::Color(255, 255, 50);
		mLifetime = sf::seconds(1.0f);
	}
	~ComponentParticleEmmiter() {
	}

	void Create(
		TextureManager* InTextureManager,
		const EResourceTag& Tag)
	{
		mTexture = std::make_unique<sf::Texture>(InTextureManager->GetResource(Tag));
	}

	void SetType(const EParticleType InParticleType) { mType = InParticleType; }
	const EParticleType GetType() const { return mType; }

	void SetAccumulatedTime(const sf::Time InAccumulatedTime) { mAccumulatedTime = InAccumulatedTime; }
	const sf::Time GetAccumulatedTime() const { return mAccumulatedTime; }

	void SetColor(const sf::Color InColor) { mColor = InColor; }
	const sf::Color GetColor() const { return mColor; }

	void SetLifetime(const sf::Time InLifetime) { mLifetime = InLifetime; }
	const sf::Time GetLifetime() const { return mLifetime; }

	void AddParticle(Particle& InParticle) { mParticles.push_back(InParticle); }
	Particle& GetParticle() { return mParticles.front(); }
	void RemoveFrontParticle() { mParticles.pop_front(); }
	std::deque<Particle>& GetAllParticles() { return mParticles; }

	sf::Vector2u GetTextureSize() const { return mTexture->getSize(); }
	void ClearVertex() { mVertexArray.clear(); }
	void AddVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
	{
		sf::Vertex vertex;
		vertex.position = sf::Vector2f(worldX, worldY);
		vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
		vertex.color = color;
		mVertexArray.append(vertex);
	}

	void Draw(sf::RenderWindow& Window) override {
		sf::RenderStates states;
		states.texture = &(*mTexture);
		Window.draw(mVertexArray, states);
	}
	void UpdatePosition(const sf::Vector2f& UpdateVec) override { mPosition = UpdateVec; }
	void UpdateAngle(const float InAngle) override { mAngle = InAngle; }
	void DrawBoundingRect(sf::RenderWindow& Window) override {}
	void SetRenderPriority(ERenderPriority Priority) override {}

	const ERenderPriority GetRenderPriority() const override { return ERenderPriority::Scene; }
	const sf::Vector2u GetSize() const override { return mTexture->getSize(); }
	sf::Vector2f GetLocalBoundsPosition() const override { return sf::Vector2f(); }
	const sf::FloatRect GetGlobalBounds() const override { return sf::FloatRect(); }
	sf::Vector2f GetPosition() const override { return mPosition; }
	const float GetRotation() const { return mAngle; }
	const sf::FloatRect& GetBounds() const { return mChacedBounds; }
	void SetBounds(const sf::FloatRect& InBounds) { mChacedBounds = InBounds; }
};