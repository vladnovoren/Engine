#include "logging.hpp"
#ifdef LOGGING
using namespace logging;

Logger::Logger(const char* log_file) {
  log_file_ = log_file;
}

Logger::Logger(const char* filename, const char* funcname, int line, const char* log_file) {
  log_file_ = log_file;
  filename_ = filename;
  funcname_ = funcname;
  line_ = line;
  printf("%p %p %d\n", filename_, funcname_, line);
  printf("%s %s %d\n", filename_, funcname_, line);
  std::ofstream file;
  file.open(log_file_, std::ofstream::out | std::ofstream::app);
  file << "Entering " << funcname_ << "() - (" << filename_ << ":" << line_ << ")" << std::endl;
  file.close();
}

Logger::~Logger() {
  if (filename_ != nullptr && funcname_ != nullptr && line_ != 0) {
    std::ofstream file;
    file.open(log_file_, std::ofstream::out | std::ofstream::app);
    file << "Leaving  " << funcname_ << "() - (" << filename_ << ")" << std::endl;
    file.close();
  }
}

#endif