#ifndef SL_SEA_METRICDROP_H_
#define SL_SEA_METRICDROP_H_

#include "sl/Sea/Drop.h"

namespace sl {

/// \brief Allows the analysis to output metrics about the code or
/// the analysis of the code to the tool user.
class MetricDrop final : public Drop {
  friend class Sea;

protected:
  /// Invoked by the sea and subclass constructors.
  MetricDrop(std::shared_ptr<Sea> sea) : Drop{sea} {}
};

} // namespace sl

#endif // SL_SEA_METRICDROP_H_
