/*
replacement for TMW's logging
*/

class Logger
{

 public:

  Logger();

  ~Logger();

  /**
   * Enters a message in the log. The message will be timestamped.
   */
  void log(const char *log_text, ...);

  /**
   * Log an error and quit. The error will pop-up on Windows and Mac, and
   * will be printed to standard error everywhere else.
   */
  void error(const std::string &error_text);

};

extern Logger *logger;
