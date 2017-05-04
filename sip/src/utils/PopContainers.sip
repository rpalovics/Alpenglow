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
    // std::map <int,double> * getRecommendation(int num);
    // std::pair<int,double> get(int idx);
    // std::pair<int,double> getNormed(int idx);
    int getItem(int idx);
    int getIndex(int item);
    int size();
    void setThreshold(int num);
    bool hasChanged();
};