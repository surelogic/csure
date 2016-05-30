#include "sl/Sea/Sea.h"

#include <algorithm>
#include <chrono>
#include <sstream>
#include <utility>
#include <vector>

// Uncomment the next line to get detailed diagnostic output from the analysis.
//#define SL_DEBUG
#include "sl/Common/SLDebug.h"
#include "sl/Common/SLUtil.h"

namespace sl {

// static
std::shared_ptr<Sea> *Sea::default_;

// static
std::shared_ptr<Sea> Sea::Default() {
  if (default_ == nullptr) {
    default_ = new std::shared_ptr<Sea>{new Sea{}};
  }
  return *default_;
}

// static
std::shared_ptr<Sea> Sea::New() { return std::shared_ptr<Sea>{new Sea{}}; }

std::shared_ptr<Drop> Sea::NewDrop() {
  std::shared_ptr<Drop> result =
      std::shared_ptr<Drop>{new Drop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<HintDrop> Sea::NewHint() {
  std::shared_ptr<HintDrop> result =
      std::shared_ptr<HintDrop>{new HintDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<MetricDrop> Sea::NewMetric() {
  std::shared_ptr<MetricDrop> result =
      std::shared_ptr<MetricDrop>{new MetricDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<StartsPromiseDrop> Sea::NewStartsPromise() {
  std::shared_ptr<StartsPromiseDrop> result =
      std::shared_ptr<StartsPromiseDrop>{
          new StartsPromiseDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ProposedPromiseDrop> Sea::NewProposedPromise() {
  std::shared_ptr<ProposedPromiseDrop> result =
      std::shared_ptr<ProposedPromiseDrop>{
          new ProposedPromiseDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultDrop> Sea::NewConsistentResult() {
  std::shared_ptr<ResultDrop> result =
      std::shared_ptr<ResultDrop>{new ResultDrop{shared_from_this()}};
  result->SetConsistent(true);
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultDrop> Sea::NewInconsistentResult() {
  std::shared_ptr<ResultDrop> result =
      std::shared_ptr<ResultDrop>{new ResultDrop{shared_from_this()}};
  result->SetConsistent(false);
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultFolderDrop> Sea::NewAndFolder() {
  std::shared_ptr<ResultFolderDrop> result = std::shared_ptr<ResultFolderDrop>{
      new ResultFolderDrop{shared_from_this(), LogicOp::AND}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultFolderDrop> Sea::NewOrFolder() {
  std::shared_ptr<ResultFolderDrop> result = std::shared_ptr<ResultFolderDrop>{
      new ResultFolderDrop{shared_from_this(), LogicOp::OR}};
  drops_.insert(result);
  return result;
}

std::unordered_set<std::shared_ptr<Drop>> Sea::GetDrops() { return drops_; }

unsigned int Sea::GetDropCount() { return drops_.size(); }

void Sea::UpdateConsistencyProof() {
  D(const std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();)

  // Get all the proof drops.
  std::unordered_set<std::shared_ptr<ProofDrop>> all_proof_drops =
      FilterDropsOfType<ProofDrop>(GetDrops());

  //
  // INITIALIZE drop-sea flow analysis-based automated proof.
  //
  for (auto drop : all_proof_drops) {
    drop->ProofInitialize();
  }

  //
  // ITERATE unti we reach a fixed-point, i.e., no changes.
  //
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto drop : all_proof_drops) {
      // Transfer from "higher" drops.
      changed |= drop->ProofTransfer();
    }
  }

  //
  // FINALIZE drop-sea flow analysis-based automated proof.
  //
  for (auto drop : all_proof_drops) {
    drop->ProofFinalize();
  }

  D(const std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();)
  D(const unsigned int duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();)
  D(l() << "sl::Sea done updating consistency proof: " << duration << " us\n";)
}

namespace {

// Streams into 'out' a message using a leveled structure. For example:
// |-msg1
// | |-msg2
// | |-msg2a
// | | |-msg3
//
// Warning: This method does not work with graphs yet! It will core dump.
void printMsg(std::ostringstream &out, int level, const std::string &msg) {
  if (level > 0)
    for (int i = 0; i < level; ++i)
      out << "| ";
  out << "|-" << msg << "\n";
}

// Forward declaration for recursive call.
void printPromises(
    std::ostringstream &out, int level,
    const std::unordered_set<std::shared_ptr<PromiseDrop>> &promises);

// Streams into 'out' result drops and their consistency status.
void printResults(
    std::ostringstream &out, int level,
    const std::unordered_set<std::shared_ptr<AnalysisResultDrop>> &results) {
  for (auto &result : results) {
    std::ostringstream msg;
    msg << (result->ProvedConsistent() ? "[positive]" : "[negative]");
    msg << " " << result->GetMessage();
    printMsg(out, level, msg.str());
    std::unordered_set<std::shared_ptr<PromiseDrop>> trusted_promises =
        Sea::FilterDropsOfType<PromiseDrop>(result->GetTrusted());
    printPromises(out, level + 1, trusted_promises);
    std::unordered_set<std::shared_ptr<AnalysisResultDrop>> trusted_results =
        Sea::FilterDropsOfType<AnalysisResultDrop>(result->GetTrusted());
    printResults(out, level + 1, trusted_results);
  }
}

// Streams into 'out' top-level promises and their consistency status.
void printPromises(
    std::ostringstream &out, int level,
    const std::unordered_set<std::shared_ptr<PromiseDrop>> &promises) {
  std::vector<std::pair<std::string, std::shared_ptr<PromiseDrop>>> sorted;
  for (auto promise : promises) {
    sorted.push_back(std::make_pair(promise->GetMessage(), promise));
  }
  std::sort(sorted.begin(), sorted.end());

  for (auto &pair : sorted) {
    std::ostringstream msg;
    auto promise = pair.second;
    msg << (promise->ProvedConsistent() ? "[consistent]" : "[not proved]");
    msg << " " << promise->GetMessage();
    printMsg(out, level, msg.str());
    printResults(out, level + 1, promise->GetCheckedBy());
  }
}

} // namespace

std::string Sea::ToString() {
  std::ostringstream out;
  std::unordered_set<std::shared_ptr<PromiseDrop>> promises =
      FilterDropsOfType<PromiseDrop>(GetDrops());
  printPromises(out, 0, promises);
  return out.str();
}

void Sea::Reset() {
  while (!drops_.empty()) {
    std::shared_ptr<Drop> drop = *drops_.begin();
    drop->Invalidate();
  }
  if (!drops_.empty()) {
    l() << "Sea::Reset() did not clear out sea (code bug).\n";
    // Forced removal of remaining drops.
    drops_.erase(drops_.begin(), drops_.end());
  }
}

void Sea::NotifyDropInvalidated(std::shared_ptr<Drop> drop) {
  drops_.erase(drop);
}

} // namespace sl
