#include "sl/Sea/ResultFolderDrop.h"

#include "sl/Sea/ResultDrop.h"
#include "sl/Sea/Sea.h"

namespace sl {

bool ResultFolderDrop::ImmediatelyConsistent() {
  bool result = true; // assume the best
  switch (op_) {
  case LogicOp::AND:
    ////////////////
    // AND Folder //
    ////////////////
    for (std::shared_ptr<ProofDrop> drop : GetTrusted()) {
      std::shared_ptr<ResultDrop> result_drop =
          std::dynamic_pointer_cast<ResultDrop>(drop);
      if (result_drop != nullptr)
        result &= result_drop->IsConsistent();
      else
        result &= drop->ProvedConsistent();
    }
    break;
  case LogicOp::OR:
    ///////////////
    // OR Folder //
    ///////////////
    result = ProvedConsistent();
    std::shared_ptr<ResultDrop> result_drop =
        std::dynamic_pointer_cast<ResultDrop>(choice_of_or_folder_);
    if (result_drop != nullptr)
      result = result_drop->IsConsistent();
    break;
  }
  return result;
}

void ResultFolderDrop::ProofInitialize() {
  proved_consistent_ = true;
  choice_of_or_folder_.reset();
  proof_uses_reddot_ = false;
}

bool ResultFolderDrop::ProofTransfer() {

  ////////////////
  // AND Folder //
  ////////////////
  if (op_ == LogicOp::AND)
    return ProofTransferHelper(GetTrusted());

  ///////////////
  // OR Folder //
  ///////////////

  // Our lattice -- lower index is better.
  // (consistency, reddot)
  bool lattice[4][2] = {
      {true, false}, {true, true}, {false, true}, {false, false},
  };

  int index_of_best_choice = 9999; // outside the lattice

  std::shared_ptr<ProofDrop> chosen_drop;
  for (std::shared_ptr<ProofDrop> choice : GetTrusted()) {
    for (int i = 0; i < 4; ++i) {
      if (lattice[i][0] == choice->ProvedConsistent() &&
          lattice[i][1] == choice->ProofUsesRedDot()) {
        if (i < index_of_best_choice) {
          index_of_best_choice = i;
          chosen_drop = choice;
        }
      }
    }
  }

  if (chosen_drop != nullptr) {
    choice_of_or_folder_ = chosen_drop;
    return ProofTransferDropHelper(chosen_drop);
  } else {
    return false;
  }
}

} // namespace sl
