// class FactorFilter{
//   %TypeHeaderCode
//   #include "filters/FactorModelFilter.h"
//   %End
//   public:
//     FactorFilter();
//     ~FactorFilter();
//     void setFactors(Factors * _factors, Factors * _otherFactors);
//     void setEntities(std::vector <int> * _entities, std::vector <int> * _otherEntities);
//     void setUpperVector(std::vector<pair<int,double> >* upperBounds_);
//     bool self_test();
//     void computeBoundVectors();
//     void computeBounds();
//     void run();
//     std::vector<double>* getUpper();
//     std::vector<double>* getLower(); 
// };

class FactorModelFilter : public ModelFilter{
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
    void setItems(std::vector<int>* items /Transfer/);
    void setItems(VectorInt);
    %MethodCode
        sipCpp->setItems(&(a0->vec));
    %End
    void setUsers(std::vector<int>* users /Transfer/);
    void setUsers(VectorInt);
    %MethodCode
        sipCpp->setUsers(&(a0->vec));
    %End
    void setModel(FactorModel * _model); 
};
