#ifndef SL_SEA_RESULTDROP_H_
#define SL_SEA_RESULTDROP_H_

#include "sl/Sea/AnalysisResultDrop.h"

namespace sl {

class ResultDrop final : public AnalysisResultDrop {
  friend class Sea;

public:
  // Returns if this result is consistent.
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
  bool consistent_;
  bool vouched_;
};

} // namespace sl

#endif // SL_SEA_RESULTDROP_H_
