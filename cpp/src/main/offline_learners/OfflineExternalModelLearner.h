#ifndef OFFLINE_EXTERNAL_MODEL_LEARNER_H
#define OFFLINE_EXTERNAL_MODEL_LEARNER_H

#include "OfflineLearner.h"
#include "../models/ExternalModel.h"
#include <stdexcept>
#include <gtest/gtest_prod.h>

using namespace std;

struct OfflineExternalModelLearnerParameters {
  string mode;
  string out_name_base;
  string in_name_base;
};

class OfflineExternalModelLearner : public OfflineLearner{
  public:
    OfflineExternalModelLearner(OfflineExternalModelLearnerParameters* parameters)
    : out_name_base_(parameters->out_name_base), in_name_base_(parameters->in_name_base), mode_(parameters->mode){}
    ~OfflineExternalModelLearner(){};
    void fit(RecommenderData* recommender_data);
    void set_model(ExternalModel* model){
      model_=model;
    }
  private:
    const string out_name_base_;
    const string in_name_base_;
    const string mode_;
    ExternalModel* model_;
    int call_number_=1;
};

#endif /* OFFLINE_EXTERNAL_MODEL_LEARNER_H */
