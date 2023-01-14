#include "pch.h"
#include "Nameko/Chunk.h"

class BLOCK_TEST : public ::testing::Test {
protected:
	Nameko::Block<char, 256> block0_;
	Nameko::Block<float, 512> block1_;
	Nameko::Block<double, 1024> block2_;

protected:
	virtual void SetUp() {
		block0_.create('a');	// a		(create: a)
		block0_.create('b');	// a b		(create: b)
		block0_.create('c');	// a b c	(create: c)
		block0_.create('d');	// a b c d	(create: d)
		block0_.swap(0, 2);		// c b a d	(swap: a c)
		block0_.destroy(1);		// c d a	(destroy: b, swap b d)
		block0_.create('e');	// c d a e	(create: e)
		block0_.swap(0, 3);		// e d a c	(swap: c e)

		block1_.create(1);		// 1
		block1_.create(2);		// 1 2
		block1_.create(3);		// 1 2 3
		block1_.create(4);		// 1 2 3 4
		block1_.swap(0, 1);		// 2 1 3 4
		block1_.destroy(3);		// 2 1 3
		block1_.create(5);		// 2 1 3 5
		block1_.swap(2, 3);		// 2 1 5 3

		block2_.create(0.1);	// 0.1
		block2_.create(0.2);	// 0.1 0.2
		block2_.create(0.3);	// 0.1 0.2 0.3
		block2_.create(0.4);	// 0.1 0.2 0.3 0.4
		block2_.destroy(1);		// 0.1 0.4 0.3
		block2_.destroy(1);		// 0.1 0.3
		block2_.create(0.5);	// 0.1 0.3 0.5
		block2_.create(0.6);	// 0.1 0.3 0.5 0.6
	}

	virtual void TearDown() {
	}
};
TEST_F(BLOCK_TEST, Size) {
	// Test actual element size
	EXPECT_EQ(4, block0_.size());
	EXPECT_EQ(4, block1_.size());
	EXPECT_EQ(4, block2_.size());
	// Test max element size
	EXPECT_EQ(256, block0_.getMaxElement());
	EXPECT_EQ(512, block1_.getMaxElement());
	EXPECT_EQ(1024, block2_.getMaxElement());
	// Test type size
	EXPECT_EQ(1, block0_.getTypeSize());	// char   : 1[byte]
	EXPECT_EQ(4, block1_.getTypeSize());	// float  : 4[byte]
	EXPECT_EQ(8, block2_.getTypeSize());	// double : 8[byte]
	// Test total size
	EXPECT_EQ(1 * 256, block0_.getTotalSize());
	EXPECT_EQ(4 * 512, block1_.getTotalSize());
	EXPECT_EQ(8 * 1024, block2_.getTotalSize());
}
TEST_F(BLOCK_TEST, Element) {
	EXPECT_EQ('e', block0_[0]);
	EXPECT_EQ('d', block0_[1]);
	EXPECT_EQ('a', block0_[2]);
	EXPECT_EQ('c', block0_[3]);

	EXPECT_EQ(2, block1_[0]);
	EXPECT_EQ(1, block1_[1]);
	EXPECT_EQ(5, block1_[2]);
	EXPECT_EQ(3, block1_[3]);

	EXPECT_EQ(0.1, block2_[0]);
	EXPECT_EQ(0.3, block2_[1]);
	EXPECT_EQ(0.5, block2_[2]);
	EXPECT_EQ(0.6, block2_[3]);
}
TEST_F(BLOCK_TEST, Pointer) {
	EXPECT_EQ(block0_.get(0) + 1, block0_.get(1));
	EXPECT_EQ(block0_.get(0) + 2, block0_.get(2));
	EXPECT_EQ(block0_.get(0) + 3, block0_.get(3));

	EXPECT_EQ(block1_.get(0) + 1, block1_.get(1));
	EXPECT_EQ(block1_.get(0) + 2, block1_.get(2));
	EXPECT_EQ(block1_.get(0) + 3, block1_.get(3));

	EXPECT_EQ(block2_.get(0) + 1, block2_.get(1));
	EXPECT_EQ(block2_.get(0) + 2, block2_.get(2));
	EXPECT_EQ(block2_.get(0) + 3, block2_.get(3));
}
