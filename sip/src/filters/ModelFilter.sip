
class ModelFilter {
  %TypeHeaderCode
  #include "filters/ModelFilter.h"
  %End
public:
  virtual void run(RecDat* rec_dat);
  virtual void run(double time);
//  virtual std::vector<pair<int,double>>* get_global_items();
//  virtual std::vector<pair<int,double>>* get_global_users();
//  virtual std::vector<pair<int,double>>* get_personalized_items(int user);
//  virtual std::vector<pair<int,double>>* get_personalized_users(int item);
  virtual bool active(RecDat*);
  virtual ~ModelFilter();
  bool self_test();
};

