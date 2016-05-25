#ifndef SL_SEA_PROMISEDROP_H_
#define SL_SEA_PROMISEDROP_H_

#include "sl/Sea/ProofDrop.h"

namespace sl {

// \brief Base class for all promises in the sea of knowledge. Within a
// analysis-based verification system, promises represent models of design
// intent or cutpoints for the analyses.
class PromiseDrop : public ProofDrop {
  friend class Sea;

public:
  bool IsCheckedByAnalysis() { return false; }

protected:
  // Invoked by the sea and subclass constructors.
  PromiseDrop(std::shared_ptr<Sea> sea) : ProofDrop{sea} {}

  virtual void ProofInitialize() {}

  virtual void ProofTransfer() {}
};

} // namespace sl

#endif // SL_SEA_PROMISEDROP_H_
