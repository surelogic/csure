#ifndef SL_SEA_RESULTDROP_H_
#define SL_SEA_RESULTDROP_H_

#include "sl/Sea/AnalysisResultDrop.h"

namespace sl {

// \brief A model/code consistency result recording an analysis judgement in
// terms of what promises are partially or wholly established in terms of a
// (possibly empty) set of prerequisite assertion promises.
class ResultDrop final : public AnalysisResultDrop {
  friend class Sea;

public:
  // Returns if this result indicates a consistent result.
  bool IsConsistent() { return consistent_; }

  // Sets whether this result indicates model/code consistency.
  void SetConsistent(const bool value) { consistent_ = value; }

  // Returns if this result is vouched.
  bool IsVouched() { return vouched_; }

  // Sets whether this result is being "vouched" for by a programmer even
  // though it is inconsistent.
  void SetVouched(const bool value) { vouched_ = value; }

protected:
  // Invoked by the sea and subclass constructors.
  ResultDrop(std::shared_ptr<Sea> sea) : AnalysisResultDrop{sea} {}

  virtual void ProofInitialize();

  virtual bool ProofTransfer();

private:
  // True if this result is consistent.
  bool consistent_ = false;
  // True if this result is vouched.
  bool vouched_ = false;
};

} // namespace sl

#endif // SL_SEA_RESULTDROP_H_
