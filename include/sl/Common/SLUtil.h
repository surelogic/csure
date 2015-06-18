#pragma once

#include "llvm/Support/Debug.h"

namespace sl {

	/// <summary>This is a helper to return llvm::errs() for debug output within llvm</summary>
	llvm::raw_ostream& l();

}
