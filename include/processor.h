#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
  LinuxParser::CpuStats prev_stat_{0, 0};
  LinuxParser::CpuStats cur_stat_{0, 0};
};

#endif
