#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

// These tests focus on the sea knowledge container.

TEST(SeaTest, AllDropTypes) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  sea->NewDrop();
  sea->NewProofDrop();
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
  std::unordered_set<std::shared_ptr<sl::ProofDrop>> proof_drops;
  for (unsigned int i = 0; i < fifty; ++i) {
    sea->NewDrop();
    proof_drops.insert(sea->NewProofDrop());
  }
  EXPECT_EQ(fifty * 2, sea->GetDropCount());
  std::unordered_set<std::shared_ptr<sl::ProofDrop>> drops =
      sl::Sea::FilterDropsOfType<sl::ProofDrop>(sea->GetDrops());
  EXPECT_EQ(fifty, drops.size());
  EXPECT_EQ(proof_drops, drops);
}
