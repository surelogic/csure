#ifndef SL_SEA_SEA_H_
#define SL_SEA_SEA_H_

#include <memory>
#include <unordered_set>

#include "sl/Common/SLUtil.h"

// Drops
//#include "sl/Sea/AnalysisOutputDrop.h"
//#include "sl/Sea/AnalysisResultDrop.h"
#include "sl/Sea/Drop.h"
//#include "sl/Sea/HintDrop.h"
//#include "sl/Sea/MetricDrop.h"
//#include "sl/Sea/PromiseDrop.h"
#include "sl/Sea/ProofDrop.h"
//#include "sl/Sea/ProposedPromiseDrop.h"
//#include "sl/Sea/ResultDrop.h"
//#include "sl/Sea/ResultFolderDrop.h"

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
public:
  // Gets the default sea.
  static std::shared_ptr<Sea> Default();

  // Gets a new sea. Most analysis code should use Sea::Default().
  // This constructor is primarily intended for test use.
  static std::shared_ptr<Sea> New();

  // Constructs a new 'Drop' in this sea.
  std::shared_ptr<Drop> NewDrop();

  // Constructs a new 'ProofDrop' in this sea.
  std::shared_ptr<ProofDrop> NewProofDrop();

  // Gets all drops in this sea.
  std::unordered_set<std::shared_ptr<Drop>> Drops();

private:
  // Constructs a sea. Most analysis code should use Sea::Default().
  // This constructor is primarily intended for test use.
  Sea() = default;

  // Clears any invalid drops out of 'drops_'.
  void ClearOutInvalidDrops();

  // Holds all drops managed by this sea. Invalid drops are cleaned out
  // at each call to a non-static member function.
  std::unordered_set<std::shared_ptr<Drop>> drops_;

  // References the default sea.
  static std::shared_ptr<Sea> *default_;
};

} // namespace sl

#endif // SL_SEA_SEA_H_
