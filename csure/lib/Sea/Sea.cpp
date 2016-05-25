#include "sl/Sea/Sea.h"

#include <algorithm>

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
  ClearOutInvalidDrops();
  std::shared_ptr<Drop> result =
      std::shared_ptr<Drop>{new Drop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<HintDrop> Sea::NewHint() {
  ClearOutInvalidDrops();
  std::shared_ptr<HintDrop> result =
      std::shared_ptr<HintDrop>{new HintDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<MetricDrop> Sea::NewMetric() {
  ClearOutInvalidDrops();
  std::shared_ptr<MetricDrop> result =
      std::shared_ptr<MetricDrop>{new MetricDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<StartsPromiseDrop> Sea::NewStartsPromise() {
  ClearOutInvalidDrops();
  std::shared_ptr<StartsPromiseDrop> result =
      std::shared_ptr<StartsPromiseDrop>{
          new StartsPromiseDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ProposedPromiseDrop> Sea::NewProposedPromise() {
  ClearOutInvalidDrops();
  std::shared_ptr<ProposedPromiseDrop> result =
      std::shared_ptr<ProposedPromiseDrop>{
          new ProposedPromiseDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultDrop> Sea::NewResult() {
  ClearOutInvalidDrops();
  std::shared_ptr<ResultDrop> result =
      std::shared_ptr<ResultDrop>{new ResultDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultFolderDrop> Sea::NewAndFolder() {
  ClearOutInvalidDrops();
  std::shared_ptr<ResultFolderDrop> result = std::shared_ptr<ResultFolderDrop>{
      new ResultFolderDrop{shared_from_this(), LogicOp::AND}};
  drops_.insert(result);
  return result;
}

std::shared_ptr<ResultFolderDrop> Sea::NewOrFolder() {
  ClearOutInvalidDrops();
  std::shared_ptr<ResultFolderDrop> result = std::shared_ptr<ResultFolderDrop>{
      new ResultFolderDrop{shared_from_this(), LogicOp::OR}};
  drops_.insert(result);
  return result;
}

std::unordered_set<std::shared_ptr<Drop>> Sea::GetDrops() {
  ClearOutInvalidDrops();
  return drops_;
}

unsigned int Sea::GetDropCount() {
  ClearOutInvalidDrops();
  return drops_.size();
}

void Sea::UpdateConsistencyProof() {
  // TODO
}

void Sea::Reset() {
  for (auto drop : drops_) {
    drop->Invalidate();
  }
  ClearOutInvalidDrops();
  if (!drops_.empty()) {
    l() << "Sea::Reset() did not clear out sea (code bug).";
    // Forced removal of remaining drops.
    drops_.erase(drops_.begin(), drops_.end());
  }
}

void Sea::ClearOutInvalidDrops() {
  for (auto it = drops_.begin(); it != drops_.end();)
    if (!(*it)->IsValid())
      it = drops_.erase(it);
    else
      ++it;
}

} // namespace sl
