#include "TransitionModelEndLogger.h"
void TransitionModelEndLogger::run(RecDat* rec_dat){
  ofstream ofs(log_file_name_.c_str());
  if(!ofs.is_open()){
    cerr << "Error in TransitionModelEndLogger: can't open file '"
      << log_file_name_ << "'." << endl;
  }
  run_core(ofs);
}
void TransitionModelEndLogger::run_core(ostream& ofs){
  //vector<map<int,int>> transition_frequencies_;
  for(uint item=0;item<model_->transition_frequencies_.size();item++){
    int out_degree = model_->transition_frequencies_[item].size();
    int popularity = pop_container_->get(item);
    ofs << item << " " << out_degree << " " << popularity;
    vector<pair<int,int>> to_items;
    for(auto to_item = model_->transition_frequencies_[item].begin();to_item!=model_->transition_frequencies_[item].end();to_item++){
      to_items.push_back(make_pair(to_item->first,to_item->second));
    }
    sort(
        to_items.begin(),
        to_items.end(),
        [](pair<int,int> a, pair<int,int> b) -> bool
           { return (a.second) > (b.second); }
        );
    for(int index=0;index<(int)to_items.size() and index<max_length_;index++){
      ofs << " " << to_items[index].first << "," << to_items[index].second;
    }
    ofs << endl;
  }
}
