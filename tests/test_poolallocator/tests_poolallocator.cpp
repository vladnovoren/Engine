#include <gtest/gtest.h>

#include <cstdint>

#include "../../poolallocator/allocator.h"

/*
Example:

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
*/

TEST(BucketTestsSystem, Bucket) {
  bucket test_bucket(8, 3);
  uint8_t* p = NULL;
  uint8_t* d = NULL;
  p = (uint8_t*)(test_bucket.allocate(2));
  EXPECT_FALSE(test_bucket.belongs(d));
  EXPECT_TRUE(test_bucket.belongs(p));
  EXPECT_NE(p, nullptr);
  test_bucket.deallocate(p, 2);
  EXPECT_TRUE(test_bucket.belongs(p));
}