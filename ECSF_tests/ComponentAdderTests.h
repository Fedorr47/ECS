#pragma once

#include "EntityManager.h"

class TestComponent1 : public IComponentBase
{
public:
	TestComponent1(ComponentType InComponentType) :
		IComponentBase{ InComponentType }
	{}

	virtual const ComponentType GetType() const override
	{
		return mType;
	}
private:
	float TestFloat{ 0.0f };
};

class TestComponentAdder : public ::testing::Test
{
protected:
	void SetUp()
	{
		ObjEntityManager.reset(dynamic_cast<EntityManager*>(CreateEntityManager()));
	}
	void TearDown()
	{
	}
	std::shared_ptr<EntityManager> ObjEntityManager;
};
