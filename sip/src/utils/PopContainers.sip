
class PopContainer{
  %TypeHeaderCode
  #include "utils/PopContainers.h"
  %End
  public:
    void increase(int item);
    void reduce(int item);
    int get(int item);
    void clear();
};

class TopPopContainer{
  %TypeHeaderCode
  #include "utils/PopContainers.h"
  %End
  public:
    void increase( int item);
    void reduce(int item);
//    map<int,double>* get_recommendation(int num);
//    pair<int,double> get(int idx);
//    pair<int,double> get_normed(int idx);
    int get_item(int idx);
    int get_index(int item);
    int size();
    void set_threshold(int num);
    bool has_changed();
};

