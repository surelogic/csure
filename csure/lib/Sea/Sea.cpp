#include "sl/Sea/Sea.h"

#include <algorithm>
#include <chrono>

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

std::shared_ptr<ResultDrop> Sea::NewResult() {
  std::shared_ptr<ResultDrop> result =
      std::shared_ptr<ResultDrop>{new ResultDrop{shared_from_this()}};
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
  const std::chrono::steady_clock::time_point start =
      std::chrono::steady_clock::now();

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

  const std::chrono::steady_clock::time_point end =
      std::chrono::steady_clock::now();
  const unsigned int duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();
  l() << "Done updating consistency proof: " << duration << " us";
}

void Sea::Reset() {
  for (auto drop : drops_) {
    drop->Invalidate();
  }
  if (!drops_.empty()) {
    l() << "Sea::Reset() did not clear out sea (code bug).";
    // Forced removal of remaining drops.
    drops_.erase(drops_.begin(), drops_.end());
  }
}

void Sea::NotifyDropInvalidated(std::shared_ptr<Drop> drop) {
  drops_.erase(drop);
}

} // namespace sl
