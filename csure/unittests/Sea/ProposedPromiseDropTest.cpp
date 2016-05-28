#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

//////////////////////////////////////////////////
// Test proposed promise drop functionality works.
//////////////////////////////////////////////////

TEST(ProposedPromiseDropTest, CheckFilter) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ProposedPromiseDrop> prop = sea->NewProposedPromise();
  std::unordered_set<std::shared_ptr<sl::ProposedPromiseDrop>> props =
      sl::Sea::FilterDropsOfType<sl::ProposedPromiseDrop>(sea->GetDrops());
  EXPECT_EQ(1u, props.size());
  EXPECT_EQ(1u, props.count(prop));
}

TEST(ProposedPromiseDropTest, CheckPromiseProposal) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ProposedPromiseDrop> prop = sea->NewProposedPromise();
  std::shared_ptr<sl::StartsPromiseDrop> promise = sea->NewStartsPromise();
  promise->AddDependent(prop);
  std::unordered_set<std::shared_ptr<sl::Drop>> deps = promise->GetDependents();
  std::unordered_set<std::shared_ptr<sl::Drop>> depo = prop->GetDeponents();
  EXPECT_EQ(1u, deps.count(prop));
  EXPECT_EQ(1u, depo.count(promise));
}

TEST(ProposedPromiseDropTest, CheckResultProposal) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::ProposedPromiseDrop> prop = sea->NewProposedPromise();
  std::shared_ptr<sl::ResultDrop> result = sea->NewResult();
  result->AddDependent(prop);
  std::unordered_set<std::shared_ptr<sl::Drop>> deps = result->GetDependents();
  std::unordered_set<std::shared_ptr<sl::Drop>> depo = prop->GetDeponents();
  EXPECT_EQ(1u, deps.count(prop));
  EXPECT_EQ(1u, depo.count(result));
}
