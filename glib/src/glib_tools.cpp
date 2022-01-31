#include "glib_tools.hpp"


size_t RoundUpDiv(const size_t divident, const size_t divider) {
  return (divident + divider - 1) / divider;
}
