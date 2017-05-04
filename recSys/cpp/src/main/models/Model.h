#ifndef MODEL
#define MODEL

#include "../recommender_data/RecommenderData.h"
#include <exception>

class Model{
  public:
    Model() {}
    virtual void add(RecDat * recDat) = 0;
    virtual double prediction(RecDat * recDat) = 0;
    //virtual void logParameters(RecDat * recDat) {}; deprecated unused function, use Logger instead
    virtual void writeParameters() {};
    virtual void write(ofstream& file) { throw exception(); }
    virtual void read(ifstream& file) { throw exception(); }
    virtual void clear(){ throw exception(); }
    virtual ~Model() {}
    bool selfTest(){ return self_test(); }
    bool self_test(){ return true; }
};

#endif
