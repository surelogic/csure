#ifndef SL_SEA_PROOFDROP_H_
#define SL_SEA_PROOFDROP_H_

#include "sl/Sea/Drop.h"

namespace sl {

// \brief Represents a promise or a result used in the code/model
// consistency proof.
class ProofDrop : public Drop {
  friend class Sea;

  // Returns if this drop has been judged to be consistent by
  // Sea::UpdateConsistencyProof(): true if consistent, false otherwise
  // (consistency can't be proved).
  bool ProvedConsistent() { return proved_consistent_; }

  // Returns if the proof of this drop depends upon a "red dot" -- a
  // user vouching for or assuming something which may not be true.
  bool ProofUsesRedDot() { return proof_uses_red_dot_; }

protected:
  // Invoked by the sea and subclass constructors.
  ProofDrop(std::shared_ptr<Sea> sea) : Drop{sea} {}

  virtual void ProofInitialize() = 0;

  virtual void ProofTransfer() = 0;

private:
  // True if this drop has been judged to be consistent.
  bool proved_consistent_;

  // True if this drop depends upon a "red dot".
  bool proof_uses_red_dot_;
};

} // namespace sl

#endif // SL_SEA_PROOFDROP_H_
