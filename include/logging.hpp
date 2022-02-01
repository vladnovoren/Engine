#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <string>
#include <fstream>
#include <iostream>

// #define LOGGING

#ifdef LOGGING
  namespace logging {
    class Logger {
      public:
        Logger(const char* log_file = "log.txt");
    	  Logger(const char* filename, const char* funcname, int line, const char* log_file = "log.txt");
    	  ~Logger();

        template<class T>
        void WriteToLog(T message) {
          std::ofstream file;
          file.open("log.txt", std::ofstream::out | std::ofstream::app);
          file << message << std::endl;
          file.close();
        }

      private:
        const char* log_file_ = nullptr;
      	const char* filename_ = nullptr;
      	const char* funcname_ = nullptr;
        int line_ = -1;
    };
  }
  #define INIT_LOGGER(path)       logging::Logger LOGGER{path}
  #define INIT_LOGGER_TRACE(path) logging::Logger LOGGER{__FILE__, __FUNCTION__, __LINE__, path}
  #define LOGGER_MESSAGE(message) LOGGER.WriteToLog(message)
#else
  #define INIT_LOGGER(path)
  #define INIT_LOGGER_TRACE(path)
  #define LOGGER_MESSAGE(message)
#endif

#endif