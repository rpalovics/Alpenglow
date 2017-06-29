class ExperimentEnvironment{
  %TypeHeaderCode
  #include "online_experiment/ExperimentEnvironment.h"
  %End
  public:
    ExperimentEnvironment();
    void set_parameters(OnlineExperimentParameters* params);
    int get_top_k();
    int get_min_time();
    int get_max_time();
    bool is_lookback();

    RecommenderDataIterator* get_recommender_data_iterator();
    Random* get_random();

    bool is_item_new_for_user();
    //vector<int>* get_items();
    //vector<int>* get_users();
    SpMatrix* get_train_matrix();
    PopContainer* get_popularity_container();
    TopPopContainer* get_popularity_sorted_container();

    void update(RecDat* rec_dat);
};
