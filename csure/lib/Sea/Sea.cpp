#include "sl/Sea/Sea.h"

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

std::shared_ptr<Sea> Sea::New() { return std::shared_ptr<Sea>{new Sea{}}; }

std::shared_ptr<Drop> Sea::NewDrop() {
  std::shared_ptr<Drop> result =
      std::shared_ptr<Drop>{new Drop{shared_from_this()}};
  drops_.push_back(result);
  return result;
}

std::shared_ptr<ProofDrop> Sea::NewProofDrop() {
  std::shared_ptr<ProofDrop> result =
      std::shared_ptr<ProofDrop>{new ProofDrop{shared_from_this()}};
  drops_.push_back(result);
  return result;
}

std::vector<std::shared_ptr<Drop>> Sea::Drops() { return drops_; }

} // namespace sl
