#ifndef SL_SEA_ANALYSISRESULTDROP_H_
#define SL_SEA_ANALYSISRESULTDROP_H_

#include "sl/Sea/ProofDrop.h"

namespace sl {

// \brief A partial implementation of an analysis result that consolidates
// code shared by ResultDrop and ResultFolderDrop. In particular, management
// of the set of promise drops being checked, or established, by this result
// is provided by this class.
class AnalysisResultDrop : public ProofDrop {
  friend class Sea;

public:
protected:
  // Invoked by the sea and subclass constructors.
  AnalysisResultDrop(std::shared_ptr<Sea> sea) : ProofDrop{sea} {}

  virtual void ProofInitialize() {}

  virtual bool ProofTransfer() { return false; }
};

} // namespace sl

#endif // SL_SEA_ANALYSISRESULTDROP_H_
