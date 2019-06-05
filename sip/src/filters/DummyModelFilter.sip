
class DummyModelFilter : public ModelFilter, public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "filters/DummyModelFilter.h"
  %End
  public:
    void run(RecDat* rec_dat);
//    std::vector<pair<int,double>>* get_global_users();
//    std::vector<pair<int,double>>* get_global_items();
    void set_users(std::vector<int>* users);
    void set_users(VectorInt);
    %MethodCode
        sipCpp->set_users(&(a0->vec));
    %End
    void set_items(std::vector<int>* items);
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    bool self_test();
  protected:
    bool autocalled_initialize();
};

