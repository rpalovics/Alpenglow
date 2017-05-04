%Exception PythonKeyboardInterruptException(SIP_Exception) /PyName=CppKeyboardInterrupt/
{
  %TypeHeaderCode
  #include "cpp/InterruptLogger.h"
  %End
  %RaiseCode
      const char *detail = sipExceptionRef.what();
  %End
};

class InterruptLogger : public Logger {
  %TypeHeaderCode
  #include "cpp/InterruptLogger.h"
  %End
  public:
    InterruptLogger();
    ~InterruptLogger();
    void run(RecDat* rec_dat);
};