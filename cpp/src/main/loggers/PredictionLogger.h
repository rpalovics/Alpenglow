#ifndef PREDICTION_LOGGER
#define PREDICTION_LOGGER
#include <gtest/gtest_prod.h>
#include "Logger.h"
#include "../utils/PredictionCreator.h"

//SIP_OVERWRITE

struct OnlinePredictions{
  vector<int> ids;
  vector<int> times;
  vector<int> users;
  vector<int> items;
  vector<int> ranks;
  vector<double> scores;
};

class PredictionLogger : public Logger{ //SIP_NODEFAULTCTORS
  public:
    PredictionLogger(){}
    ~PredictionLogger(){}
    void run(RecDat * recDat);
    void set_prediction_creator(PredictionCreator* prediction_creator_){prediction_creator = prediction_creator_;}
    bool self_test(){
      bool ok = true;
      if(prediction_creator == NULL){
        cerr << "PredictionLogger: prediction_creator not set!";
        ok = false;
      }
      return ok;
    }
    OnlinePredictions* get_predictions(){
      return &predictions_;
    }
  private:
    OnlinePredictions predictions_;
    PredictionCreator * prediction_creator=NULL;
    ofstream  ofs;
    FRIEND_TEST(TestPredictionLogger, test);
};

#endif
