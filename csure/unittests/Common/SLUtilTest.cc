#include "gtest/gtest.h"
#include "sl/Common/SLUtil.h"

TEST(UtilTest, General) {
  EXPECT_NE(nullptr, &sl::l());
}
