#pragma once
#include "EntityManager.h"

class TestEntityManager : public ::testing::Test
{
protected:
	void SetUp()
	{
		ObjEntityManager.reset(CreateEntityManager());
	}
	void TearDown()
	{
	}
	std::shared_ptr<IEntityManager> ObjEntityManager;
};