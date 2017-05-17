#ifndef ONLINE_PREDICTOR
#define ONLINE_PREDICTOR
#include <gtest/gtest_prod.h>
#include "../loggers/Logger.h"
#include "PredictionCreator.h"

struct OnlinePredictorParameters{
  double min_time, timeFrame;
  string fileName;
};


class OnlinePredictor : public Logger{
 public:
   OnlinePredictor(OnlinePredictorParameters * params){setParameters(params);};
   ~OnlinePredictor(){};
   void run(RecDat * recDat);
   void setParameters(OnlinePredictorParameters* params);
   void setPredictionCreator(PredictionCreator* predictionCreator_){predictionCreator = predictionCreator_;}
   bool selfTest(){
     bool OK = true;
     if(predictionCreator==NULL){ OK=false; }
     return OK;
   }
 private:
   bool doPredict(RecDat * recDat);
   PredictionCreator * predictionCreator;
   double min_time, timeFrame;
   int actualTimeFrame, pastTimeFrame;
   ofstream  ofs;
   FRIEND_TEST(TestOnlinePredictor, test);
};

#endif
