#pragma once

#include "ComponentAdder.h"
#include "IComponentBase.h"

enum class ComponentTypeTest
{
	TestComponentType1,
	TestComponentType2,
	MAX
};

class TestComponent1 : public IComponentBase
{
public:
	TestComponent1() :
		IComponentBase(static_cast<ComponentType>(ComponentTypeTest::TestComponentType1))
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
		ObjComponentAdder.reset(GetComponentAdder());
	}
	void TearDown()
	{
	}

	std::shared_ptr<IComponentAdder> ObjComponentAdder;
};
