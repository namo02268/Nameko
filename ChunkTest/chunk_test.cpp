#include "pch.h"
#include "Nameko/Chunk.h"

/*
class CHUNK_TEST : public ::testing::Test {
protected:
	struct Component1 {
		float x;
		float y;
		Component1(float x, float y) : x(x), y(y) {}
	};

	struct Component2 {
		double x;
		double y;
		double z;
		Component2(double x, float y, float z) : x(x), y(y), z(z) {}
	};

	Nameko::Chunk chunk1_;
	Nameko::Chunk chunk2_;

protected:
	virtual void SetUp() {
		chunk1_.AllocateBlock<Component1, 512>();
		chunk2_.AllocateBlock<Component1, 512>();
		chunk2_.AllocateBlock<Component2, 512>();

		chunk1_.AddComponent(Component1(1, 2));
		chunk1_.AddComponent(Component1(3, 4));
		chunk1_.AddComponent(Component1(5, 6));

		chunk2_.AddComponent(Component1(1, 2));
		chunk2_.AddComponent(Component2(1, 2, 3));
		chunk2_.AddComponent(Component1(3, 4));
		chunk2_.AddComponent(Component2(4, 5, 6));
		chunk2_.AddComponent(Component1(5, 6));
		chunk2_.AddComponent(Component2(7, 8, 9));

		chunk1_.RemoveComponent<Component1>(0);
		chunk2_.RemoveComponent<Component1>(1);
		chunk2_.RemoveComponent<Component2>(1);

	}

	virtual void TearDown() {
	}
};

TEST_F(CHUNK_TEST, Get) {
	EXPECT_EQ(5, chunk1_.Get<Component1>(0).x);
	EXPECT_EQ(6, chunk1_.Get<Component1>(0).y);
	EXPECT_EQ(3, chunk1_.Get<Component1>(1).x);
	EXPECT_EQ(4, chunk1_.Get<Component1>(1).y);

	EXPECT_EQ(1, chunk2_.Get<Component1>(0).x);
	EXPECT_EQ(2, chunk2_.Get<Component1>(0).y);
	EXPECT_EQ(5, chunk2_.Get<Component1>(1).x);
	EXPECT_EQ(6, chunk2_.Get<Component1>(1).y);

	EXPECT_EQ(1, chunk2_.Get<Component2>(0).x);
	EXPECT_EQ(2, chunk2_.Get<Component2>(0).y);
	EXPECT_EQ(3, chunk2_.Get<Component2>(0).z);
	EXPECT_EQ(7, chunk2_.Get<Component2>(1).x);
	EXPECT_EQ(8, chunk2_.Get<Component2>(1).y);
	EXPECT_EQ(9, chunk2_.Get<Component2>(1).z);
}

TEST_F(CHUNK_TEST, IterateALL) {
	double sum = 0.0f;
	chunk1_.IterateAll<2, Component1>([&sum](Component1& c1) {
		sum += (c1.x + c1.y);
		std::cout << c1.x << std::endl;
		std::cout << c1.y << std::endl;
		});
	EXPECT_EQ(18, sum);

	sum = 0.0f;
	chunk2_.IterateAll<2, Component1>([&sum](Component1& c1) {
		sum += (c1.x + c1.y);
	});
	EXPECT_EQ(14, sum);

	sum = 0.0f;
	chunk2_.IterateAll<2, Component1, Component2>([&sum](Component1& c1, Component2& c2) {
		sum += (c1.x + c1.y + c2.x + c2.y + c2.z);
		});
	EXPECT_EQ(44, sum);
}
*/