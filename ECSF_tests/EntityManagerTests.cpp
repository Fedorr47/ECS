#include "pch.h"
#include "EntityManagerTests.h"

TEST(EntityManagerTest, EntityManagerCreate) {
	std::weak_ptr<IEntityManager> WeakObjEntityManager;
	{
		std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
		WeakObjEntityManager = ObjEntityManager;
	}
	std::shared_ptr<IEntityManager> ObjEntityManager = WeakObjEntityManager.lock();
	EXPECT_EQ(ObjEntityManager, nullptr);
}

TEST_F(TestEntityManager, GetMaxEntitiesCount)
{
	EXPECT_EQ(ObjEntityManager->GetMaxEntitiesCount(), 2048);
}

TEST_F(TestEntityManager, SetMaxEntitiesCount) {
	ObjEntityManager->SetMaxEntitiesCount(4096);
	EXPECT_EQ(ObjEntityManager->GetMaxEntitiesCount(), 4096);
}

TEST_F(TestEntityManager, CreateEntity) {
	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_EQ(CreatedEntityId, 1);
}

TEST_F(TestEntityManager, HasEntity) {
	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->HasEntity(CreatedEntityId));
	EXPECT_FALSE(ObjEntityManager->HasEntity(++CreatedEntityId));
}

TEST_F(TestEntityManager, RemoveEntity) {
	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->RemoveEntity(CreatedEntityId));
}

TEST_F(TestEntityManager, AddComponent) {
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->AddComponent(CreatedEntityId, lComponentType));
}

TEST_F(TestEntityManager, HasComponent) {
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	ObjEntityManager->AddComponent(CreatedEntityId, lComponentType);
	EXPECT_TRUE(ObjEntityManager->HasComponent(CreatedEntityId, lComponentType));
}

TEST_F(TestEntityManager, RemoveComponent) {
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	ObjEntityManager->AddComponent(CreatedEntityId, lComponentType);
	EXPECT_TRUE(ObjEntityManager->RemoveComponent(CreatedEntityId, lComponentType));
}

TEST_F(TestEntityManager, GetComponent) {
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	ObjEntityManager->AddComponent(CreatedEntityId, lComponentType);
}