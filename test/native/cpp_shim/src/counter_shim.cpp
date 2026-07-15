#include "counter_shim.h"

#include <cstdint>
#include <limits>
#include <new>

class Counter final {
public:
  explicit Counter(std::int32_t initial_value) noexcept : value_(initial_value) {}

  bool add(std::int32_t amount) noexcept {
    const std::int64_t sum = static_cast<std::int64_t>(value_)
      + static_cast<std::int64_t>(amount);
    if (sum < std::numeric_limits<std::int32_t>::min()
        || sum > std::numeric_limits<std::int32_t>::max()) {
      return false;
    }
    value_ = static_cast<std::int32_t>(sum);
    return true;
  }

  std::int32_t value() const noexcept {
    return value_;
  }

private:
  std::int32_t value_;
};

struct hxc_fixture_counter {
  explicit hxc_fixture_counter(std::int32_t initial_value) noexcept
    : value(initial_value) {}

  Counter value;
};

extern "C" {

hxc_fixture_counter *hxc_fixture_counter_create(int32_t initial_value) {
  return new (std::nothrow) hxc_fixture_counter(initial_value);
}

bool hxc_fixture_counter_add(hxc_fixture_counter *counter, int32_t amount) {
  return counter != nullptr && counter->value.add(amount);
}

bool hxc_fixture_counter_value(
  const hxc_fixture_counter *counter,
  int32_t *out_value
) {
  if (counter == nullptr || out_value == nullptr) {
    return false;
  }
  *out_value = counter->value.value();
  return true;
}

void hxc_fixture_counter_destroy(hxc_fixture_counter *counter) {
  delete counter;
}

} /* extern "C" */
