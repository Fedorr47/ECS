#pragma once

#include "ECS/Components/ComponentBase.h"

class ComponentDrawable : public ComponentBase 
{
public:
	ComponentDrawable(const EComponentType& ComponentType) : ComponentBase(ComponentType) {}
	virtual ~ComponentDrawable() {}

	virtual void UpdatePosition(const sf::Vector2f& UpdateVec) = 0;
	virtual void UpdateAngle(const float InAngle) = 0;
	virtual void Draw(sf::RenderWindow& Window) = 0;
	virtual void DrawBoundingRect(sf::RenderWindow& Window) = 0;
	virtual void SetRenderPriority(ERenderPriority Priority) = 0;

	virtual const ERenderPriority GetRenderPriority() const = 0;
	virtual const sf::Vector2u GetSize() const = 0;
	virtual sf::Vector2f GetLocalBoundsPosition() const = 0;
	virtual const sf::FloatRect GetGlobalBounds() const = 0;
	virtual sf::Vector2f GetPosition() const = 0;
};