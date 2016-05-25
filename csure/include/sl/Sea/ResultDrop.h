#ifndef SL_SEA_RESULTDROP_H_
#define SL_SEA_RESULTDROP_H_

#include "sl/Sea/AnalysisResultDrop.h"

namespace sl {

class ResultDrop final : public AnalysisResultDrop {
  friend class Sea;

protected:
  // Invoked by the sea and subclass constructors.
  ResultDrop(std::shared_ptr<Sea> sea) : AnalysisResultDrop{sea} {}
};

} // namespace sl

#endif // SL_SEA_RESULTDROP_H_
