#include "sl/Sea/ResultDrop.h"

namespace sl {

void ResultDrop::ProofInitialize() {
  AnalysisResultDrop::ProofInitialize();

  proved_consistent_ = IsConsistent() || IsVouched();
}

bool ResultDrop::ProofTransfer() {
  return ProofTransferHelper(GetTrusted());
}

} // namespace sl
