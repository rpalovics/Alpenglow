class PopContainer{
  %TypeHeaderCode
  #include "utils/PopContainers.h"
  %End
  public:
    PopContainer();
    ~PopContainer();
    void increase(int item);
    void reduce(int item);
    void resize(int item);
    int get(int item);
    void clear();
};

class TopPopContainer{
  %TypeHeaderCode
  #include "utils/PopContainers.h"
  %End
  public:
    TopPopContainer();
    ~TopPopContainer();
    void increase( int item);
    void reduce(int item);
    // std::map <int,double> * get_recommendation(int num);
    // std::pair<int,double> get(int idx);
    // std::pair<int,double> get_normed(int idx);
    int get_item(int idx);
    int get_index(int item);
    int size();
    void set_threshold(int num);
    bool has_changed();
};