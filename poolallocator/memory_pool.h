#include <algorithm>

#include "bucket_cfg.h"

class memory_pool {
 public:
  struct info {
    std::size_t index{0};
    std::size_t block_count{0};
    std::size_t waste{0};

    bool operator<(const info &other) const noexcept {
      return (waste == other.waste) ? block_count < other.block_count : waste < other.waste;
    }
  };

  static bool cmp(const info &a, const info &b) {
    return a < b;
  }

  template <std::size_t id>
  [[nodiscard]] static void *allocate(std::size_t bytes) {
    auto &pool = get_instance<id>();
    std::array<info, bucket_count<id>> deltas;
    std::size_t index = 0;
    for (const auto &bucket : pool) {
      deltas[index].index = index;
      if (bucket.BlockSize >= bytes) {
        deltas[index].waste = bucket.BlockSize - bytes;
        deltas[index].block_count = 1;
      } else {
        const auto n = 1 + ((bytes - 1) / bucket.BlockSize);
        const auto storage_required = n * bucket.BlockSize;
        deltas[index].waste = storage_required - bytes;
        deltas[index].block_count = n;
      }
      ++index;
    }
    std::sort(deltas.begin(), deltas.end(), cmp);
    for (const auto &d : deltas)
      if (auto ptr = pool[d.index].allocate(bytes); ptr != nullptr) {
        return ptr;
      }
    throw std::bad_alloc{};
  }

  template <std::size_t id>
  static void deallocate(void *ptr, std::size_t bytes) noexcept {
    auto &pool = get_instance<id>();
    for (auto &bucket : pool) {
      if (bucket.belongs(ptr)) {
        bucket.deallocate(ptr, bytes);
        return;
      }
    }
  }

  template <size_t id>
  static constexpr bool is_defined() noexcept {
    return bucket_count<id> != 0;
  }

  template <size_t id>
  static bool initialize() noexcept {
    (void)get_instance<id>();
    return is_defined<id>();
  }
};