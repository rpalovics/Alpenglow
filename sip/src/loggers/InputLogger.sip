
struct InputLoggerParameters {
  %TypeHeaderCode
  #include "loggers/InputLogger.h"
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

