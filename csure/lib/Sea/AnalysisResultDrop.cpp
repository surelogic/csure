#include "sl/Sea/AnalysisResultDrop.h"

#include "sl/Sea/PromiseDrop.h"

namespace sl {

void AnalysisResultDrop::AddChecked(std::shared_ptr<PromiseDrop> promise) {
  checks_.insert(promise);
}

std::unordered_set<std::shared_ptr<PromiseDrop>>
AnalysisResultDrop::GetChecked() {
  return checks_;
}

bool AnalysisResultDrop::HasChecked() { return !checks_.empty(); }

namespace {

// Recursive helper call to determine if the passed result, directly or
// indirectly, checks a promise. Called from ChecksAPromise() below.
bool ChecksAPromiseHelper(
    std::shared_ptr<AnalysisResultDrop> result,
    std::unordered_set<std::shared_ptr<AnalysisResultDrop>> *examined) {
  // Directly checks a promise.
  if (result->HasChecked())
    return true;

  // Guard against infinite recursion.
  if (examined->count(result) == 0) {
    examined->insert(result);

    // Indirectly checks a promise.
    for (std::shared_ptr<AnalysisResultDrop> trusted_by_result :
         result->GetTrustedBy()) {
      if (ChecksAPromiseHelper(trusted_by_result, examined))
        return true;
    }
  }
  // Doesn't check a promise.
  return false;
}

} // namespace

bool AnalysisResultDrop::ChecksAPromise() {
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> examined;
  std::shared_ptr<AnalysisResultDrop> self =
      std::dynamic_pointer_cast<AnalysisResultDrop>(shared_from_this());
  return ChecksAPromiseHelper(self, &examined);
}

void AnalysisResultDrop::AddTrusted(std::shared_ptr<ProofDrop> proof_drop) {
  trusts_.insert(proof_drop);
}

std::unordered_set<std::shared_ptr<ProofDrop>>
AnalysisResultDrop::GetTrusted() {
  return trusts_;
}

bool AnalysisResultDrop::HasTrusted() { return !trusts_.empty(); }

void AnalysisResultDrop::ProofInitialize() {
  // Analysis result drops, by definition, can not start off with a red dot.
  proof_uses_reddot_ = false;
}

} // namespace sl
