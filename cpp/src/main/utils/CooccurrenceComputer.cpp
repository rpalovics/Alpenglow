#include "CooccurrenceComputer.h"

void CooccurrenceComputer::set_parameters(int max_length, int slice_size){
  max_length_ = max_length;
  slice_size_ = slice_size;
}
void CooccurrenceComputer::run(RecommenderData& recommender_data, vector<set<ITEM>>& users_items, vector<map<ITEM,WEIGHT>>& edge_weights, ostream& output_file){
  int maxitem = compute_itemset_for_users(recommender_data,users_items);
  int slice_index = 0;
  while (slice_size_*slice_index<=maxitem) {
    compute_edge_weights(users_items,edge_weights,slice_index);
    write_toplists(edge_weights,output_file,slice_index);
    cerr << "Finished slice " << slice_index << ", largest item<" << (slice_index+1)*slice_size_ << endl;
    edge_weights.clear();
    slice_index++;
  }
}
int CooccurrenceComputer::compute_itemset_for_users(RecommenderData& recommender_data, vector<set<ITEM>>& users_items){
  int maxitem = 0;
  for(int i=0;i<recommender_data.size();i++){
    if(i%500000==0) cerr << i << " samples processed." << endl;
    RecDat* rec_dat = recommender_data.get(i);
    USER user = rec_dat->user;
    ITEM item = rec_dat->item;
    if(maxitem<item) maxitem = item;
    EXPAND_VECTORMAP(users_items,user);
    users_items[user].insert(item);
  }
  return maxitem;
}
void CooccurrenceComputer::compute_edge_weights(vector<set<ITEM>>& users_items, vector<map<ITEM,WEIGHT>>& edge_weights, int slice_index){
  //OPTA start
  vector<vector<int>> users_items_transformed;
  users_items_transformed.resize(users_items.size());
  for(uint user=0;user<users_items.size();user++){
    users_items_transformed[user].reserve(users_items[user].size());
    for(auto item1:users_items[user]){
      users_items_transformed[user].push_back(item1);
    }
  }
  //OPTA end
  int min_item_bound = slice_size_*slice_index; //e.g. min_item_bound==4 -> smallest allowed item is 4
  int max_item_bound = slice_size_*(slice_index+1); //e.g. max_item_bound==8 -> largest allowed item is 7
  edge_weights.resize(slice_size_); //OPTA
  for(uint user=0;user<users_items_transformed.size();user++){ //OPTM
    if(user%1000==0) cerr << user << " users processed." << endl;
    for(auto item1:users_items_transformed[user]){ //OPTM
      if (item1<min_item_bound or item1>=max_item_bound) continue;
      int offseted_item1=item1-slice_size_*slice_index;
      //EXPAND_VECTORMAP(edge_weights,offseted_item1); //OPTR
      for(auto item2:users_items_transformed[user]){ //OPTM
        if (item1==item2) continue; //diagonal is left 0
        edge_weights[offseted_item1][item2]; //inserts element to map if necessary
        edge_weights[offseted_item1][item2]++;
      }
    }
  }
}
void CooccurrenceComputer::write_toplists(vector<map<ITEM,WEIGHT>>& edge_weights, ostream& ofs, int slice_index){
  int offset = slice_size_*slice_index;
  for(uint item=0;item<edge_weights.size();item++){
    if(item%200000==0) cerr << item << " items processed." << endl;
    int out_degree = edge_weights[item].size();
    int popularity = -1;
    ofs << offset+item << " " << out_degree << " " << popularity;
    vector<pair<ITEM,WEIGHT>> to_items;
    for(auto to_item = edge_weights[item].begin();to_item!=edge_weights[item].end();to_item++){
      to_items.push_back(make_pair(to_item->first,to_item->second));
    }
    sort(
        to_items.begin(),
        to_items.end(),
        [](pair<ITEM,WEIGHT> a, pair<ITEM,WEIGHT> b) -> bool
           { return (a.second) > (b.second); }
        );
    for(int index=0;(uint)index<to_items.size() and index<max_length_;index++){
      ofs << " " << to_items[index].first << "," << to_items[index].second;
    }
    ofs << endl;
  }
}

