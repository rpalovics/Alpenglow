#ifndef DUMMY_MODEL_FILTER
#define DUMMY_MODEL_FILTER

#include "ModelFilter.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"

///Assuption: vectors users_ and items_ never get shorter
class DummyModelFilter : ModelFilter, INeedExperimentEnvironment {
  public:
    DummyModelFilter(){
      last_users_size_ = -1;
      last_items_size_ = -1;
      items_ = NULL;
      users_ = NULL;
    }
    virtual void run(RecDat* rec_dat) override;
    virtual vector<pair<int,double> >* get_global_users() override;
    virtual vector<pair<int,double> >* get_global_items() override;
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; }
    void set_users(vector<int>* users){ users_ = users; }
    void set_items(vector<int>* items){ items_ = items; }
    void init(){
      if(items_==NULL) items_=experiment_environment_->get_items();
      if(users_==NULL) users_=experiment_environment_->get_users();
    }
    bool self_test(){
      bool OK = ModelFilter::self_test();
      if(items_==NULL){ OK=false; cerr << "DummyModelFilter::items_ is not set." << endl; }
      if(users_==NULL){ OK=false; cerr << "DummyModelFilter::users_ is not set." << endl; }
      return OK;
    }
  private:
    ExperimentEnvironment* experiment_environment_;
    vector<int>* users_;
    vector<int>* items_;
    int last_users_size_;
    int last_items_size_;
    vector<pair<int,double>> user_filter_;
    vector<pair<int,double>> item_filter_;
};



#endif
