#ifndef INPUT_LOGGER_H
#define INPUT_LOGGER_H

//SIP_AUTOCONVERT

#include <iostream>
#include <string>
#include <gtest/gtest_prod.h>
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
      *output_stream_ << (int) rec_dat->time << " " << rec_dat->user << " " << rec_dat->item << " " << rec_dat->id << " " << rec_dat->score << endl;
    }
    bool self_test(){
      bool ok = Logger::self_test();
      if (output_stream_==NULL) ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      output_file_.open(output_file_name_.c_str());
      output_stream_ = &output_file_;
      return true;
    }
  private:
    string output_file_name_;
    ofstream output_file_; 
    ostream* output_stream_ = NULL;
  FRIEND_TEST(TestInputLogger, test);
};

#endif /* INPUT_LOGGER_H */
