#ifndef SL_SEA_SEA_H_
#define SL_SEA_SEA_H_

#include <memory>
#include <unordered_set>

#include "sl/Common/SLUtil.h"

// Drops
#include "sl/Sea/AnalysisResultDrop.h"
#include "sl/Sea/Drop.h"
#include "sl/Sea/HintDrop.h"
#include "sl/Sea/MetricDrop.h"
#include "sl/Sea/PromiseDrop.h"
#include "sl/Sea/ProofDrop.h"
#include "sl/Sea/ProposedPromiseDrop.h"
#include "sl/Sea/ResultDrop.h"
#include "sl/Sea/ResultFolderDrop.h"
#include "sl/Sea/StartsPromiseDrop.h"

namespace sl {

// \brief Represents a sea of knowledge, not intended to be subclassed. Sea
// instances contain and manage drops of information. Instances form a truth
// maintenance system by managing dependent and deponent drops.
//
// Most analysis use can us the default sea by calling sl::Sea::Default().
// For unit testing, and other special purpose code, sl::Sea::New() creates
// a new empty sea.
//
// All access to instances is via a shared_ptr.
class Sea final : public std::enable_shared_from_this<Sea> {
  friend class Drop;

public:
  ///////////////
  // ACCESSORS //
  ///////////////

  // Gets the default sea.
  static std::shared_ptr<Sea> Default();

  // Gets a new sea. Most analysis code should use Sea::Default().
  // This constructor is primarily intended for test use.
  static std::shared_ptr<Sea> New();

  ///////////////////////
  // UTILITY FUNCTIONS //
  ///////////////////////

  // Returns a new set containing all the members of the passed set that are
  // drops of type T.
  template <typename T>
  static std::unordered_set<std::shared_ptr<T>>
  FilterDropsOfType(const std::unordered_set<std::shared_ptr<Drop>> &drops);

  //////////////////////
  // MEMBER FUNCTIONS //
  //////////////////////

  // Constructs a new 'Drop' in this sea.
  std::shared_ptr<Drop> NewDrop();

  // Constructs a new 'HintDrop' in this sea.
  std::shared_ptr<HintDrop> NewHint();

  // Constructs a new 'MetricDrop' in this sea.
  std::shared_ptr<MetricDrop> NewMetric();

  // Constructs a new 'StartsPromiseDrop' in this sea.
  std::shared_ptr<StartsPromiseDrop> NewStartsPromise();

  // Constructs a new 'ProposedPromiseDrop' in this sea.
  std::shared_ptr<ProposedPromiseDrop> NewProposedPromise();

  // Constructs a new 'ResultDrop' in this sea.
  std::shared_ptr<ResultDrop> NewResult();

  // Constructs a new logical 'and' operator 'ResultFolderDrop' in this sea.
  std::shared_ptr<ResultFolderDrop> NewAndFolder();

  // Constructs a new logical 'or' operator 'ResultFolderDrop' in this sea.
  std::shared_ptr<ResultFolderDrop> NewOrFolder();

  // Gets all drops in this sea.
  std::unordered_set<std::shared_ptr<Drop>> GetDrops();

  // Gets the count of drops in this sea.
  unsigned int GetDropCount();

  // Causes the proof of model/code consistency to be run on this sea
  // of knowledge. Until this is done ProofDrop's do not contain valid
  // information. Normally this function should be invoked after all analysis
  // has been completed and results have been reported into this sea.
  //
  // This analysis is patterned after a reverse flow analysis. It uses the
  // following lattice:
  //
  //        consistent
  //   consistent / red-dot
  //  inconsistent / red-dot
  //       inconsistent
  //
  // Implementation Note: The methods ProofDrop::ProofInitialize() and
  // ProofDrop::ProofTransfer() are invoked by this algorithm.
  void UpdateConsistencyProof();

  // Invalidates all drops of knowldege in this sea. Resets it to a
  // empty sea instance.
  void Reset();

private:
  // Constructs a new sea of knowledge.
  Sea() = default;

  // Clears an drop that is no longer true out of 'drops_'.
  void NotifyDropInvalidated(std::shared_ptr<Drop> drop);

  // Holds all drops managed by this sea. Invalid drops are cleaned out
  // at each call to a non-static member function.
  std::unordered_set<std::shared_ptr<Drop>> drops_;

  // References the default sea.
  static std::shared_ptr<Sea> *default_;
};

////////////////////////////
// IMPLEMENTATION DETAILS //
////////////////////////////

// static
template <typename T>
std::unordered_set<std::shared_ptr<T>>
Sea::FilterDropsOfType(const std::unordered_set<std::shared_ptr<Drop>> &drops) {
  std::unordered_set<std::shared_ptr<T>> result;
  for (auto drop : drops) {
    std::shared_ptr<T> drop_t = std::dynamic_pointer_cast<T>(drop);
    if (drop_t != nullptr)
      result.insert(drop_t);
  }
  return result;
}

} // namespace sl

#endif // SL_SEA_SEA_H_
