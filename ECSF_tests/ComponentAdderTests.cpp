#include "pch.h"
#include "ComponentAdderTests.h"

TEST(ComponentAdderTest, ComponentAdderGet) {
	std::weak_ptr<IComponentAdder> WeakObjComponentAdder;
	{
		std::shared_ptr<IComponentAdder> ObjEntityManager(GetComponentAdder());
		WeakObjComponentAdder = ObjEntityManager;
	}
	std::shared_ptr<IComponentAdder> ObjComponentAdder = WeakObjComponentAdder.lock();
	EXPECT_EQ(ObjComponentAdder, nullptr);
}

TEST_F(TestComponentAdder, AddComponent)
{
	std::unique_ptr<IComponentBase> tComponentForTest(new TestComponent1());
	EXPECT_TRUE(ObjComponentAdder->AddComponent(std::move(tComponentForTest)));
}

TEST_F(TestComponentAdder, RemoveComponent)
{
	std::unique_ptr<IComponentBase> tComponentForTest(new TestComponent1());
	ObjComponentAdder->AddComponent(std::move(tComponentForTest));
	EXPECT_TRUE(ObjComponentAdder->RemoveComponent(static_cast<ComponentType>(ComponentTypeTest::TestComponentType1)));
}