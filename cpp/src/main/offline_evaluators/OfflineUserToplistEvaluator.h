#ifndef USER_TOPLIST_EVALUATOR
#define USER_TOPLIST_EVALUATOR
#include <fstream>
#include "OfflineEvaluator.h"
#include "../recommender_data/macros.h"
#include "../recommender_data/RecommenderData.h"
#include "../utils/PredictionCreator.h"

using namespace std;
struct OfflineUserToplistEvaluatorParameters{ //TODO jinjactor
  string test_file_name;
  string test_file_type;
  string output_file_name;
};
class OfflineUserToplistEvaluator : public OfflineEvaluator{
  public:
    OfflineUserToplistEvaluator(OfflineUserToplistEvaluatorParameters* params){
      test_file_name_ = params->test_file_name;
      test_file_type_ = params->test_file_type;
      output_file_name_ = params->output_file_name;
      toplist_creator_ = NULL;
    }
    virtual void evaluate();
    void set_toplist_creator(PredictionCreatorPersonalized* toplist_creator){ toplist_creator_ = toplist_creator; }
    bool self_test(){
      bool OK = OfflineEvaluator::self_test();
      if(toplist_creator_ == NULL){
        OK=false; cerr << "OfflineUserToplistEvaluator::toplist_creator_ is not set." << endl;
      }
      return OK;
    }
  private:
    string test_file_name_;
    string test_file_type_;
    string output_file_name_;
    PredictionCreatorPersonalized* toplist_creator_;
};


#endif
