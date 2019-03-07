#ifndef COOCCURRENCE_COMPUTER_H
#define COOCCURRENCE_COMPUTER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include "../recommender_data/RecommenderData.h"
#include "../recommender_data/macros.h"

#define WEIGHT int
using namespace std;

class CooccurrenceComputer{
public:
  void set_parameters(int max_length, int slice_size); //max_length; slice_size
  void run(RecommenderData& recommender_data, vector<set<ITEM>>& users_items, vector<map<ITEM,WEIGHT>>& edge_weights, ostream& output_file);
  int compute_itemset_for_users(RecommenderData& recommender_data, vector<set<ITEM>>& users_items);
  void compute_edge_weights(vector<set<ITEM>>& users_items, vector<map<ITEM,WEIGHT>>& edge_weights, int slice_index);
  void write_toplists(vector<map<ITEM,WEIGHT>>& edge_weights, ostream& output_file, int slice_index);
private:
  int max_length_;
  int slice_size_;
};


#endif /* COOCCURRENCE_COMPUTER_H */
