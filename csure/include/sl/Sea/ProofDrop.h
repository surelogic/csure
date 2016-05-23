#ifndef SL_SEA_PROOFDROP_H_
#define SL_SEA_PROOFDROP_H_

#include "sl/Sea/Drop.h"

namespace sl {

class ProofDrop : public Drop {
  friend class Sea;

protected:
  // Invoked by the sea and subclass constructors.
  ProofDrop(std::shared_ptr<Sea> sea) : Drop{sea} {}
};

} // namespace sl

#endif // SL_SEA_PROOFDROP_H_
