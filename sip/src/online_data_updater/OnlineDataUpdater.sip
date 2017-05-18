struct OnlineDataUpdaterParameters{
  %TypeHeaderCode
  #include "online_data_updater/OnlineDataUpdater.h"
  %End
  SpMatrix * train_matrix;
  std::vector <int> * items;
  std::vector <int> * users;
  TopPopContainer * pop; 
  PopContainer* pop_container;
};

class OnlineDataUpdater : public Updater{
  %TypeHeaderCode
  #include "online_data_updater/OnlineDataUpdater.h"
  %End
  public:
    OnlineDataUpdater(OnlineDataUpdaterParameters * parameters);
    void set_parameters(OnlineDataUpdaterParameters * parameters);
    OnlineDataUpdater();
    void set_train_matrix(SpMatrix* train_matrix);
    void set_popularity_sorted_container(TopPopContainer* popularity_sorted_container);
    void set_popularity_container(PopContainer* popularity_container);
    void set_items(std::vector<int>* items);
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    void set_users(std::vector<int>* users);
    void set_users(VectorInt);
    %MethodCode
        sipCpp->set_users(&(a0->vec));
    %End
    ~OnlineDataUpdater();
    void update(RecDat* rec_dat);
};