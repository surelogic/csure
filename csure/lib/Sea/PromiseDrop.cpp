#include "sl/Sea/PromiseDrop.h"

#include "sl/Sea/AnalysisResultDrop.h"
#include "sl/Sea/ResultDrop.h"
#include "sl/Sea/Sea.h"

/// @file

namespace sl {

bool PromiseDrop::IsCheckedByAnalysis() {
  if (!IsIntendedToBeCheckedByAnalysis())
    return true;
  std::unordered_set<std::shared_ptr<PromiseDrop>> examined;
  return IsCheckedByAnalysis(&examined);
}

bool PromiseDrop::IsCheckedByAnalysis(
    std::unordered_set<std::shared_ptr<PromiseDrop>> *examined) {
  std::shared_ptr<PromiseDrop> self =
      std::dynamic_pointer_cast<PromiseDrop>(shared_from_this());

  // Guard against infinite recursion.
  if (examined->count(self) == 0) {
    examined->insert(self);

    // Check if any dependent result checks this promise.
    if (!GetCheckedBy().empty())
      return true;

    // Check if any of our dependent promise drops are checked.
    std::unordered_set<std::shared_ptr<PromiseDrop>> p =
        Sea::FilterDropsOfType<PromiseDrop>(GetDependents());
    for (const auto &promise : p) {
      if (promise->IsCheckedByAnalysis(examined))
        return true;
    }
  }
  return false;
}

std::unordered_set<std::shared_ptr<AnalysisResultDrop>>
PromiseDrop::GetCheckedBy() {
  std::shared_ptr<PromiseDrop> self =
      std::dynamic_pointer_cast<PromiseDrop>(shared_from_this());

  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> result;

  // Find the set of dependent results that check this drop.
  // Note that "trusts" doesn't count.
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> r =
      Sea::FilterDropsOfType<AnalysisResultDrop>(GetDependents());
  for (auto &rd : r) {
    if (rd->GetChecked().count(self) != 0)
      result.insert(rd);
  }
  return result;
}

void PromiseDrop::ProofInitialize() {
  // For a promise drop we flag a reddot if it is not checked by analysis.
  proof_uses_reddot_ = !IsCheckedByAnalysis();

  proved_consistent_ = true; // assume the best

  // If no immediate result drops are inconsistent then we are consistent.
  // And-in local results, but only real results not result folders.
  std::unordered_set<std::shared_ptr<ResultDrop>> result_drops =
      Sea::FilterDropsOfType<ResultDrop>(GetCheckedBy());
  for (auto &result : result_drops) {
    proved_consistent_ &= result->IsConsistent() || result->IsVouched();
  }
}

bool PromiseDrop::ProofTransfer() {
  std::unordered_set<std::shared_ptr<ProofDrop>> proof_drops =
      Sea::FilterDropsOfType<ProofDrop>(GetCheckedBy());
  std::unordered_set<std::shared_ptr<PromiseDrop>> promises =
      Sea::FilterDropsOfType<PromiseDrop>(GetDependents());
  proof_drops.insert(promises.begin(), promises.end());
  return ProofTransferHelper(proof_drops);
}

} // namespace sl
