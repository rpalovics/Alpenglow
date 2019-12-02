class PythonModel  : Model {
  %TypeHeaderCode
  #include "models/PythonModel.h"
  %End
};

class PythonToplistModel : public PythonModel, public TopListRecommender {
  %TypeHeaderCode
  #include "models/PythonModel.h"
  %End
};

class PythonRankingIteratorModel : public PythonModel, public RankingScoreIteratorProvider{
  %TypeHeaderCode
  #include "models/PythonModel.h"
  %End
  public:
    virtual bool iterator_has_next_(int id, int user, double upper_bound)=0;
    virtual std::pair<int, double> iterator_get_next_(int id, int user)=0;
};