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

std::shared_ptr<ProofDrop> Sea::NewProofDrop() {
  ClearOutInvalidDrops();
  std::shared_ptr<ProofDrop> result =
      std::shared_ptr<ProofDrop>{new ProofDrop{shared_from_this()}};
  drops_.insert(result);
  return result;
}

std::unordered_set<std::shared_ptr<Drop>> Sea::Drops() {
  ClearOutInvalidDrops();
  return drops_;
}

void Sea::ClearOutInvalidDrops() {
  for (auto it = drops_.begin(); it != drops_.end();)
    if (!(*it)->IsValid())
      it = drops_.erase(it);
    else
      ++it;
}

} // namespace sl
