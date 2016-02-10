#pragma once

#include "llvm/Support/raw_ostream.h"

namespace sl {

/// <summary>This is a helper to return llvm::errs() for debug output within
/// llvm</summary>
llvm::raw_ostream& l();
}
