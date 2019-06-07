#ifndef PREDICTION_LOGGER_H
#define PREDICTION_LOGGER_H
#include <gtest/gtest_prod.h>
#include "Logger.h"
#include "../utils/ToplistCreator.h"

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
    void run(RecDat* recDat);
    void set_prediction_creator(ToplistCreator* prediction_creator_){prediction_creator = prediction_creator_;}
    bool self_test(){
      bool ok = Logger::self_test();
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
    ToplistCreator* prediction_creator=NULL;
    FRIEND_TEST(TestPredictionLogger, test);
};

#endif /* PREDICTION_LOGGER_H */
