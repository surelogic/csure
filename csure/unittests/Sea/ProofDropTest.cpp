#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

///////////////////////////////////////
// Test proof drop functionality works.
// This includes promises and results.
///////////////////////////////////////

TEST(ProofDropTest, TestMessages) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  EXPECT_EQ("+", p1->GetMessageWhenProvedConsistent());
  p1->SetMessageWhenNotProvedConsistent("x");
  EXPECT_EQ("x", p1->GetMessageWhenNotProvedConsistent());
}

TEST(ProofDropTest, SimpleConsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r1->IsConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, SimpleInconsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  r1->AddChecked(p1);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_EQ("x", p1->GetMessage());
  EXPECT_FALSE(r1->ProvedConsistent());
  EXPECT_FALSE(r1->IsConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, SimpleRedDot) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(p1->ProofUsesRedDot());
}
