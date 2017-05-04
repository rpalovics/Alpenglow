#ifndef SIMILARITY_MODEL
#define SIMILARITY_MODEL

class SimilarityModel{
  public:
    SimilarityModel(){}
    virtual ~SimilarityModel(){}
    virtual double similarity(int item1, int item2)=0;
    bool self_test(){ return true; }
};


#endif
