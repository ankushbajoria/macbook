#pragma once

#include <iostream>
#include <pthread.h>
#include <string>

/**
 * author : abajoria
 * date : 20140621
 * description : synchonize write to stdout across multiple threads;
 **/

class StdoutLogger {
  private:
    pthread_mutex_t m_lock;

  public:
    StdoutLogger() {
      pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
      m_lock = lock;
    }

    void write (std::string str) {
      pthread_mutex_lock(&m_lock);
      time_t ltime = time(NULL);
      struct tm *Tm = localtime(&ltime);
      printf("%d:%d:%02d ", Tm->tm_hour, Tm->tm_min, Tm->tm_sec);
      std::cout << str;
      pthread_mutex_unlock(&m_lock);
    }
};

class StdoutLoggerSingleton {
  static StdoutLogger* s_logger;

  public:
    static StdoutLogger* getStdoutLogger() {
      return s_logger;
    }
};
