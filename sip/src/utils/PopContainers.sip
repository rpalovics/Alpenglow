
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

//    pair<int,double> get(int idx);

    int get_item(int idx);

    int size();

};

