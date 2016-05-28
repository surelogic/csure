#ifndef SL_SEA_PROMISEDROP_H_
#define SL_SEA_PROMISEDROP_H_

#include "sl/Sea/ProofDrop.h"

namespace sl {

// Forward declaration to avoid circular #include.
class AnalysisResultDrop;

// \brief Base class for all promises in the sea of knowledge. Within a
// analysis-based verification system, promises represent models of design
// intent or cutpoints for the analyses.
class PromiseDrop : public ProofDrop {
  friend class Sea;

public:
  // Returns if this promise is intended to be checked by analysis or not.
  // Most promises are supported by analysis results (i.e., they have
  // AnalysisResultDrops attached to them), however some are simply
  // well-formed (e.g., definitional models). If the promise is simply
  // well-formed then it should override this function and return false.
  virtual bool IsIntendedToBeCheckedByAnalysis() { return true; }

  // Returns if this promise has been checked by analysis or not. If this
  // promise (or any deponent promise of this promise drop) has results
  // it is considered to be checked, otherwise it is considered to be
  // trusted.
  //
  // If a promise does not need to be checked by analysis you should
  // override IsIntendedToBeCheckedByAnalysis() and return false;
  bool IsCheckedByAnalysis();

  // Returns the set of dependent results that directly check this promise.
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> GetCheckedBy();

protected:
  // Invoked by the sea and subclass constructors.
  PromiseDrop(std::shared_ptr<Sea> sea) : ProofDrop{sea} {}

  virtual void ProofInitialize();

  virtual bool ProofTransfer();

  // Helper for IsCheckedByAnalysis() that guards against cyclecs in the
  // drop-sea graph.
  bool IsCheckedByAnalysis(
      std::unordered_set<std::shared_ptr<PromiseDrop>> *examined);
};

} // namespace sl

#endif // SL_SEA_PROMISEDROP_H_
