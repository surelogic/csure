#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

TEST(SeaTest, EmptySea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  EXPECT_EQ(0u, sea->Drops().size());
}

TEST(SeaTest, OneDropSea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::Drop>> drops;
  drops.insert(sea->NewDrop());
  EXPECT_EQ(1u, sea->Drops().size());
  EXPECT_EQ(drops, sea->Drops());
}

TEST(SeaTest, TwoDropSea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::Drop>> drops;
  drops.insert(sea->NewDrop());
  drops.insert(sea->NewDrop());
  EXPECT_EQ(2u, sea->Drops().size());
  EXPECT_EQ(drops, sea->Drops());
}

TEST(SeaTest, FiftyDropSea) {
  const unsigned int fifty = 50;
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::Drop>> drops;
  for (unsigned int i = 0; i < fifty; ++i)
    drops.insert(sea->NewDrop());
  EXPECT_EQ(fifty, sea->Drops().size());
  EXPECT_EQ(drops, sea->Drops());
}
