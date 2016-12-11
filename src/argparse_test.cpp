#include <gtest/gtest.h>

TEST (dummy, dummy)
{
  ASSERT_EQ(0, 0);
}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
