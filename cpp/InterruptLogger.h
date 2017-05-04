#ifndef MEMORY_RANKING_LOGGER_H
#define MEMORY_RANKING_LOGGER_H

#include "loggers/Logger.h"
#include <vector>
#include <sip.h>

struct PythonKeyboardInterruptException : public exception {};

class InterruptLogger : public Logger {
  public:
    InterruptLogger(){}
    ~InterruptLogger(){}
    void run(RecDat* rec_dat){
      if(PyErr_CheckSignals()!=0){
        if(PyErr_Occurred() && PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)){
          throw PythonKeyboardInterruptException();
        }
      }
    }
};

#endif