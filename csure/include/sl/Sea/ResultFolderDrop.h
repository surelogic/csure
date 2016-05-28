#pragma once

#include "sl/Sea/AnalysisResultDrop.h"

namespace sl {

// Type of logic the result folder implements.
enum class LogicOp { AND, OR };

// \brief a model/code consistency result grouping a set of analysis results
// by a logical operator (AND / OR) to produce an analysis judgement.
class ResultFolderDrop final : public AnalysisResultDrop {
  friend class Sea;

public:
  // Returns the type of logic this result folder implements.
  LogicOp GetLogicOp() { return op_; }

protected:
  // Returns if the folder (by its logic operator) is immediately consistent.
  bool ImmediatelyConsistent();

  // Invoked by the sea and subclass constructors.
  ResultFolderDrop(std::shared_ptr<Sea> sea, LogicOp op)
      : AnalysisResultDrop{sea}, op_{op} {}

  virtual void ProofInitialize();

  virtual bool ProofTransfer();

private:
  // The type of logic this result folder implements.
  const LogicOp op_;
  // Holds the best choice within the contents of an OR-folder.
  std::shared_ptr<ProofDrop> choice_of_or_folder_;
};

} // namespace sl
