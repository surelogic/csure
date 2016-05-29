#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

///////////////////////////////////////
// Test proof drop functionality works.
///////////////////////////////////////

TEST(ProofDropTest, TestMessages) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  EXPECT_EQ("+", p1->GetMessageWhenProvedConsistent());
  p1->SetMessageWhenNotProvedConsistent("x");
  EXPECT_EQ("x", p1->GetMessageWhenNotProvedConsistent());
}

TEST(ProofDropTest, TrustedPromise) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_FALSE(p1->IsCheckedByAnalysis());
  EXPECT_TRUE(p1->ProofUsesRedDot());
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

TEST(ProofDropTest, TwoResultsConsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r2->AddChecked(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r1->IsConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_TRUE(r2->IsConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
  EXPECT_FALSE(r2->ProofUsesRedDot());
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

TEST(ProofDropTest, SimpleConsistentWithRedDot) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  r1->AddChecked(p1);
  r1->SetVouched(true);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_FALSE(r1->IsConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, OneDirectInconsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r2->AddChecked(p1);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_EQ("x", p1->GetMessage());
  EXPECT_FALSE(r1->ProvedConsistent());
  EXPECT_FALSE(r1->IsConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_TRUE(r2->IsConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
  EXPECT_FALSE(r2->ProofUsesRedDot());
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

////////////////////
// Result folders //
////////////////////

TEST(ProofDropTest, ResultFolderAndConsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewAndFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  rf->AddTrusted(r1);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(rf->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderAndInconsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewAndFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_EQ("x", p1->GetMessage());
  EXPECT_FALSE(rf->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderAndConsistentWithRedDot) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewAndFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  r1->SetVouched(true);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(rf->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderOrConsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewOrFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewInconsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(rf->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderOrInconsistent) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewOrFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewInconsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_EQ("x", p1->GetMessage());
  EXPECT_FALSE(rf->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderOrConsistentWithNoRedDot) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewOrFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  r1->SetVouched(true);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(rf->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderOrConsistentWithRedDotVouchedResult) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewOrFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  r1->SetVouched(true);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewInconsistentResult();
  rf->AddTrusted(r3);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(rf->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(rf->ProofUsesRedDot());
}

TEST(ProofDropTest, ResultFolderOrConsistentWithRedDotTrustedPromise) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  p1->SetMessageWhenProvedConsistent("+");
  p1->SetMessageWhenNotProvedConsistent("x");
  std::shared_ptr<sl::ResultFolderDrop> rf = sea->NewOrFolder();
  rf->AddChecked(p1);
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  rf->AddTrusted(r1);
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  rf->AddTrusted(r2);
  std::shared_ptr<sl::ResultDrop> r3 = sea->NewConsistentResult();
  rf->AddTrusted(r3);
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  r3->AddTrusted(p2);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_EQ("+", p1->GetMessage());
  EXPECT_TRUE(rf->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(rf->ProofUsesRedDot());
}

/////////////////////////////////////
// Complex acyclic drop-sea graphs //
/////////////////////////////////////

TEST(ProofDropTest, ChainConsistent) {
  //  p1   ->   r1   ->   p2   ->   r2
  //    (checks)  (trusts)  (checks)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(p2->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(p2->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
  EXPECT_FALSE(r2->ProofUsesRedDot());
}

TEST(ProofDropTest, ChainConsistentWithRedDotVouchedResult) {
  //  p1   ->   r1   ->   p2   ->   r2
  //    (checks)  (trusts)  (checks)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  r2->SetVouched(true);
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(p2->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(p2->ProofUsesRedDot());
  EXPECT_TRUE(r1->ProofUsesRedDot());
  EXPECT_TRUE(r2->ProofUsesRedDot());
}

TEST(ProofDropTest, ChainConsistentWithRedDotTrustedPromise) {
  //  p1   ->   r1   ->   p2   ->   r2   ->   p3
  //    (checks)  (trusts)  (checks)  (trusts)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p3 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  r2->AddTrusted(p3);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(p2->ProvedConsistent());
  EXPECT_TRUE(p3->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(p2->ProofUsesRedDot());
  EXPECT_TRUE(p3->ProofUsesRedDot());
  EXPECT_TRUE(r1->ProofUsesRedDot());
  EXPECT_TRUE(r2->ProofUsesRedDot());
}

TEST(ProofDropTest, ChainInconsistent) {
  //  p1   ->   r1   ->   p2   ->   r2
  //    (checks)  (trusts)  (checks)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_FALSE(p2->ProvedConsistent());
  EXPECT_FALSE(r1->ProvedConsistent());
  EXPECT_FALSE(r2->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(p2->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
  EXPECT_FALSE(r2->ProofUsesRedDot());
}

////////////////////////////////////
// Complex cyclic drop-sea graphs //
////////////////////////////////////

TEST(ProofDropTest, SimpleCycleConsistent) {
  //  p1  ->  r1  ->  p1 (again)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, SimpleCycleInconsistent) {
  //  p1  ->  r1  ->  p1 (again)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p1);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_FALSE(r1->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, SimpleCycleConsistentWithRedDotVouchedResult) {
  //  p1  ->  r1  ->  p1 (again)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewInconsistentResult();
  r1->SetVouched(true);
  r1->AddChecked(p1);
  r1->AddTrusted(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(r1->ProofUsesRedDot());
}

TEST(ProofDropTest, CycleConsistent) {
  //  p1  ->  r1  ->  p2  -> r2  -> p1 (again)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewConsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  r2->AddTrusted(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(p2->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(p2->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
  EXPECT_FALSE(r2->ProofUsesRedDot());
}

TEST(ProofDropTest, CycleInconsistent) {
  //  p1  ->  r1  ->  p2  -> r2  -> p1 (again)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  r2->AddTrusted(p1);
  sea->UpdateConsistencyProof();
  EXPECT_FALSE(p1->ProvedConsistent());
  EXPECT_FALSE(p2->ProvedConsistent());
  EXPECT_FALSE(r1->ProvedConsistent());
  EXPECT_FALSE(r2->ProvedConsistent());
  EXPECT_FALSE(p1->ProofUsesRedDot());
  EXPECT_FALSE(p2->ProofUsesRedDot());
  EXPECT_FALSE(r1->ProofUsesRedDot());
  EXPECT_FALSE(r2->ProofUsesRedDot());
}

TEST(ProofDropTest, CycleConsistentWithRedDot) {
  //  p1  ->  r1  ->  p2  -> r2  -> p1 (again)
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::StartsPromiseDrop> p1 = sea->NewStartsPromise();
  std::shared_ptr<sl::StartsPromiseDrop> p2 = sea->NewStartsPromise();
  std::shared_ptr<sl::ResultDrop> r1 = sea->NewConsistentResult();
  std::shared_ptr<sl::ResultDrop> r2 = sea->NewInconsistentResult();
  r2->SetVouched(true);
  r1->AddChecked(p1);
  r1->AddTrusted(p2);
  r2->AddChecked(p2);
  r2->AddTrusted(p1);
  sea->UpdateConsistencyProof();
  EXPECT_TRUE(p1->ProvedConsistent());
  EXPECT_TRUE(p2->ProvedConsistent());
  EXPECT_TRUE(r1->ProvedConsistent());
  EXPECT_TRUE(r2->ProvedConsistent());
  EXPECT_TRUE(p1->ProofUsesRedDot());
  EXPECT_TRUE(p2->ProofUsesRedDot());
  EXPECT_TRUE(r1->ProofUsesRedDot());
  EXPECT_TRUE(r2->ProofUsesRedDot());
}
