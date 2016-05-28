#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

////////////////////////////////////////
// Test result drop functionality works.
////////////////////////////////////////

TEST(ResultDropTest, GetChecked) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r1->AddChecked(p2);
  r2->AddChecked(p1);
  r3->AddTrusted(p1);
  EXPECT_EQ(2u, r1->GetChecked().size());
  EXPECT_EQ(1u, r1->GetChecked().count(p1));
  EXPECT_EQ(1u, r1->GetChecked().count(p2));

  EXPECT_EQ(1u, r2->GetChecked().size());
  EXPECT_EQ(1u, r2->GetChecked().count(p1));
  EXPECT_EQ(0u, r2->GetChecked().count(p2));

  EXPECT_EQ(0u, r3->GetChecked().size());
  EXPECT_EQ(0u, r3->GetChecked().count(p1));
  EXPECT_EQ(0u, r3->GetChecked().count(p2));
}

TEST(ResultDropTest, HasChecked) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  EXPECT_FALSE(r1->HasChecked());
  r1->AddChecked(p1);
  EXPECT_TRUE(r1->HasChecked());
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(r1->HasChecked());
}

TEST(ResultDropTest, GetTrusted) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  r1->AddTrusted(p1);
  r1->AddTrusted(p2);
  r2->AddTrusted(p1);
  r3->AddChecked(p1);
  EXPECT_EQ(2u, r1->GetTrusted().size());
  EXPECT_EQ(1u, r1->GetTrusted().count(p1));
  EXPECT_EQ(1u, r1->GetTrusted().count(p2));

  EXPECT_EQ(1u, r2->GetTrusted().size());
  EXPECT_EQ(1u, r2->GetTrusted().count(p1));
  EXPECT_EQ(0u, r2->GetTrusted().count(p2));

  EXPECT_EQ(0u, r3->GetTrusted().size());
  EXPECT_EQ(0u, r3->GetTrusted().count(p1));
  EXPECT_EQ(0u, r3->GetTrusted().count(p2));
}

TEST(ResultDropTest, HasTrusted) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  EXPECT_FALSE(r1->HasTrusted());
  r1->AddTrusted(p1);
  EXPECT_TRUE(r1->HasTrusted());
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(r1->HasTrusted());
}

TEST(ResultDropTest, ChecksAPromise) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  EXPECT_FALSE(r1->ChecksAPromise());
  EXPECT_FALSE(r2->ChecksAPromise());
  r1->AddChecked(p1);
  EXPECT_TRUE(r1->ChecksAPromise());
  EXPECT_FALSE(r2->ChecksAPromise());
  r1->AddTrusted(r2);
  EXPECT_TRUE(r1->ChecksAPromise());
  EXPECT_TRUE(r2->ChecksAPromise());
}
