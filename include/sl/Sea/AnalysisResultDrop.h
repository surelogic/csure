#pragma once

#include "sl/Sea/AnalysisOutputDrop.h"
#include "sl/Sea/ProofDrop.h"

namespace sl {

class AnalysisResultDrop : public ProofDrop, public AnalysisOutputDrop {};
}
