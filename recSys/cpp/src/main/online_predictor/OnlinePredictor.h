#ifndef ONLINE_PREDICTOR
#define ONLINE_PREDICTOR
#include <gtest/gtest_prod.h>
#include "../loggers/Logger.h"
#include "PredictionCreator.h"

struct OnlinePredictorParameters{
  double min_time, time_frame;
  string file_name;
};


class OnlinePredictor : public Logger{
 public:
   OnlinePredictor(OnlinePredictorParameters * params){set_parameters(params);};
   ~OnlinePredictor(){};
   void run(RecDat * rec_dat);
   void set_parameters(OnlinePredictorParameters* params);
   void set_prediction_creator(PredictionCreator* prediction_creator_){prediction_creator = prediction_creator_;}
   bool self_test(){
     bool OK = true;
     if(prediction_creator==NULL){ OK=false; }
     return OK;
   }
 private:
   bool do_predict(RecDat * rec_dat);
   PredictionCreator * prediction_creator;
   double min_time, time_frame;
   int actual_time_frame, past_time_frame;
   ofstream  ofs;
   FRIEND_TEST(TestOnlinePredictor, test);
};

#endif
