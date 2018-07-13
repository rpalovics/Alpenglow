
class SimilarityModel{
  %TypeHeaderCode
  #include "models/SimilarityModel.h"
  %End
  public:
    SimilarityModel();
    virtual ~SimilarityModel();
    virtual double similarity(int item1, int item2)=0;
    bool self_test();
};

