#include "sl/Sea/Sea.h"
#include "gtest/gtest.h"

////////////////////////////////////////
// Test metric drop functionality works.
////////////////////////////////////////

TEST(MetricDropTest, CheckFilter) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::MetricDrop> metric = sea->NewMetric();
  std::unordered_set<std::shared_ptr<sl::MetricDrop>> metrics =
      sl::Sea::FilterDropsOfType<sl::MetricDrop>(sea->GetDrops());
  EXPECT_EQ(1u, metrics.size());
  EXPECT_EQ(1u, metrics.count(metric));
}

TEST(MetricDropTest, CheckPromiseMetric) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::MetricDrop> metric = sea->NewMetric();
  std::shared_ptr<sl::StartsPromiseDrop> promise = sea->NewStartsPromise();
  promise->AddDependent(metric);
  std::unordered_set<std::shared_ptr<sl::Drop>> deps = promise->GetDependents();
  std::unordered_set<std::shared_ptr<sl::Drop>> depo = metric->GetDeponents();
  EXPECT_EQ(1u, deps.count(metric));
  EXPECT_EQ(1u, depo.count(promise));
  promise->Invalidate();
  EXPECT_FALSE(metric->IsValid());
  EXPECT_FALSE(promise->IsValid());
  EXPECT_TRUE(sea->GetDrops().empty());
}

TEST(MetricDropTest, CheckResultMetric) {
  std::shared_ptr<sl::Sea> sea{sl::Sea::New()};
  std::shared_ptr<sl::MetricDrop> metric = sea->NewMetric();
  std::shared_ptr<sl::ResultDrop> result = sea->NewConsistentResult();
  result->AddDependent(metric);
  std::unordered_set<std::shared_ptr<sl::Drop>> deps = result->GetDependents();
  std::unordered_set<std::shared_ptr<sl::Drop>> depo = metric->GetDeponents();
  EXPECT_EQ(1u, deps.count(metric));
  EXPECT_EQ(1u, depo.count(result));
  result->Invalidate();
  EXPECT_FALSE(metric->IsValid());
  EXPECT_FALSE(result->IsValid());
  EXPECT_TRUE(sea->GetDrops().empty());
}
