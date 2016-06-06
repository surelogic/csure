#ifndef SL_COMMON_SLUTIL_H_
#define SL_COMMON_SLUTIL_H_

#include "llvm/Support/raw_ostream.h"

namespace sl {

/// \brief This is a helper to return llvm::errs() for debug output within
/// the SureLogic tools.
llvm::raw_ostream &l();

} // namespace sl

#endif // SL_COMMON_SLUTIL_H_
