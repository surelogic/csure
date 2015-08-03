#include "gtest/gtest.h"
#include "sl/Sea/Sea.h"

TEST(DropTest, Message) {
  EXPECT_EQ("todo", sl::Drop::create()->getMessage());
}
