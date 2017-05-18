#ifndef MODEL
#define MODEL

#include "../recommender_data/RecommenderData.h"
#include <exception>

class Model{
  public:
    Model() {}
    virtual void add(RecDat* rec_dat) = 0;
    virtual double prediction(RecDat* rec_dat) = 0;
    virtual void write(ofstream& file) { throw exception(); }
    virtual void read(ifstream& file) { throw exception(); }
    virtual void clear(){ throw exception(); }
    virtual ~Model() {}
    bool self_test(){ return true; }
};

#endif
