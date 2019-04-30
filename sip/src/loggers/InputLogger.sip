
struct InputLoggerParameters {
  %TypeHeaderCode
  #include "loggers/InputLogger.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.InputLogger`. See documentation there.
  %End
  std::string output_file;
};
class InputLogger : public Logger, public Initializable  /NoDefaultCtors/{
  %TypeHeaderCode
  #include "loggers/InputLogger.h"
  %End
  public:
    InputLogger(InputLoggerParameters* params);
    void run(RecDat* rec_dat);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

