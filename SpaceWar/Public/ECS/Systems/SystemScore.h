#pragma once
#include "ECS/Systems/SystemBase.h"

enum class EScoredType : uint8_t;

class SystemScore : public SystemBase{
public:
	SystemScore(SystemManager* InSystemManager);
	~SystemScore();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
	int GetScore(const EntityID EntityId);
private:
	void AddScore(const EntityID EntityId, const EScoredType ScoreType);
};