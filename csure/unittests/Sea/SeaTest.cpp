#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

////////////////////////////////////////////////////
// These tests focus on the sea knowledge container.
////////////////////////////////////////////////////

TEST(SeaTest, AllDropTypes) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  sea->NewDrop();
  sea->NewHint();
  EXPECT_EQ(2u, sea->GetDropCount());
}
TEST(SeaTest, EmptySea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  EXPECT_EQ(0u, sea->GetDrops().size());
}

TEST(SeaTest, OneDropSea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::Drop>> drops;
  drops.insert(sea->NewDrop());
  EXPECT_EQ(1u, sea->GetDrops().size());
  EXPECT_EQ(drops, sea->GetDrops());
}

TEST(SeaTest, TwoDropSea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::Drop>> drops;
  drops.insert(sea->NewDrop());
  drops.insert(sea->NewDrop());
  EXPECT_EQ(2u, sea->GetDrops().size());
  EXPECT_EQ(drops, sea->GetDrops());
}

TEST(SeaTest, FiftyDropSea) {
  const unsigned int fifty = 50;
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::Drop>> drops;
  for (unsigned int i = 0; i < fifty; ++i)
    drops.insert(sea->NewDrop());
  EXPECT_EQ(fifty, sea->GetDrops().size());
  EXPECT_EQ(fifty, sea->GetDropCount());
  EXPECT_EQ(drops, sea->GetDrops());
}

TEST(SeaTest, GetDropCount) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  EXPECT_EQ(0u, sea->GetDropCount());
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  EXPECT_EQ(1u, sea->GetDropCount());
  drop->Invalidate();
  EXPECT_EQ(0u, sea->GetDropCount());
}

TEST(SeaTest, ResetOfSea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  EXPECT_EQ(1u, sea->GetDropCount());
  sea->Reset();
  EXPECT_EQ(0u, sea->GetDropCount());
}

TEST(SeaTest, FilterDropsOfType) {
  const unsigned int fifty = 50;
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::unordered_set<std::shared_ptr<sl::HintDrop>> proof_drops;
  for (unsigned int i = 0; i < fifty; ++i) {
    sea->NewDrop();
    proof_drops.insert(sea->NewHint());
  }
  EXPECT_EQ(fifty * 2, sea->GetDropCount());
  std::unordered_set<std::shared_ptr<sl::HintDrop>> drops =
      sl::Sea::FilterDropsOfType<sl::HintDrop>(sea->GetDrops());
  EXPECT_EQ(fifty, drops.size());
  EXPECT_EQ(proof_drops, drops);
}

//////////////////////////////////////
// Test creation of each type of drop.
//////////////////////////////////////

TEST(SeaTest, Drop) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, HintDrop) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::HintDrop> drop = sea->NewHint();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, MetricDrop) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::MetricDrop> drop = sea->NewMetric();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, StartsPromiseDrop) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> drop = sea->NewStartsPromise();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, ProposedPromiseDrop) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ProposedPromiseDrop> drop = sea->NewProposedPromise();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, ResultDrop) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ResultDrop> drop = sea->NewResult();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, ResultFolderDrop_AND) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ResultFolderDrop> drop = sea->NewAndFolder();
  EXPECT_EQ(1u, sea->GetDropCount());
}

TEST(SeaTest, ResultFolderDrop_OR) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ResultFolderDrop> drop = sea->NewOrFolder();
  EXPECT_EQ(1u, sea->GetDropCount());
}
