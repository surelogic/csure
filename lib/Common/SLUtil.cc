#include "sl/Common/SLUtil.h"

namespace sl {

	llvm::raw_ostream& l() {
		return llvm::errs();
	}

}

