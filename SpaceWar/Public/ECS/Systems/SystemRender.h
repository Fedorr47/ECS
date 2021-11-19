#pragma once

#include <algorithm>

#include "ECS/Systems/SystemBase.h"

enum class ERenderPriority : uint8_t;
enum class EResourceTag : uint8_t;

class SystemRender : public SystemBase {
public:
	SystemRender(SystemManager* InSystemManager);
	~SystemRender();

	void Update(float DeltaTime);
	void HandleEvent(const EntityID EntityId, const EntityEvent EventtType);
	void Notify(const Message& msg);	
	void Render(Window* InWindow);
private:
	void ChangeSprite(const EntityID EntityId, EResourceTag ResourceTag);
	void ChangeRenderPriority(const EntityID EntityId, ERenderPriority RenderPriority);
	void ChangePosition(const EntityID EntityId, const sf::Vector2f& InPosVec);
	void ChangeRotation(const EntityID EntityId, const float InAngle);
	void UpdateBounds(const EntityID EntityId, const sf::FloatRect& Bounds);
	bool Wraparound(class ComponentDrawable* InComponentDrawable, class Window* InWindow);
	void NotifyWraparound(const EntityID EntityId, const sf::Vector2f& InVecpos);
	void CalculateParticlesVertices(const EntityID EntityId);
	void SortDrawables();
};