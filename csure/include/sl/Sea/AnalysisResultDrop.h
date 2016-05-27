#ifndef SL_SEA_ANALYSISRESULTDROP_H_
#define SL_SEA_ANALYSISRESULTDROP_H_

#include "sl/Sea/ProofDrop.h"

namespace sl {

// Forward declaration to avoid circular #include.
class PromiseDrop;

// \brief A partial implementation of an analysis result that consolidates
// code shared by ResultDrop and ResultFolderDrop. In particular, management
// of the set of promise drops being checked, or established, by this result
// is provided by this class.
class AnalysisResultDrop : public ProofDrop {
  friend class Sea;

public:
  // Adds a promise to the set of promises this result establishes, or checks.
  void AddChecked(std::shared_ptr<PromiseDrop> promise);

  // Returns the set of promises that this result establishes.
  std::unordered_set<std::shared_ptr<PromiseDrop>> GetChecked();

  // Returns true if this result has any promises that it establishes.
  bool HasChecked();

  // Returns true if this result, directly or indirectly, checks a promise.
  bool ChecksAPromise();

  // Adds a proof drop to the set of drops that this result uses as a
  // prerequisite assertion, or trusts. For a result folder, this method adds
  // a proof drop into the folder.
  void AddTrusted(std::shared_ptr<ProofDrop> proof_drop);

  // Returns the set of drops that this result uses as prequisite assertions.
  std::unordered_set<std::shared_ptr<ProofDrop>> GetTrusted();

  // Returns true if this result has any prerequiste assertions.
  bool HasTrusted();

protected:
  // Invoked by the sea and subclass constructors.
  AnalysisResultDrop(std::shared_ptr<Sea> sea) : ProofDrop{sea} {}

  virtual void ProofInitialize() {}

  virtual bool ProofTransfer() { return false; }

private:
  std::unordered_set<std::shared_ptr<PromiseDrop>> checks_;
  std::unordered_set<std::shared_ptr<ProofDrop>> trusts_;
};

} // namespace sl

#endif // SL_SEA_ANALYSISRESULTDROP_H_
