#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace sl {

/**
 * \brief The base class for all drops within the sea, intended to allow
 * multiple implementations.
 *
 * All access to instances is via a shared_ptr.
 */
class Drop : public std::enable_shared_from_this<Drop> {
  // remove copy constructor and assignment (move as well)
  Drop(const Drop&) = delete;
  Drop& operator=(const Drop&) = delete;
  Drop(Drop&&) = delete;
  Drop& operator=(Drop&&) = delete;

 public:
  Drop() = default;

 public:
  static std::shared_ptr<Drop> create() {
    return std::shared_ptr<Drop>{new Drop{}};
  }

  std::string getMessage() { return "todo"; }

  std::string getMessageCanonical() { return "todo"; }

  // TODO: getCRef

  bool isFromSrc() { return false; }

  virtual ~Drop() { std::cout << "~Drop invoked\n"; };
};
}
