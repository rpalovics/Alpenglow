#include "TransitionModelLogger.h"
void TransitionModelLogger::write_eval_statistics(RecDat* rec_dat){ 
  if(rec_dat->eval==0) return;
  int user = rec_dat->user;
  int item = rec_dat->item;
  auto actual_frequency_map = model_->get_frequency_map(user);
  vector<pair<int,int>> filtered_sorted_frequency_list = compute_toplist(actual_frequency_map, rec_dat);
  int toplist_length = filtered_sorted_frequency_list.size();
  int from_pop = accumulate(filtered_sorted_frequency_list.begin(),filtered_sorted_frequency_list.end(),0.0,[](double a, pair<int,int> b){ return a+b.second; });
  int to_pop = pop_container_->get(item);
  double actual_prediction = model_->prediction(rec_dat);
  int first_score = (filtered_sorted_frequency_list.size()==0?0:filtered_sorted_frequency_list[0].second); //TODO ez most freq, nem score
  int second_score = (filtered_sorted_frequency_list.size()<2?0:filtered_sorted_frequency_list[1].second); //TODO ez most freq, nem score
  int last_score = (filtered_sorted_frequency_list.size()<101?0:filtered_sorted_frequency_list[99].second); //TODO top_k szerint kene, 100 bele van egetve
  timeline_file_ << (int)rec_dat->time << " " << rec_dat->eval << " " << rec_dat->id << " " << item << " " << toplist_length << " " << from_pop << " " << to_pop << " " << actual_prediction << " " << first_score << " " << second_score << " " << last_score << endl;
}
vector<pair<int,int>> TransitionModelLogger::compute_toplist(map<int,int>* actual_frequency_map, RecDat* rec_dat){
  vector<pair<int,int>> toplist;
  if (actual_frequency_map==NULL) return toplist; //empty list
  int user = rec_dat->user;
  for (auto& frequency : *actual_frequency_map){
    if(train_matrix_->get(user,frequency.first)<1){
      toplist.push_back(frequency);
    }
  }
  sort(
      toplist.begin(),
      toplist.end(),
      [](pair<int,int> a, pair<int,int> b) -> bool
         { return (a.second) > (b.second); }
      );
  return toplist;
}
void TransitionModelLogger::write_toplist_lengths(RecDat* rec_dat){
  string filename=toplist_length_logfile_basename_+"_"+to_string(last_period_num_);
  ofstream ofs(filename.c_str());
  if(!ofs.is_open()) cerr << "Error: TransitionModelLogger can't open file '" << filename << "'." << endl;
  for(uint item=0;item<model_->transition_frequencies_.size();item++){
    int out_degree = model_->transition_frequencies_[item].size();
    //int popularity = pop_container_->get(item);
    ofs << item << " " << out_degree;// << " " << popularity;
    ofs << endl;
  }
}
