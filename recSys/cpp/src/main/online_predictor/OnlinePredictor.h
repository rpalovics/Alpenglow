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
   void run(RecDat * recDat);
   void set_parameters(OnlinePredictorParameters* params);
   void set_prediction_creator(PredictionCreator* predictionCreator_){predictionCreator = predictionCreator_;}
   bool self_test(){
     bool OK = true;
     if(predictionCreator==NULL){ OK=false; }
     return OK;
   }
 private:
   bool doPredict(RecDat * recDat);
   PredictionCreator * predictionCreator;
   double min_time, time_frame;
   int actualTimeFrame, pastTimeFrame;
   ofstream  ofs;
   FRIEND_TEST(TestOnlinePredictor, test);
};

#endif
