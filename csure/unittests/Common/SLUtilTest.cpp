#include "sl/Common/SLUtil.h"
#include "gtest/gtest.h"

// Ensure that our logging object references an object.
TEST(UtilTest, General) { EXPECT_NE(nullptr, &sl::l()); }
