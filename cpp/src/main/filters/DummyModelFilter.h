#ifndef DUMMY_MODEL_FILTER_H
#define DUMMY_MODEL_FILTER_H

//SIP_AUTOCONVERT

#include "ModelFilter.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

///Assuption: vectors users_ and items_ never get shorter
class DummyModelFilter : public ModelFilter, public NeedsExperimentEnvironment, public Initializable {
  public:
    void run(RecDat* rec_dat) override;
    vector<pair<int,double>>* get_global_users() override;
    vector<pair<int,double>>* get_global_items() override;
    void set_users(vector<int>* users){ users_ = users; }
    /* SIP_CODE
    void set_users(VectorInt);
    %MethodCode
        sipCpp->set_users(&(a0->vec));
    %End
    */
    void set_items(vector<int>* items){ items_ = items; }
    /* SIP_CODE
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    */
    bool self_test(){
      bool OK = ModelFilter::self_test();
      if(items_==NULL){ OK=false; cerr << "DummyModelFilter::items_ is not set." << endl; }
      if(users_==NULL){ OK=false; cerr << "DummyModelFilter::users_ is not set." << endl; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if (items_==NULL) {
        if (experiment_environment_==NULL) return false;
        items_=experiment_environment_->get_items();
      }
      if (users_==NULL) {
        if (experiment_environment_==NULL) return false;
        users_=experiment_environment_->get_users();
      }
      return true;
    }
  private:
    const vector<int>* users_ = NULL;
    const vector<int>* items_ = NULL;
    int last_users_size_ = -1;
    int last_items_size_ = -1;
    vector<pair<int,double>> user_filter_;
    vector<pair<int,double>> item_filter_;
};



#endif /* DUMMY_MODEL_FILTER_H */
