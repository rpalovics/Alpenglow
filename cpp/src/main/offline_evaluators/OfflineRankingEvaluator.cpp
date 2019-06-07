#include "OfflineRankingEvaluator.h"
void OfflineRankingEvaluator::evaluate(){
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
