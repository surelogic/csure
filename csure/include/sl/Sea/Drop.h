#ifndef SL_SEA_DROP_H_
#define SL_SEA_DROP_H_

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

namespace sl {

// Forward declaration to avoid circular #include.
class Sea;

/// \brief The base class for all drops within the sea, intended to allow
/// multiple implementations.
///
/// All access to instances is via a shared_ptr.
class Drop : public std::enable_shared_from_this<Drop> {
  friend class Sea;

public:
  /// Gets the sea that this drop is within.
  std::shared_ptr<Sea> InSea() { return sea_; }

  /// Gets whether this drop is valid or not.
  /// A drop being valid indicates that the knowledge represented by the drop
  /// is currently still supported within the truth maintenance system.
  bool IsValid() { return is_valid_; }

  /// Sets this drop's message.
  void SetMessage(const std::string &value) { message_ = value; }

  /// Gets this drop's message.
  /// If no message has been set then the empty string is returned.
  std::string GetMessage() { return message_; }

  /// Adds a dependent drop to this drop, meaning that the truth of the passed
  /// drop relies upon the truth of this drop. This drop is made a deponent of
  /// the passed drop.
  void AddDependent(std::shared_ptr<Drop> dependent);

  /// Indicates if this drop has any dependent drops.
  bool HasDependents() { return !dependents_.empty(); }

  /// Returns all immediate dependent drops of this drop.
  std::unordered_set<std::shared_ptr<Drop>> GetDependents() {
    return dependents_;
  }

  /// Returns the number of immediate dependent drops of this drop.
  unsigned int GetDependentCount() { return dependents_.size(); }

  /// Adds a deponent drop to this drop, meaning that the truth of this drop
  /// relies upon the truth of the passed drop. This drop is made a dependent
  /// of the passed drop.
  void AddDeponent(std::shared_ptr<Drop> deponent);

  /// Indicates if this drop has any deponent drops.
  bool HasDeponents() { return !deponents_.empty(); }

  /// Returns all immediate deponent drops of this drop.
  std::unordered_set<std::shared_ptr<Drop>> GetDeponents() {
    return deponents_;
  }

  /// Returns the number of immediate deponent drops of this drop.
  unsigned int GetDeponentCount() { return deponents_.size(); }

  /// Invalidates, or makes false, the information that this drop represents.
  /// It is critial that any std::shared_ptr instances pointing from this drop
  /// to other drops or from other drops to this drop be deleted.
  virtual void Invalidate();

  /// Destructor of all drops.
  virtual ~Drop() {}

protected:
  /// Invoked when a dependent drop is invalidated. This function has no effect,
  /// however, a subclass may override to change this behavior. This function's
  /// behavior is consistent with truth maintenance system use: the truth
  /// of this drop should not depend upon the truth of any dependent drop.
  virtual void DependentInvalidAction() {}

  /// Invoked when a deponent drop is invalidated causing this drop to be
  /// invalidated as well. A subclass may override to change this behavior. This
  /// function's behavior is consistent with truth maintenance system use: the
  /// truth of this drop should depend upon the truth of any deponent drop.
  virtual void DeponentInvalidAction() { Invalidate(); }

  /// Invoked by the sea and subclass constructors.
  Drop(std::shared_ptr<Sea> sea) : is_valid_{true}, sea_{sea} {}

private:
  // Remove copy constructor and assignment (move as well).
  Drop(const Drop &) = delete;
  Drop &operator=(const Drop &) = delete;
  Drop(Drop &&) = delete;
  Drop &operator=(Drop &&) = delete;

  void removeDependent(std::shared_ptr<Drop> dependent);
  void removeDeponent(std::shared_ptr<Drop> deponent);

  bool is_valid_;
  std::shared_ptr<Sea> sea_;
  std::unordered_set<std::shared_ptr<Drop>> dependents_;
  std::unordered_set<std::shared_ptr<Drop>> deponents_;
  std::string message_;
};

} // namespace sl

#endif // SL_SEA_DROP_H_
