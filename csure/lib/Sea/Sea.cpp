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
void PrintMsg(std::ostringstream &out, int level, const std::string &msg) {
  static const char *line = u8"\u2502";
  static const char *line_r_dash = u8"\u251c";
  static const char *dash = u8"\u2500";
  if (level > 0)
    for (int i = 0; i < level; ++i)
      out << line << " ";
  out << line_r_dash << dash << msg << "\n";
}

// Utility function to order the unordered sets commonly used by the sea.
// This changes a std::unordered_set into a std::vector sorted by the
// drop's message string.
//
// This function is used by the sea output functions below.
template <typename T>
std::vector<std::shared_ptr<T>>
AlphaOrder(std::unordered_set<std::shared_ptr<T>> &drop_set) {
  std::vector<std::pair<std::string, std::shared_ptr<T>>> msgs_and_drops;
  for (auto &drop : drop_set)
    msgs_and_drops.push_back(std::make_pair(drop->GetMessage(), drop));
  std::sort(msgs_and_drops.begin(), msgs_and_drops.end());
  std::vector<std::shared_ptr<T>> result;
  for (auto &msg_and_drop : msgs_and_drops)
    result.push_back(msg_and_drop.second);
  return result;
}

// Forward declaration for mutually recursive calls below.
void PrintResult(
    std::ostringstream &out, int level,
    std::shared_ptr<AnalysisResultDrop> result,
    std::unordered_set<std::shared_ptr<PromiseDrop>> promises_already_printed);

// Streams into 'out' a promise and its consistency status, recursively,
// Mutually recursive with PrintResult() below.
void PrintPromise(
    std::ostringstream &out, int level, std::shared_ptr<PromiseDrop> promise,
    std::unordered_set<std::shared_ptr<PromiseDrop>> promises_already_seen) {
  bool recurse = true;
  if (promises_already_seen.count(promise) == 0)
    promises_already_seen.insert(promise);
  else
    recurse = false;

  // Output the promise information.
  static const char *arrow = u8"\u21ba";
  std::ostringstream msg;
  if (!recurse)
    msg << arrow;
  msg << (promise->ProvedConsistent() ? "[consistent]" : "[not proved]");
  msg << " " << promise->GetMessage();
  PrintMsg(out, level, msg.str());

  // Output results this promise is checked by.
  if (recurse) {
    std::unordered_set<std::shared_ptr<AnalysisResultDrop>> checked_by_results =
        promise->GetCheckedBy();
    for (auto result : AlphaOrder(checked_by_results)) {
      PrintResult(out, level + 1, result, promises_already_seen);
    }
  }
}

// Streams into 'out' a result and its consistency status, recursively,
// Mutually recursive with PrintPromise() above.
void PrintResult(
    std::ostringstream &out, int level,
    std::shared_ptr<AnalysisResultDrop> result,
    std::unordered_set<std::shared_ptr<PromiseDrop>> promises_already_seen) {
  // Output the result information.
  std::ostringstream msg;
  msg << (result->ProvedConsistent() ? "[positive]" : "[negative]");
  msg << " " << result->GetMessage();
  PrintMsg(out, level, msg.str());

  // Output trusted promises.
  std::unordered_set<std::shared_ptr<PromiseDrop>> trusted_promises =
      Sea::FilterDropsOfType<PromiseDrop>(result->GetTrusted());
  for (auto &trusted_promise : AlphaOrder(trusted_promises))
    PrintPromise(out, level + 1, trusted_promise, promises_already_seen);

  // Output trusted results.
  std::unordered_set<std::shared_ptr<AnalysisResultDrop>> trusted_results =
      Sea::FilterDropsOfType<AnalysisResultDrop>(result->GetTrusted());
  for (auto &trusted_result : AlphaOrder(trusted_results))
    PrintResult(out, level + 1, trusted_result, promises_already_seen);
}

} // namespace

std::string Sea::ToString() {
  std::ostringstream out;
  // Get all promises in the sea.
  std::unordered_set<std::shared_ptr<PromiseDrop>> promises =
      FilterDropsOfType<PromiseDrop>(GetDrops());
  // We need to guard against infinite recursion so we keep track, for
  // each promise, what other promises have been printed. We refer to
  // this set as 'promises_already_seen'.
  // The guard is seeing the same promise again.
  for (auto promise : AlphaOrder(promises)) {
    std::unordered_set<std::shared_ptr<PromiseDrop>>
        empty_promises_already_seen;
    PrintPromise(out, 0, promise, empty_promises_already_seen);
  }
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
