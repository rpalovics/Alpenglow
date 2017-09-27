#ifndef OFFLINE_RANKING_EVALUATOR
#define OFFLINE_RANKING_EVALUATOR
#include <fstream>
#include "../offline_evaluators/OfflineEvaluator.h"
#include "../recommender_data/macros.h"
#include "../recommender_data/RecommenderData.h"
#include "../utils/PredictionCreator.h"

using namespace std;
struct OfflineRankingEvaluatorParameters{
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
      toplist_creator_ = NULL;
    }
    virtual void evaluate(){
      LegacyRecommenderData test_data;
      test_data.read_from_file(test_file_name_, test_file_type_);
      vector<USER>* users = test_data.get_all_users();
      cerr << "Number of test users: " << users->size() << endl;
      int user_counter = 0;
      for(auto user : *users){
        user_counter++;
        if(user_counter%500==0) cerr << "Processed " << user_counter << "users." << endl;
        RecDat rec_dat;
        rec_dat.user=user;
        vector<RecDat>* toplist = toplist_creator_->run(&rec_dat);
        EXPAND_VECTORMAP(user_toplist_maps_,user);
        RANK rank = 0;
        for(auto item_pred: *toplist){
          user_toplist_maps_[user].insert(make_pair(item_pred.item,make_pair(item_pred.score,rank)));
          rank++;
        }
      }
      cerr << "End of computing toplists." << endl;
      ofstream ofs(output_file_name_.c_str());
      for(int i=0;i<test_data.size();i++){
        RecDat* rec_dat = test_data.get(i);
        USER user = rec_dat->user;
        ITEM item = rec_dat->item;
        SCORE prediction;
        RANK rank;
        auto prediction_rank = user_toplist_maps_[user].find(item);
        if(prediction_rank!=user_toplist_maps_[user].end()){
          prediction = prediction_rank->second.first;
          rank = prediction_rank->second.second;
        } else {
          prediction = 0;
          rank = top_k_+2;
        }
        ofs << rec_dat->time << " " << rec_dat->user << " " << rec_dat->item << " " << rank << " " << prediction << " " << rec_dat->id << endl;

      }
    }
    void set_toplist_creator(PredictionCreatorPersonalized* toplist_creator){ toplist_creator_ = toplist_creator; }
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
    PredictionCreatorPersonalized* toplist_creator_;
    vector<map<ITEM,pair<SCORE,RANK>>> user_toplist_maps_;
};


#endif
