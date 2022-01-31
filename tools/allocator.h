#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

/**************************************************************************************************************/
/*
██████╗░██╗░░░██╗░█████╗░██╗░░██╗███████╗████████╗
██╔══██╗██║░░░██║██╔══██╗██║░██╔╝██╔════╝╚══██╔══╝
██████╦╝██║░░░██║██║░░╚═╝█████═╝░█████╗░░░░░██║░░░
██╔══██╗██║░░░██║██║░░██╗██╔═██╗░██╔══╝░░░░░██║░░░
██████╦╝╚██████╔╝╚█████╔╝██║░╚██╗███████╗░░░██║░░░
╚═════╝░░╚═════╝░░╚════╝░╚═╝░░╚═╝╚══════╝░░░╚═╝░░░

██╗███╗░░░███╗██████╗░██╗░░░░░███████╗███╗░░░███╗███████╗███╗░░██╗████████╗░█████╗░████████╗██╗░█████╗░███╗░░██╗
██║████╗░████║██╔══██╗██║░░░░░██╔════╝████╗░████║██╔════╝████╗░██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
██║██╔████╔██║██████╔╝██║░░░░░█████╗░░██╔████╔██║█████╗░░██╔██╗██║░░░██║░░░███████║░░░██║░░░██║██║░░██║██╔██╗██║
██║██║╚██╔╝██║██╔═══╝░██║░░░░░██╔══╝░░██║╚██╔╝██║██╔══╝░░██║╚████║░░░██║░░░██╔══██║░░░██║░░░██║██║░░██║██║╚████║
██║██║░╚═╝░██║██║░░░░░███████╗███████╗██║░╚═╝░██║███████╗██║░╚███║░░░██║░░░██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
╚═╝╚═╝░░░░░╚═╝╚═╝░░░░░╚══════╝╚══════╝╚═╝░░░░░╚═╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝
*/
/**************************************************************************************************************/

/*!
 * \brief memory bucket
 * \author Andrey Krotov
 * \version 0.1
 * \date 31.1.22
 * Class bucket - a section of memory containing blocks of the same size.
 */
class bucket {
 public:
  const std::size_t BlockSize;  /*!< Memory allocated for an object */
  const std::size_t BlockCount; /*!< Number of blocks with the same amount of memory */

  bucket(std::size_t block_size, std::size_t block_count); /*!< Constructor for memory buckets */

  ~bucket(); /*!< Destructor for memory buckets */

  bool belongs(void *ptr) const noexcept; /*!< Checks block in bucket */

  [[nodiscard]] void *allocate(std::size_t bytes) noexcept; /*!< Allocate bytes of memory in block */

  void deallocate(void *ptr, std::size_t bytes) noexcept; /*!< Deallocate bytes of memory in block by pointer */

 private:
  std::size_t find_contiguous_blocks(
      std::size_t n) const noexcept; /*!< Search for a memory area of size n * BlockSize */

  void set_block_in_use(std::size_t index, std::size_t n) noexcept; /*!< Marks the block size n as used */

  void set_block_free(std::size_t index, std::size_t n) noexcept; /*!< Removes labels from blocks that they are used */

  std::byte *m_data{nullptr};   /*!< Actual memory for allocations */
  std::byte *m_ledger{nullptr}; /*!< Reserves one bit per block to indicate whether it is in-use */
};

/*!
 * Counts the distance from the bucket to the pointer,
 * if the pointer is contained then the block is inside the bucket.
 * @param ptr Pointer to the memory block
 */
bool bucket::belongs(void *ptr) const noexcept {
  const auto p = static_cast<const std::byte *>(ptr);
  const std::size_t dist = static_cast<std::size_t>(p - m_data);

  if (dist >= BlockCount)
    return false;
  return true;
}

/*!
 * Search for a memory area of size n * BlockSize
 * @param n Count of empty block
 * @return Position in bucket of the first free n blocks or position of the end of bucket
 */
std::size_t bucket::find_contiguos_blocks(std::size_t n) const noexcept {
  std::size_t index = 0;       /*!< Index for moving block */
  std::size_t exit_index = 0;  /*!< Position of the first free position in block */
  std::size_t count = 0;       /*!< Count of busy blocks in bucket */
  std::size_t block_index = 0; /*!< Number of block in bucket */
  while (index + (block_index * BlockSize) < BlockCount) {
    /*! Changing to the next block in the bucket */
    if ((index % 8 == 0) && (index != 0)) {
      block_index = block_index + 1;
      index = 0;
      exit_index = 0;
    }
    /*! Checking the block's occupancy */
    if (((*(m_ledger + block_index) >> index) & (std::byte)(1)) == (std::byte)(0)) {
      count++;
    } else {
      count = 0;
      exit_index = index + 1;
    }

    if (count == n) {
      return exit_index + (block_index * BlockSize);
    }
    /*! Moving in bucket for the next position */
    index++;
  }
  return BlockCount;
}

/*!
 * Marks the block size n as used
 * @param index The initial position of the block
 * @param n Size of section
 */
void set_block_in_use(std::size_t index, std::size_t n) noexcept {
  /*! Counting block index */
  std::size_t block_index = index / BlockSize;
  index %= BlockSize;
  std::size_t count = 0;
  /*! Marking positions as used */
  while (index + (block_index * BlockSize) < BlockCount && count < n) {
    if ((index % BlockSize == 0) && (index != 0)) {
      block_index = block_index + 1;
      index = 0;
    }
    *(m_ledger + block_index) &= (~((std::byte)(1) << (index)));
    index++;
    count++;
  }
}

/*!
 * Removes labels from blocks that they are used
 * @param index The initial position of the block
 * @param n Size of section
 */
void bucket::set_blocks_free(std::size_t index, std::size_t n) noexcept {
  /*! Counting block index */
  std::size_t block_index = index / BlockSize;
  index %= BlockSize;
  std::size_t count = 0;
  /*! Removing labels */
  while (index + (block_index * BlockSize) < BlockCount && count < n) {
    if ((index % BlockSize == 0) && (index != 0)) {
      block_index = block_index + 1;
      index = 0;
    }
    *(m_ledger + block_index) &= (~((std::byte)(1) << (index)));
    index++;
    count++;
  }
}

bucket::bucket(std::size_t block_size, std::size_t block_count) : BlockSize{block_size}, BlockCount{block_count} {
  /*! Calculating and allocating the pool size */
  const auto data_size = BlockCount * BlockSize;
  m_data = static_cast<std::byte *>(std::malloc(data_size));
  assert(m_data != nullptr);
  /*! Allocating memory for markers of used blocks*/
  const auto ledger_size = 1 + ((BlockCount - 1) / 8;
  m_ledger = static_cast<std::byte*>(std::malloc(ledger_size));
  assert(m_ledger != nullptr);

  std::memset(m_data, 0, data_size);
  std::memset(m_ledger, 0, ledger_size);
}

bucket::~bucket() {
  std::free(m_ledger);
  std::free(m_data);
}

[[nodiscard]] void *bucket::allocate(std::size_t bytes) noexcept {
  const auto n = 1 + ((bytes - 1) / BlockSize);

  const auto index = find_contiguous_blocks(n);
  if (index == BlockCount) {
    return nullptr;
  }

  set_blocks_in_use(index, n);
  return m_data + (index * BlockSize);
}

void bucket::deallocate(void *ptr, std::size_t bytes) noexcept {
  const auto p = static_cast<const std::byte *>(ptr);
  const std::size_t dist = static_cast<std::size_t>(p - m_data);

  const auto index = dist / BlockSize;

  const auto n = 1 + ((bytes - 1) / BlockSize);

  set_blocks_free(index, n);
}

/**************************************************************************************************************/
/*
███╗░░░███╗███████╗███╗░░░███╗░█████╗░██████╗░██╗░░░██╗  ██████╗░░█████╗░░█████╗░██╗░░░░░
████╗░████║██╔════╝████╗░████║██╔══██╗██╔══██╗╚██╗░██╔╝  ██╔══██╗██╔══██╗██╔══██╗██║░░░░░
██╔████╔██║█████╗░░██╔████╔██║██║░░██║██████╔╝░╚████╔╝░  ██████╔╝██║░░██║██║░░██║██║░░░░░
██║╚██╔╝██║██╔══╝░░██║╚██╔╝██║██║░░██║██╔══██╗░░╚██╔╝░░  ██╔═══╝░██║░░██║██║░░██║██║░░░░░
██║░╚═╝░██║███████╗██║░╚═╝░██║╚█████╔╝██║░░██║░░░██║░░░  ██║░░░░░╚█████╔╝╚█████╔╝███████╗
╚═╝░░░░░╚═╝╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░  ╚═╝░░░░░░╚════╝░░╚════╝░╚══════╝

██╗███╗░░░███╗██████╗░██╗░░░░░███████╗███╗░░░███╗███████╗███╗░░██╗████████╗░█████╗░████████╗██╗░█████╗░███╗░░██╗
██║████╗░████║██╔══██╗██║░░░░░██╔════╝████╗░████║██╔════╝████╗░██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
██║██╔████╔██║██████╔╝██║░░░░░█████╗░░██╔████╔██║█████╗░░██╔██╗██║░░░██║░░░███████║░░░██║░░░██║██║░░██║██╔██╗██║
██║██║╚██╔╝██║██╔═══╝░██║░░░░░██╔══╝░░██║╚██╔╝██║██╔══╝░░██║╚████║░░░██║░░░██╔══██║░░░██║░░░██║██║░░██║██║╚████║
██║██║░╚═╝░██║██║░░░░░███████╗███████╗██║░╚═╝░██║███████╗██║░╚███║░░░██║░░░██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
╚═╝╚═╝░░░░░╚═╝╚═╝░░░░░╚══════╝╚══════╝╚═╝░░░░░╚═╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝
*/
/**************************************************************************************************************/

template <std::size_t id>
struct bucket_descriptors {
  using type = std::tuple<>;
};

/*!
 * Bucket configurations for objects
 */

struct bucket_cfg16 {
  static constexpr std::size_t BlockSize = 16;
  static constexpr std::size_t BlockCount = 10000;
};

struct bucket_cfg32 {
  static constexpr std::size_t BlockSize = 32;
  static constexpr std::size_t BlockCount = 10000;
};

struct bucket_cfg1024 {
  static constexpr std::size_t BlockSize = 1024;
  static constexpr std::size_t BlockCount = 50000;
};

template <>
struct bucket_descriptors<1> {
  using type = std::tuple<bucket_cfg16, bucket_cfg32, bucket_cfg1024>;
};

template <std::size_t id>
using bucket_descriptors_t = typename bucket_descriptors<id>::type;

template <std::size_t id>
static constexpr std::size_t bucket_count = std::tuple_size<bucket_descriptors_t<id>>::value;

template <std::size_t id>
using pool_type = std::array<bucket, bucket_count<id>>;

template <std::size_t id, std::size_t Idx>
struct get_size : std::integral_constant<std::size_t, std::tuple_element_t<Idx, bucket_descriptors_t<id>>::BlockSize> {
};

template <std::size_t id, std::size_t Idx>
struct get_count
    : std::integral_constant<std::size_t, std::tuple_element_t<Idx, bucket_descriptors_t<id>>::BlockCount> {};

template <std::size_t id, std::size_t... Idx>
auto &get_instance(std::index_sequence<Idx...>) noexcept {
  static pool_type<id> instance{{{get_size<id, Idx>::value, get_count<id, Idx>::value}...}};
  return instance;
}

template <std::size_t id>
auto &get_instance() noexcept {
  return get_instance<id>(std::make_index_sequence<bucket_count<id>>());
}

/*
░█████╗░██████╗░███████╗░█████╗░████████╗██╗███╗░░██╗░██████╗░  ███╗░░░███╗███████╗███╗░░░███╗░█████╗░██████╗░██╗░░░██╗
██╔══██╗██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██║████╗░██║██╔════╝░  ████╗░████║██╔════╝████╗░████║██╔══██╗██╔══██╗╚██╗░██╔╝
██║░░╚═╝██████╔╝█████╗░░███████║░░░██║░░░██║██╔██╗██║██║░░██╗░  ██╔████╔██║█████╗░░██╔████╔██║██║░░██║██████╔╝░╚████╔╝░
██║░░██╗██╔══██╗██╔══╝░░██╔══██║░░░██║░░░██║██║╚████║██║░░╚██╗  ██║╚██╔╝██║██╔══╝░░██║╚██╔╝██║██║░░██║██╔══██╗░░╚██╔╝░░
╚█████╔╝██║░░██║███████╗██║░░██║░░░██║░░░██║██║░╚███║╚██████╔╝  ██║░╚═╝░██║███████╗██║░╚═╝░██║╚█████╔╝██║░░██║░░░██║░░░
░╚════╝░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝╚═╝░░╚══╝░╚═════╝░  ╚═╝░░░░░╚═╝╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░

██████╗░░█████╗░░█████╗░██╗░░░░░░██████╗
██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔════╝
██████╔╝██║░░██║██║░░██║██║░░░░░╚█████╗░
██╔═══╝░██║░░██║██║░░██║██║░░░░░░╚═══██╗
██║░░░░░╚█████╔╝╚█████╔╝███████╗██████╔╝
╚═╝░░░░░░╚════╝░░╚════╝░╚══════╝╚═════╝░
*/

class memory_pool {
  struct info {
    std::size_t index{0};
    std::size_t block_count{0};
    std::size_t waste{0};

    bool operator<(const info &other) const noexcept {
      return (waste == other.waste) ? block_count < other.block_count : waste < other.waste;
    }
  };

  bool cmp(const info &a, const info &b) {
    return a < b;
  }

  template <std::size_t id>
  [[nodiscard]] void *allocate(std::size_t bytes) {
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
  void deallocate(void *ptr, std::size_t bytes) noexcept {
    auto &pool = get_instance<id>();
    for (auto &bucket : pool) {
      if (bucket.belongs(ptr)) {
        bucket.deallocate(ptr, bytes);
        return;
      }
    }
  }

  template <size_t id>
  constexpr bool is_defined() noexcept {
    return bucket_count<id> != 0;
  }

  template <size_t id>
  bool initialize() noexcept {
    (void)get_instance<id>();
    return is_defined<id>();
  }
};

#endif  // !POOLALLOCATOR_H
