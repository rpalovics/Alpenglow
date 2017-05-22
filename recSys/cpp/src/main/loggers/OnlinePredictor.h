#ifndef ONLINE_PREDICTOR
#define ONLINE_PREDICTOR
#include <gtest/gtest_prod.h>
#include "Logger.h"
#include "utils/PredictionCreator.h"

struct OnlinePredictorParameters{
  double min_time, time_frame;
  string file_name;
};


class OnlinePredictor : public Logger{
 public:
   OnlinePredictor(OnlinePredictorParameters* params){set_parameters(params);};
   ~OnlinePredictor(){};
   void run(RecDat* rec_dat);
   void set_parameters(OnlinePredictorParameters* params);
   void set_prediction_creator(PredictionCreator* prediction_creator){prediction_creator_ = prediction_creator;}
   bool self_test(){
     bool OK = true;
     if(prediction_creator_==NULL){ OK=false; }
     return OK;
   }
 private:
   bool do_predict(RecDat* rec_dat);
   PredictionCreator* prediction_creator_;
   double min_time_, time_frame_;
   int actual_time_frame_, past_time_frame_;
   ofstream ofs_;
   FRIEND_TEST(TestOnlinePredictor, test);
};

#endif
