#include <iostream>
#include <cstdarg>
#include <cstdio>

#include "log.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::log(const char *log_text, ...)
{
  char* buf = new char[1024];
  va_list ap;

  // Use a temporary buffer to fill in the variables
  va_start(ap, log_text);
  vsprintf(buf, log_text, ap);
  va_end(ap);

  std::cout << "[LOG] " << buf << std::endl;
}

void Logger::error(const std::string &error_text)
{
  std::cout << "[ERR] " << error_text << std::endl;
}
