#include "sl/Sea/ResultFolderDrop.h"

namespace sl {

void ResultFolderDrop::ProofInitialize() {
  AnalysisResultDrop::ProofInitialize();

  //proved_consistent_ = IsConsistent() || IsVouched();
}

bool ResultFolderDrop::ProofTransfer() {
  return ProofTransferHelper(GetTrusted());
}

} // namespace sl
