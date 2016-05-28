#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

//////////////////////////////////////
// Test hint drop functionality works.
//////////////////////////////////////

TEST(HintDropTest, CheckFilter) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::HintDrop> hint = sea->NewHint();
  std::unordered_set<std::shared_ptr<sl::HintDrop>> hints =
      sl::Sea::FilterDropsOfType<sl::HintDrop>(sea->GetDrops());
  EXPECT_EQ(1u, hints.size());
  EXPECT_EQ(1u, hints.count(hint));
}

TEST(HintDropTest, CheckPromiseHint) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::HintDrop> hint = sea->NewHint();
  std::shared_ptr<sl::StartsPromiseDrop> promise = sea->NewStartsPromise();
  promise->AddDependent(hint);
  std::unordered_set<std::shared_ptr<sl::Drop>> deps = promise->GetDependents();
  std::unordered_set<std::shared_ptr<sl::Drop>> depo = hint->GetDeponents();
  EXPECT_EQ(1u, deps.count(hint));
  EXPECT_EQ(1u, depo.count(promise));
}

TEST(HintDropTest, CheckResultHint) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::HintDrop> hint = sea->NewHint();
  std::shared_ptr<sl::ResultDrop> result = sea->NewResult();
  result->AddDependent(hint);
  std::unordered_set<std::shared_ptr<sl::Drop>> deps = result->GetDependents();
  std::unordered_set<std::shared_ptr<sl::Drop>> depo = hint->GetDeponents();
  EXPECT_EQ(1u, deps.count(hint));
  EXPECT_EQ(1u, depo.count(result));
}
