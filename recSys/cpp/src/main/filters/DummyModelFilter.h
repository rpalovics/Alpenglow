#ifndef DUMMY_MODEL_FILTER
#define DUMMY_MODEL_FILTER

#include "ModelFilter.h"

//feltetelezi, hogy a users es items vektorok csak hizhatnak
class DummyModelFilter : public ModelFilter{
  public:
    DummyModelFilter(){
      last_users_size_ = -1;
      last_items_size_ = -1;
    }
    virtual void run(RecDat* rec_dat) override;
    virtual vector<pair<int,double> >* get_global_users() override;
    virtual vector<pair<int,double> >* get_global_items() override;
    void set_users(vector<int>* users){ users_ = users; }
    void set_items(vector<int>* items){ items_ = items; }
    bool self_test(){
      bool OK = ModelFilter::self_test();
      return OK;
    }
  private:
    vector<int>* users_;
    vector<int>* items_;
    int last_users_size_;
    int last_items_size_;
    vector<pair<int,double>> user_filter_;
    vector<pair<int,double>> item_filter_;
};



#endif
