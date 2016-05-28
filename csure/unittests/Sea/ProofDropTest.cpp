#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

///////////////////////////////////////
// Test proof drop functionality works.
// This includes promises and results.
///////////////////////////////////////

TEST(ProofDropTest, SimpleConsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, SimpleRedDot) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
}

TEST(ProofDropTest, HasChecked) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  EXPECT_FALSE(r1->HasChecked());
  r1->AddChecked(p1);
  EXPECT_TRUE(r1->HasChecked());
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(r1->HasChecked());
}

TEST(ProofDropTest, HasTrusted) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  EXPECT_FALSE(r1->HasTrusted());
  r1->AddTrusted(p1);
  EXPECT_TRUE(r1->HasTrusted());
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(r1->HasTrusted());
}

TEST(ProofDropTest, ChecksAPromise) {
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
