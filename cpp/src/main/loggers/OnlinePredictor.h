#ifndef ONLINE_PREDICTOR_H
#define ONLINE_PREDICTOR_H
#include <gtest/gtest_prod.h>
#include "Logger.h"
#include "../utils/PredictionCreator.h"
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
   OnlinePredictor(OnlinePredictorParameters* params){set_parameters(params);};
   void run(RecDat* rec_dat) override;
   void set_prediction_creator(PredictionCreator* prediction_creator){prediction_creator_ = prediction_creator;}
   bool self_test(){
     bool OK = true;
     if(prediction_creator_==NULL){ OK=false; }
     return OK;
   }
 protected:
   bool autocalled_initialize(){
     if (evaluation_start_time_<0){
        if (experiment_environment_ == NULL) return false;
        evaluation_start_time_ = experiment_environment_->get_evaluation_start_time();
     }
     return true;
   }
 private:
   void set_parameters(OnlinePredictorParameters* params);
   bool do_predict(RecDat* rec_dat);
   PredictionCreator* prediction_creator_;
   double evaluation_start_time_, time_frame_;
   int actual_time_frame_, past_time_frame_;
   ofstream ofs_;
   FRIEND_TEST(TestOnlinePredictor, test);
};

#endif /* ONLINE_PREDICTOR_H */
