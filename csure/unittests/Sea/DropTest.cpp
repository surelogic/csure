#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

///////////////////////////////////////
// Test basic drop functionality works.
///////////////////////////////////////

TEST(DropTest, EmptyMessage) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  EXPECT_EQ("", sea->NewDrop()->GetMessage());
}

TEST(DropTest, Message) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  const std::string test_msg = "a test message";
  drop->SetMessage(test_msg);
  EXPECT_EQ(test_msg, drop->GetMessage());
}

TEST(DropTest, InSea) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  EXPECT_EQ(sea, drop->InSea());
}

TEST(DropTest, EmptyDependents) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  EXPECT_FALSE(drop->HasDependents());
  EXPECT_EQ(0u, drop->GetDependentCount());
}

TEST(DropTest, AddDependent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  drop->AddDependent(second);
  EXPECT_TRUE(drop->HasDependents());
  EXPECT_EQ(1u, drop->GetDependentCount());
  EXPECT_FALSE(second->HasDependents());
  EXPECT_EQ(0u, second->GetDependentCount());
  EXPECT_FALSE(drop->HasDeponents());
  EXPECT_EQ(0u, drop->GetDeponentCount());
  EXPECT_TRUE(second->HasDeponents());
  EXPECT_EQ(1u, second->GetDeponentCount());
}

TEST(DropTest, EmptyDeponents) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  EXPECT_FALSE(drop->HasDeponents());
  EXPECT_EQ(0u, drop->GetDeponentCount());
}

TEST(DropTest, AddDeponent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  drop->AddDeponent(second);
  EXPECT_FALSE(drop->HasDependents());
  EXPECT_EQ(0u, drop->GetDependentCount());
  EXPECT_TRUE(second->HasDependents());
  EXPECT_EQ(1u, second->GetDependentCount());
  EXPECT_TRUE(drop->HasDeponents());
  EXPECT_EQ(1u, drop->GetDeponentCount());
  EXPECT_FALSE(second->HasDeponents());
  EXPECT_EQ(0u, second->GetDeponentCount());
}

TEST(DropTest, InvalidateDependent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  drop->AddDependent(second);
  second->Invalidate();
  EXPECT_TRUE(drop->IsValid());
  EXPECT_FALSE(second->IsValid());
}

TEST(DropTest, InvalidateDependent2) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  second->AddDeponent(drop);
  second->Invalidate();
  EXPECT_TRUE(drop->IsValid());
  EXPECT_FALSE(second->IsValid());
}

TEST(DropTest, InvalidateDeponent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  drop->AddDeponent(second);
  second->Invalidate();
  EXPECT_FALSE(drop->IsValid());
  EXPECT_FALSE(second->IsValid());
}

TEST(DropTest, InvalidateDeponent2) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  second->AddDependent(drop);
  second->Invalidate();
  EXPECT_FALSE(drop->IsValid());
  EXPECT_FALSE(second->IsValid());
}

TEST(DropTest, InvalidateTwoCircular) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  drop->AddDependent(second);
  second->AddDependent(drop);
  drop->Invalidate();
  EXPECT_FALSE(drop->IsValid());
  EXPECT_FALSE(second->IsValid());
}

TEST(DropTest, InvalidateTwoCircular2) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  drop->AddDeponent(second);
  second->AddDeponent(drop);
  drop->Invalidate();
  EXPECT_FALSE(drop->IsValid());
  EXPECT_FALSE(second->IsValid());
}

TEST(DropTest, GetDependents) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  std::shared_ptr<sl::Drop> third = sea->NewDrop();
  std::unordered_set<std::shared_ptr<sl::Drop>> expected{second, third};
  drop->AddDependent(second);
  drop->AddDependent(third);
  EXPECT_EQ(expected, drop->GetDependents());
  EXPECT_EQ(2u, drop->GetDependentCount());
}

TEST(DropTest, GetDependentsIndirect) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  std::shared_ptr<sl::Drop> third = sea->NewDrop();
  std::unordered_set<std::shared_ptr<sl::Drop>> expected{second};
  drop->AddDependent(second);
  second->AddDependent(third);
  EXPECT_EQ(expected, drop->GetDependents());
  EXPECT_EQ(1u, drop->GetDependentCount());
  EXPECT_EQ(1u, second->GetDependentCount());
}

TEST(DropTest, GetDeponents) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  std::shared_ptr<sl::Drop> third = sea->NewDrop();
  std::unordered_set<std::shared_ptr<sl::Drop>> expected{second, third};
  drop->AddDeponent(second);
  drop->AddDeponent(third);
  EXPECT_EQ(expected, drop->GetDeponents());
  EXPECT_EQ(2u, drop->GetDeponentCount());
}

TEST(DropTest, GetDeponentsIndirect) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::Drop> drop = sea->NewDrop();
  std::shared_ptr<sl::Drop> second = sea->NewDrop();
  std::shared_ptr<sl::Drop> third = sea->NewDrop();
  std::unordered_set<std::shared_ptr<sl::Drop>> expected{second};
  drop->AddDeponent(second);
  second->AddDeponent(third);
  EXPECT_EQ(expected, drop->GetDeponents());
  EXPECT_EQ(1u, drop->GetDeponentCount());
  EXPECT_EQ(1u, second->GetDeponentCount());
}

