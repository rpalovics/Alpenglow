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
class InputLogger : public Logger, public Initializable { //SIP_NODEFAULTCTORS
  public:
    InputLogger(InputLoggerParameters* params){
      output_file_name_ = params->output_file;
    }
    void run(RecDat* rec_dat) override {
      output_file_ << *rec_dat << endl;
    }
  protected:
    bool autocalled_initialize() override {
      output_file_.open(output_file_name_.c_str());
      return true;
    }
  private:
    string output_file_name_;
    ofstream output_file_; 
};

#endif
