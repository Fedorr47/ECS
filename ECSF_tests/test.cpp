#include "pch.h"
#include "EntityManager.h"

TEST(EntityManagerTest, EntityManagerCreate) {
	std::weak_ptr<IEntityManager> WeakObjEntityManager;
	{
		std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
		WeakObjEntityManager = ObjEntityManager;
	}
	std::shared_ptr<IEntityManager> ObjEntityManager = WeakObjEntityManager.lock();
	EXPECT_EQ(ObjEntityManager, nullptr);
}

TEST(EntityManagerTest, GetMaxEntitiesCount) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
	EXPECT_EQ(ObjEntityManager->GetMaxEntitiesCount(), 2048);
}

TEST(EntityManagerTest, SetMaxEntitiesCount) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
	ObjEntityManager->SetMaxEntitiesCount(4096);
	EXPECT_EQ(ObjEntityManager->GetMaxEntitiesCount(), 4096);
}

TEST(EntityManagerTest, CreateEntity) {

	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_EQ(CreatedEntityId, 1);
}

TEST(EntityManagerTest, HasEntity) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->HasEntity(CreatedEntityId));
	EXPECT_FALSE(ObjEntityManager->HasEntity(++CreatedEntityId));
}

TEST(EntityManagerTest, RemoveEntity) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->RemoveEntity(CreatedEntityId));
}

TEST(EntityManagerTest, AddComponent) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->AddComponent(CreatedEntityId, lComponentType));
}

TEST(EntityManagerTest, HasComponent) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	ObjEntityManager->AddComponent(CreatedEntityId, lComponentType);
	EXPECT_TRUE(ObjEntityManager->HasComponent(CreatedEntityId, lComponentType));
}

TEST(EntityManagerTest, RemoveComponent) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	ObjEntityManager->AddComponent(CreatedEntityId, lComponentType);
	EXPECT_TRUE(ObjEntityManager->RemoveComponent(CreatedEntityId, lComponentType));
}

TEST(EntityManagerTest, GetComponent) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
	ComponentType lComponentType = 0;

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	ObjEntityManager->AddComponent(CreatedEntityId, lComponentType);
}
