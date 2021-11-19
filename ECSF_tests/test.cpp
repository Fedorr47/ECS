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


TEST(EntityManagerTest, CreateEntity) {	
	
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_EQ(CreatedEntityId, 1);
}

TEST(EntityManagerTest, HasEntity) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());
}

TEST(EntityManagerTest, RemoveEntity) {
	std::shared_ptr<IEntityManager> ObjEntityManager(CreateEntityManager());

	EntityID CreatedEntityId = ObjEntityManager->AddEntity();
	EXPECT_TRUE(ObjEntityManager->RemoveEntity());

}
