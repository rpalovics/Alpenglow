#ifndef MODEL
#define MODEL

#include "../recommender_data/RecommenderData.h"
#include <exception>
#include <string>

using namespace std;

class Model{
  public:
    virtual void add(RecDat* rec_dat){} //called only before gradient update
    virtual double prediction(RecDat* rec_dat) = 0;
    virtual void write(ostream& file) { throw exception(); }
    virtual void read(istream& file) { throw exception(); }
    void write(string file_name);
    void read(string file_name);
    virtual void clear(){ throw exception(); }
    virtual ~Model() {}
    bool self_test(){ return true; }
};

#endif
