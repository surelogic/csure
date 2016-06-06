#ifndef SL_SEA_PROOFDROP_H_
#define SL_SEA_PROOFDROP_H_

#include "sl/Sea/Drop.h"

namespace sl {

// Forward declaration to avoid circular #include.
class AnalysisResultDrop;

/// \brief Represents a promise or a result used in the code/model
/// consistency proof.
class ProofDrop : public Drop {
  friend class Sea;

public:
  /// Returns if this drop has been judged to be consistent by
  /// Sea::UpdateConsistencyProof(): true if consistent, false otherwise
  /// (consistency can't be proved).
  bool ProvedConsistent() { return proved_consistent_; }

  /// Returns if the proof of this drop depends upon a "red dot" -- a
  /// user vouching for or assuming something which may not be true.
  bool ProofUsesRedDot() { return proof_uses_reddot_; }

  /// Set the message for this drop when consistency is proven.
  void SetMessageWhenProvedConsistent(const std::string &message) {
    message_consistent_ = message;
  }

  /// Returns the message for this drop when consistency is proven.
  std::string GetMessageWhenProvedConsistent() { return message_consistent_; }

  /// Set the message for this drop when consistency cannot be proven.
  void SetMessageWhenNotProvedConsistent(const std::string &message) {
    message_inconsistent_ = message;
  }

  /// Returns the message for this drop when consistency cannot be proven.
  std::string GetMessageWhenNotProvedConsistent() {
    return message_inconsistent_;
  }

  /// Returns the set of results which directly trust (as an "and" or as
  /// an "or" precondition) of this proof drop.
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> GetTrustedBy();

protected:
  /// Invoked by the sea and subclass constructors.
  ProofDrop(std::shared_ptr<Sea> sea) : Drop{sea} {}

  /// Called by Sea::UpdateConsistencyProof() on this proof drop to
  /// initialize its state for running the reverse flow analysis used
  /// by that function to calculate promise consistency.
  virtual void ProofInitialize() = 0;

  /// Called by Sea::UpdateConsistencyProof() on iteration to a fixed-point
  /// to allow this proof drop to examine all proof drops with a directed
  /// edge (in the drop-sea graph -- see Halloran's thesis) to this
  /// proof drop.
  virtual bool ProofTransfer() = 0;

  /// Called by Sea::UpdateConsistencyProof() on this proof drop after the
  /// consistency proof has been completed. This allows the drop to examine
  /// the results and make any state changes necessary.
  ///
  /// The default implementation changes the message based upon the analysis
  /// judgement, if judgement-based messages have been set, so overriding
  /// classes must invoked this one.
  virtual void ProofFinalize();

  /// Transfers data from all the passed proof drops.
  /// Returns true if something changes, false otherwise.
  bool ProofTransferHelper(
      std::unordered_set<std::shared_ptr<ProofDrop>> proof_drops);

  /// Transfers data from a single proof drop to this drop.
  /// Returns true if something changes, false otherwise.
  bool ProofTransferDropHelper(std::shared_ptr<ProofDrop> proof_drop);

  /// True if this drop has been judged to be consistent.
  bool proved_consistent_ = false;

  /// True if this drop depends upon a "red dot".
  bool proof_uses_reddot_ = false;

private:
  /// Output message when consistency is proven.
  std::string message_consistent_;

  /// Output message when consistency cannot be proven.
  std::string message_inconsistent_;
};

} // namespace sl

#endif // SL_SEA_PROOFDROP_H_
