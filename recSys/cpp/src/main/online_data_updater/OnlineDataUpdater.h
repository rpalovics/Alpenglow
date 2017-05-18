#ifndef ONLINE_DATA_UPDATER
#define ONLINE_DATA_UPDATER

#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>

#include "Updater.h"
#include "../recommender_data/RecommenderData.h"
#include "../utils/PopContainers.h"

using namespace std;

struct OnlineDataUpdaterParameters{
    SpMatrix * train_matrix;
    vector <int> * items;
    vector <int> * users;
    TopPopContainer * pop; 
    PopContainer* popContainer;
};

class OnlineDataUpdater : public Updater{
  public:
    OnlineDataUpdater(OnlineDataUpdaterParameters * parameters){
      set_parameters(parameters);
    }
    void set_parameters(OnlineDataUpdaterParameters * parameters);
    OnlineDataUpdater(){
      train_matrix_ = NULL;
      popularity_sorted_container_ = NULL;
      popularity_container_ = NULL;
      items_ = NULL;
      users_ = NULL;
    }
    void set_train_matrix(SpMatrix* train_matrix){train_matrix_ = train_matrix; }
    void set_popularity_sorted_container(TopPopContainer* popularity_sorted_container){ popularity_sorted_container_ = popularity_sorted_container; }
    void set_popularity_container(PopContainer* popularity_container){ popularity_container_ = popularity_container; }
    void set_items(vector<int>* items){ items_ = items; }
    void set_users(vector<int>* users){ users_ = users; }
    ~OnlineDataUpdater(){};
    void update(RecDat* rec_dat);
  protected: 
    SpMatrix* train_matrix_;
    TopPopContainer* popularity_sorted_container_;
    PopContainer* popularity_container_;
    vector<int>* items_;
    vector<int> item_map_;
    vector<int>* users_;
    vector<int> user_map_;
};


#endif
