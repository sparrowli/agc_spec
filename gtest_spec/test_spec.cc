#include "test.h"

#include "gtest/gtest.h"



TEST(TESTSPEC, PositiveNos) {
	EXPECT_EQ(18.0, square_root(324.0));
}


int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

