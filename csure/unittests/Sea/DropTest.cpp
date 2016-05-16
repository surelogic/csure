#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

TEST(DropTest, Message) { EXPECT_EQ("todo", sl::Drop::create()->getMessage()); }
