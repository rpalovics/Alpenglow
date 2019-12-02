class TopListRecommender{
  %TypeHeaderCode
  #include "models/TopListRecommender.h"
  %End
  public:
    virtual std::vector<std::pair<int,double>> get_top_list(int user, int k, const std::map<int,double> *exclude) = 0;
};