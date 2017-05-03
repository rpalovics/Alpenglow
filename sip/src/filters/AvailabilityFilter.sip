class AvailabilityFilter : ModelFilter {
  %TypeHeaderCode
  #include "filters/AvailabilityFilter.h"
  %End
  public:
    virtual void run(RecDat* rec_dat);
    virtual std::vector<std::pair<int,double>>* get_global_items();
    // virtual vector<pair<int,double>>* get_global_users();
    // virtual vector<pair<int,double>>* get_personalized_items(int user){
    //   return get_global_items();
    // }
    // virtual vector<pair<int,double>>* get_personalized_users(int item){
    //   return get_global_users();
    // }
    virtual bool active(RecDat*);
    virtual ~AvailabilityFilter();
    bool self_test();
    virtual void addAvailability(double time, int id, int duration);
};