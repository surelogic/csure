#include "sl/Sea/ProofDrop.h"

#include "sl/Sea/AnalysisResultDrop.h"
#include "sl/Sea/Sea.h"

namespace sl {

std::unordered_set<std::shared_ptr<AnalysisResultDrop>>
ProofDrop::GetTrustedBy() {
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> result;
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> s =
      Sea::FilterDropsOfType<AnalysisResultDrop>(GetDependents());
  for (std::shared_ptr<AnalysisResultDrop> rd : s) {
    if (rd->GetTrusted().count(
            std::dynamic_pointer_cast<ProofDrop>(shared_from_this())) != 0) {
      result.insert(rd);
    }
  }
  return result;
}

bool ProofDrop::ProofTransferHelper(
    std::unordered_set<std::shared_ptr<ProofDrop>> proof_drops) {
  // Assume nothing is going to change.
  bool changed = false;

  for (auto drop : proof_drops) {
    changed |= ProofTransferDropHelper(drop);
  }

  return changed;
}

bool ProofDrop::ProofTransferDropHelper(std::shared_ptr<ProofDrop> proof_drop) {
  // Assume nothing is going to change.
  bool changed = false;

  // All must be consistent for this drop to be consistent.
  if (proved_consistent_ && !proof_drop->proved_consistent_) {
    proved_consistent_ = false;
    changed = true;
  }

  // Any red dot means this drop is contingent and is marked with a red dot.
  if (proof_uses_red_dot_ && !proof_drop->proof_uses_red_dot_) {
    proof_uses_red_dot_ = false;
    changed = true;
  }

  return changed;
}

void ProofDrop::ProofFinalize() {
  if (ProvedConsistent()) {
    if (message_consistent_ != "")
      SetMessage(message_consistent_);
  } else {
    if (message_inconsistent_ != "")
      SetMessage(message_inconsistent_);
  }
}

} // namespace sl
