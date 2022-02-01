#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

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

  uint8_t *m_data{nullptr};   /*!< Actual memory for allocations */
  uint8_t *m_ledger{nullptr}; /*!< Reserves one bit per block to indicate whether it is in-use */
};

/*!
 * Counts the distance from the bucket to the pointer,
 * if the pointer is contained then the block is inside the bucket.
 * @param ptr Pointer to the memory block
 */
bool bucket::belongs(void *ptr) const noexcept {
  const auto p = static_cast<const uint8_t *>(ptr);
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
std::size_t bucket::find_contiguous_blocks(std::size_t n) const noexcept {
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
    if (((*(m_ledger + block_index) >> index) & (uint8_t)(1)) == (uint8_t)(0)) {
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
void bucket::set_block_in_use(std::size_t index, std::size_t n) noexcept {
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
    *(m_ledger + block_index) &= (~((uint8_t)(1) << (index)));
    index++;
    count++;
  }
}

/*!
 * Removes labels from blocks that they are used
 * @param index The initial position of the block
 * @param n Size of section
 */
void bucket::set_block_free(std::size_t index, std::size_t n) noexcept {
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
    *(m_ledger + block_index) &= (~((uint8_t)(1) << (index)));
    index++;
    count++;
  }
}

bucket::bucket(std::size_t block_size, std::size_t block_count) : BlockSize{block_size}, BlockCount{block_count} {
  /*! Calculating and allocating the pool size */
  const auto data_size = BlockCount * BlockSize;
  m_data = static_cast<uint8_t *>(std::malloc(data_size));
  assert(m_data != nullptr);
  /*! Allocating memory for markers of used blocks*/
  const auto ledger_size = 1 + ((BlockCount - 1) / 8);
  m_ledger = static_cast<uint8_t *>(std::malloc(ledger_size));
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

  set_block_in_use(index, n);
  return m_data + (index * BlockSize);
}

void bucket::deallocate(void *ptr, std::size_t bytes) noexcept {
  const auto p = static_cast<const uint8_t *>(ptr);
  const std::size_t dist = static_cast<std::size_t>(p - m_data);

  const auto index = dist / BlockSize;

  const auto n = 1 + ((bytes - 1) / BlockSize);

  set_block_free(index, n);
}
