#ifndef SL_SEA_STARTSPROMISEDROP_H_
#define SL_SEA_STARTSPROMISEDROP_H_

#include "sl/Sea/ProofDrop.h"

namespace sl {

// \brief Base class for all promises in the sea of knowledge. Within a
// analysis-based verification system, promises represent models of design
// intent or cutpoints for the analyses.
class StartsPromiseDrop final : public PromiseDrop {
  friend class Sea;

public:
  bool IsCheckedByAnalysis() { return false; }

protected:
  // Invoked by the sea and subclass constructors.
  StartsPromiseDrop(std::shared_ptr<Sea> sea) : PromiseDrop{sea} {}
};

} // namespace sl

#endif // SL_SEA_STARTSPROMISEDROP_H_
