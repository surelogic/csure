#include "sl/Common/SLUtil.h"
#include "gtest/gtest.h"

TEST(UtilTest, General) { EXPECT_NE(nullptr, &sl::l()); }
