#ifndef MODEL_H
#define MODEL_H

#include "../recommender_data/RecommenderData.h"
#include <stdexcept>
#include <string>

using namespace std;

class Model{
  public:
    virtual void add(RecDat* rec_dat){} //called only before gradient update
    virtual double prediction(RecDat* rec_dat) = 0;
    virtual void write(ostream& file) { throw logic_error("Not implemented."); }
    virtual void read(istream& file) { throw logic_error("Not implemented."); }
    void write(string file_name);
    void read(string file_name);
    virtual void clear(){ throw logic_error("Not implemented."); }
    virtual ~Model() {}
    bool self_test(){ return true; }
};

#endif /* MODEL_H */
