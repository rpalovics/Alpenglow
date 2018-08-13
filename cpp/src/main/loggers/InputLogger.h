#ifndef INPUT_LOGGER
#define INPUT_LOGGER

//SIP_AUTOCONVERT

#include <iostream>
#include <string>
#include "../general_interfaces/Initializable.h"
#include "Logger.h"
using namespace std;

struct InputLoggerParameters {
  string output_file = "";
};
class InputLogger : public Logger, public Initializable {
  public:
    InputLogger(InputLoggerParameters* params){
      output_file = params->output_file;
    }
    void run(RecDat* rec_dat) override {
      //TODO write into file
    }
  protected:
    bool autocalled_initialize() override {
      //TODO open file here
      return true;
    }
  private:
    string output_file;
};

#endif
