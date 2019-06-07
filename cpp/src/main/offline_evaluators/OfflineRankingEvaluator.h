#ifndef OFFLINE_RANKING_EVALUATOR_H
#define OFFLINE_RANKING_EVALUATOR_H
#include <fstream>
#include "../offline_evaluators/OfflineEvaluator.h"
#include "../recommender_data/macros.h"
#include "../recommender_data/RecommenderData.h"
#include "../utils/ToplistCreator.h"

using namespace std;
struct OfflineRankingEvaluatorParameters {
  string test_file_name;
  string test_file_type;
  string output_file_name;
  int top_k;
};
class OfflineRankingEvaluator : public OfflineEvaluator{
  public:
    OfflineRankingEvaluator(OfflineRankingEvaluatorParameters* params){
      test_file_name_ = params->test_file_name;
      test_file_type_ = params->test_file_type;
      output_file_name_ = params->output_file_name;
      top_k_ = params->top_k;
    }
    virtual void evaluate();
    void set_toplist_creator(ToplistCreatorPersonalized* toplist_creator){
      toplist_creator_ = toplist_creator;
    }
    bool self_test(){
      bool OK = OfflineEvaluator::self_test();
      if(toplist_creator_ == NULL){
        OK=false; cerr << "OfflineRankingEvaluator::toplist_creator_ is not set." << endl;
      }
      return OK;
    }
  private:
    string test_file_name_;
    string test_file_type_;
    string output_file_name_;
    int top_k_;
    ToplistCreatorPersonalized* toplist_creator_ = NULL;
    vector<map<ITEM,pair<SCORE,RANK>>> user_toplist_maps_;
};


#endif /* OFFLINE_RANKING_EVALUATOR_H */
