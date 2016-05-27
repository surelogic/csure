#pragma once

#include "sl/Sea/AnalysisResultDrop.h"

namespace sl {

enum class LogicOp { AND, OR };

class ResultFolderDrop final : public AnalysisResultDrop {
  friend class Sea;

public:
  LogicOp GetLogicOp() { return op_; }

protected:
  // Invoked by the sea and subclass constructors.
  ResultFolderDrop(std::shared_ptr<Sea> sea, LogicOp op)
      : AnalysisResultDrop{sea}, op_{op} {}

  virtual void ProofInitialize();

  virtual bool ProofTransfer();

private:
  const LogicOp op_;
};
} // namespace sl
