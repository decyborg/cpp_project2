#include "processor.h"

// Done: Return the aggregate CPU utilization
float Processor::Utilization() {
  // Store Previous stats
  prev_stat_ = cur_stat_;
  // Get new stats
  cur_stat_ = LinuxParser::CpuUtilization();

  // Calculate utilization
  float prev_total = prev_stat_.idle + prev_stat_.nonidle;
  float cur_total = cur_stat_.idle + cur_stat_.nonidle;

  float totald = cur_total - prev_total;
  float idled = cur_stat_.idle - prev_stat_.idle;

  return (totald - idled) / totald;
}
