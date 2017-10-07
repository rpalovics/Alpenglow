class FactorModelFilter : public ModelFilter, public NeedsExperimentEnvironment{
  %TypeHeaderCode
  #include "filters/FactorModelFilter.h"
  %End
  public:
    FactorModelFilter();
    ~FactorModelFilter();
    bool self_test();
    void run(RecDat* rd);
    std::vector<std::pair<int,double>>* get_global_users();
    std::vector<std::pair<int,double>>* get_global_items();
    void set_experiment_environment(ExperimentEnvironment*);
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
    void set_model(FactorModel * _model); 
  protected:
    bool autocalled_initialize();
};
