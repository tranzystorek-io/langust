#include "interpreter/MemoryArena.hpp"

#include <algorithm>
#include <utility>
#include <tuple>

using namespace langust::interpreter;

bool MemoryArena::isDefined(const std::string& varname) const {
  if(!stack_.empty() && stack_.top().count(varname))
    return true;

  return global_.count(varname);
}

bool MemoryArena::isStackEmpty() const {
  return stack_.empty();
}

MemoryArena::Variable& MemoryArena::at(const std::string& varname) {
  if(!stack_.empty()) {
    VarSpace& current = stack_.top();

    if(current.count(varname))
      return current.at(varname);
  }

  return global_.at(varname);
}

void MemoryArena::put(const std::string& varname, const Variable& var) {
  if(!stack_.empty()) {
    stack_.top().insert(std::make_pair(varname, var));
  }
  else
    global_.insert(std::make_pair(varname, var));
}

void MemoryArena::enterFrame() {
  stack_.emplace();
}

void MemoryArena::exitFrame() {
  stack_.pop();
}

bool Object::operator==(const Object& other) const {
  if(vtype != other.vtype)
    return false;

  switch(vtype) {
  case VarType::INT:
    return i == other.i;

  case VarType::BOOL:
    return b == other.b;

  case VarType::LIST:
    if(list->size() != other.list->size())
      return false;
    return std::equal(list->begin(), list->end(),
                      other.list->begin());

  default:
    return false;
  }
}

bool Object::operator!=(const Object& other) const {
  return !(*this == other);
}
