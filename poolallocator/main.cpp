#include <iostream>
#include <list>
#include <vector>

#include "allocator.h"
int main() {
  static constexpr auto AllocId = 1;

  using list = std::list<int, static_pool_allocator<int, AllocId>>;
  using vector = std::vector<list, static_pool_allocator<list, AllocId>>;
  vector v;

  std::cout << "hello2";
  v.emplace_back(5u, 42);
  if (v.size() == 5)
    std::cout << "Ok";
  std::cout << "hello";
}