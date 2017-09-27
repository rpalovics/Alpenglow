#include "OfflineUserToplistEvaluator.h"
void OfflineUserToplistEvaluator::evaluate(){
  LegacyRecommenderData test_data;
  test_data.read_from_file(test_file_name_, test_file_type_);
  vector<USER>* users = test_data.get_all_users();
  ofstream ofs(output_file_name_.c_str());
  RecDat rec_dat;
  for(auto user : *users){
    rec_dat.user=user;
    vector<RecDat>* toplist = toplist_creator_->run(&rec_dat);
    ofs << user << " ";
    for(auto item_pred: *toplist){
      ofs << item_pred.item << "," << item_pred.score << " "; //TODO inkabb a toplistfile modell stilusaban kene kiirni
    }
    ofs << endl;
  }
}
