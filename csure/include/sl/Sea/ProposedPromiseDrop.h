#ifndef SL_SEA_PROPOSEDPROMISEDROP_H_
#define SL_SEA_PROPOSEDPROMISEDROP_H_

#include "sl/Sea/Drop.h"

namespace sl {

// \brief Represents a proposed promise in the sea. A proposed promise
// indicates a missing portion of a model. Proposed promises are constructed
// by analyses and used by the tool user interface to help the programmer
// annotate their code.
class ProposedPromiseDrop final : public Drop {
  friend class Sea;

protected:
  // Invoked by the sea and subclass constructors.
  ProposedPromiseDrop(std::shared_ptr<Sea> sea) : Drop{sea} {}
};

} // namespace sl

#endif // SL_SEA_PROPOSEDPROMISEDROP_H_
