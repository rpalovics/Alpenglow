#ifndef ONLINE_PREDICTOR_H
#define ONLINE_PREDICTOR_H
#include <gtest/gtest_prod.h>
#include "Logger.h"
#include "../utils/ToplistCreator.h"
#include "../general_interfaces/Initializable.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"

//SIP_AUTOCONVERT

struct OnlinePredictorParameters {
  double evaluation_start_time = -1;
  double time_frame = -1;
  string file_name = "";
};


class OnlinePredictor
 : public Logger
 , public NeedsExperimentEnvironment
 , public Initializable
 { //SIP_NODEFAULTCTORS
 public:
   OnlinePredictor(OnlinePredictorParameters* params){set_parameters(params);}
   void run(RecDat* rec_dat) override;
   void set_prediction_creator(ToplistCreator* prediction_creator){prediction_creator_ = prediction_creator;}
   bool self_test(){
     bool OK = Logger::self_test();
     if(prediction_creator_==NULL){ OK=false; }
     if(time_frame_<=0){ OK=false; }
     if(!ofs_.is_open()){ OK=false; }
     return OK;
   }
 protected:
   bool autocalled_initialize() override {
     if (evaluation_start_time_<0){
        if (experiment_environment_ == NULL) return false;
        evaluation_start_time_ = experiment_environment_->get_evaluation_start_time();
     }
     return true;
   }
 private:
   void set_parameters(OnlinePredictorParameters* params);
   bool do_predict(RecDat* rec_dat);
   ToplistCreator* prediction_creator_ = NULL;
   double evaluation_start_time_, time_frame_;
   int actual_time_frame_ = 0, past_time_frame_ = 0;
   ofstream ofs_;
   FRIEND_TEST(TestOnlinePredictor, test);
};

#endif /* ONLINE_PREDICTOR_H */
