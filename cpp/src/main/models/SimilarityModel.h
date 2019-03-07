#ifndef SIMILARITY_MODEL_H
#define SIMILARITY_MODEL_H

//SIP_AUTOCONVERT

class SimilarityModel{
  public:
    SimilarityModel(){}
    virtual ~SimilarityModel(){}
    virtual double similarity(int item1, int item2)=0;
    bool self_test(){ return true; }
};


#endif /* SIMILARITY_MODEL_H */
