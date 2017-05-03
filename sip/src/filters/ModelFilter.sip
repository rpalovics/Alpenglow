class ModelFilter {
  %TypeHeaderCode
  #include "filters/ModelFilter.h"
  %End
  public:
    virtual void run(RecDat* rec_dat);
    // virtual vector<pair<int,double>>* get_global_items();
    // virtual vector<pair<int,double>>* get_global_users();
    // virtual vector<pair<int,double>>* get_personalized_items(int user);
    // virtual vector<pair<int,double>>* get_personalized_users(int item);
    virtual bool active(RecDat*);
    virtual ~ModelFilter();
    bool self_test();
};