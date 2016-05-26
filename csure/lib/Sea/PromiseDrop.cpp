#include "sl/Sea/PromiseDrop.h"

#include "sl/Sea/Sea.h"

namespace sl {

void PromiseDrop::ProofInitialize() {}

bool PromiseDrop::ProofTransfer() {}

std::unordered_set<std::shared_ptr<AnalysisResultDrop>>
PromiseDrop::GetCheckedBy() {
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> result;
  // Find the set of dependent results that check this drop ("trusts" doesn't
  // count).
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> ss =
      Sea::FilterDropsOfType<AnalysisResultDrop>(GetDependents());
  return result;
}

} // namespace sl
