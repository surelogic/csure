#include "sl/Sea/Drop.h"

namespace sl {

void Drop::AddDependent(std::shared_ptr<Drop> dependent) {
  if (!is_valid_ || dependent == nullptr || !dependent->IsValid() ||
      dependent == shared_from_this() || InSea() != dependent->InSea() ||
      dependents_.count(dependent) != 0)
    return;
  dependents_.insert(dependent);
  dependent->deponents_.insert(shared_from_this());
}

void Drop::AddDeponent(std::shared_ptr<Drop> deponent) {
  if (!is_valid_ || deponent == nullptr || !deponent->IsValid() ||
      deponent == shared_from_this() || InSea() != deponent->InSea() ||
      deponents_.count(deponent) != 0)
    return;
  deponents_.insert(deponent);
  deponent->dependents_.insert(shared_from_this());
}

void Drop::Invalidate() {
  if (!is_valid_) return;
  InvalidateInternal();

  is_valid_ = false;

  // Inform all deponent drops.
  for (auto deponent : deponents_) {
    deponent->removeDependent(shared_from_this());
  }
  deponents_.erase(deponents_.begin(), deponents_.end());

  // Inform all dependent drops.
  for (auto dependent : dependents_) {
    dependent->removeDeponent(shared_from_this());
  }
  dependents_.erase(dependents_.begin(), dependents_.end());

  // TODO Notify sea observers DropEvent.Invalidated
}

void Drop::removeDependent(std::shared_ptr<Drop> dependent) {
  if (!is_valid_ || dependent == nullptr || dependent == shared_from_this() ||
      dependents_.count(dependent) == 0)
    return;

  dependents_.erase(dependent);
  // TODO Notify sea observers DropEvent.DependentInvalidated
  DependentInvalidAction();
}

void Drop::removeDeponent(std::shared_ptr<Drop> deponent) {
  if (!is_valid_ || deponent == nullptr || deponents_.count(deponent) == 0)
    return;

  deponents_.erase(deponent);
  // TODO Notify sea observers DropEvent.DeponentInvalidated
  DeponentInvalidAction();
}

} // namespace sl
