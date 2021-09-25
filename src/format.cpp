#include "format.h"

#include <string>

using std::string;

constexpr int sec_in_hr{3600};
constexpr int sec_in_min{60};

string two_dig_str(int val) {
  string tmp;
  if (val > 9) {
    tmp = std::to_string(val);
  } else {
    tmp = "0" + std::to_string(val);
  }

  return tmp;
}

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  // Calculate hours/minutes
  int hours = seconds / sec_in_hr;
  seconds = seconds % sec_in_hr;
  int min = seconds / sec_in_min;
  seconds = seconds % sec_in_min;

  // Convert to string
  string time_up = two_dig_str(hours) + ":";
  time_up += two_dig_str(min) + ":";
  time_up += two_dig_str(seconds);
  return time_up;
}
