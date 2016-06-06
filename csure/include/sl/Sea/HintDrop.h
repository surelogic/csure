#ifndef SL_SEA_HINTDROP_H_
#define SL_SEA_HINTDROP_H_

#include "sl/Sea/Drop.h"

namespace sl {

/// \brief Allows the analysis to give "hints" (suggestions and warnings) to
/// the tool user.
class HintDrop final : public Drop {
  friend class Sea;

protected:
  /// Invoked by the sea and subclass constructors.
  HintDrop(std::shared_ptr<Sea> sea) : Drop{sea} {}
};

} // namespace sl

#endif // SL_SEA_HINTDROP_H_
