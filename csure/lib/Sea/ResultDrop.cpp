#include "sl/Sea/ResultDrop.h"

namespace sl {

void ResultDrop::ProofInitialize() {
  proved_consistent_ = IsConsistent() || IsVouched();
  proof_uses_reddot_ = IsVouched();
}

bool ResultDrop::ProofTransfer() { return ProofTransferHelper(GetTrusted()); }

} // namespace sl
