#include "pch.h"

#include "ComponentAdderTests.h"

TEST_F(TestComponentAdder, GetComponentsCount)
{
	size_t DefaultComponentCounts = ObjEntityManager->GetComponentsCount();
	EXPECT_EQ(DefaultComponentCounts, DEFAULT_MAX_COMPONENT_COUNT);
}

TEST_F(TestComponentAdder, SetComponentsCount)
{
	size_t NewComponentsCount = 128;

	ObjEntityManager->SetComponentsCount(NewComponentsCount);
	EXPECT_EQ(ObjEntityManager->GetComponentsCount(), NewComponentsCount);
}
TEST_F(TestComponentAdder, RegisterComponent)
{
	ComponentType lComponentType = 0;

	EXPECT_TRUE(ObjEntityManager->RegisterComponent<TestComponent1>(static_cast<ComponentType>(lComponentType)));
}

TEST_F(TestComponentAdder, UnregisterComponent)
{
	ComponentType lComponentType = 0;

	ObjEntityManager->RegisterComponent<TestComponent1>(static_cast<ComponentType>(lComponentType));
	EXPECT_TRUE(ObjEntityManager->UnregisterComponent(static_cast<ComponentType>(lComponentType)));
}

TEST_F(TestComponentAdder, CheckMaxComponents)
{
	size_t MaxComponentsCount = ObjEntityManager->GetComponentsCount();

	std::vector<ComponentType> vPsedoComponentTypes(MaxComponentsCount);
	std::generate(vPsedoComponentTypes.begin(), vPsedoComponentTypes.end(), [n = 0]() mutable { return ++n; });
	for (int PsedoComponentType : vPsedoComponentTypes)
	{
		EXPECT_TRUE(ObjEntityManager->RegisterComponent<TestComponent1>(static_cast<ComponentType>(PsedoComponentType)));
	}

	size_t OverboundComponentCount = MaxComponentsCount + 1;
	EXPECT_FALSE(ObjEntityManager->RegisterComponent<TestComponent1>(static_cast<ComponentType>(OverboundComponentCount)));

	for (int PsedoComponentType : vPsedoComponentTypes)
	{
		EXPECT_TRUE(ObjEntityManager->UnregisterComponent(PsedoComponentType));
	}

	EXPECT_FALSE(ObjEntityManager->UnregisterComponent(static_cast<ComponentType>(0)));
}