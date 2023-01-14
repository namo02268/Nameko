#include "pch.h"
#include "Nameko/Chunk.h"

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

	Nameko::Chunk<Component1> chunk1_;
	Nameko::Chunk<Component1, Component2> chunk2_;

protected:
	virtual void SetUp() {
		chunk1_.AddComponents(Component1(1, 2));
		chunk1_.AddComponents(Component1(3, 4));
		chunk1_.AddComponents(Component1(5, 6));

		chunk2_.AddComponents(Component1(1, 2), Component2(1, 2, 3));
		chunk2_.AddComponents(Component1(3, 4), Component2(4, 5, 6));
		chunk2_.AddComponents(Component1(5, 6), Component2(7, 8, 9));
	}

	virtual void TearDown() {
	}
};
TEST_F(CHUNK_TEST, GetComponent) {
	EXPECT_EQ(1, chunk1_.GetComponent<Component1>(0).x);
	EXPECT_EQ(2, chunk1_.GetComponent<Component1>(0).y);
	EXPECT_EQ(3, chunk1_.GetComponent<Component1>(1).x);
	EXPECT_EQ(4, chunk1_.GetComponent<Component1>(1).y);
	EXPECT_EQ(5, chunk1_.GetComponent<Component1>(2).x);
	EXPECT_EQ(6, chunk1_.GetComponent<Component1>(2).y);

	EXPECT_EQ(1, chunk2_.GetComponent<Component1>(0).x);
	EXPECT_EQ(3, chunk2_.GetComponent<Component2>(0).z);
	EXPECT_EQ(3, chunk2_.GetComponent<Component1>(1).x);
	EXPECT_EQ(6, chunk2_.GetComponent<Component2>(1).z);
	EXPECT_EQ(5, chunk2_.GetComponent<Component1>(2).x);
	EXPECT_EQ(9, chunk2_.GetComponent<Component2>(2).z);
}

TEST_F(CHUNK_TEST, RemoveComponent) {
	chunk1_.RemoveComponents(0);
	chunk2_.RemoveComponents(1);

	EXPECT_EQ(5, chunk1_.GetComponent<Component1>(0).x);
	EXPECT_EQ(3, chunk1_.GetComponent<Component1>(1).x);

	EXPECT_EQ(1, chunk2_.GetComponent<Component1>(0).x);
	EXPECT_EQ(1, chunk2_.GetComponent<Component2>(0).x);
	EXPECT_EQ(5, chunk2_.GetComponent<Component1>(1).x);
	EXPECT_EQ(7, chunk2_.GetComponent<Component2>(1).x);
}


TEST_F(CHUNK_TEST, IterateALL) {
	double sum = 0.0f;
	chunk1_.IterateAll<Component1>([&sum](Component1& c1) {
		sum += (c1.x + c1.y);
	});
	EXPECT_EQ(21, sum);

	sum = 0.0f;
	chunk2_.IterateAll<Component1>([&sum](Component1& c1) {
		sum += (c1.x + c1.y);
	});
	EXPECT_EQ(21, sum);

	sum = 0.0f;
	chunk2_.IterateAll<Component1, Component2>([&sum](Component1& c1, Component2& c2) {
		sum += (c1.x + c1.y + c2.x + c2.y + c2.z);
		});
	EXPECT_EQ(66, sum);
}