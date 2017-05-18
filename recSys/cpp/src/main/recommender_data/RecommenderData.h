
#ifndef RECOMMENDER_DATA
#define RECOMMENDER_DATA
/* RecommenderData class
   
   RecommenderData reads scrobble files and stores RecDats.
*/

#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
#include "../utils/SpMatrix.h"
#include "AttributeContainer.h"

using namespace std;

struct Location {
  int location_id;
  double x;
  double y;
  double z;
};


struct Tweet {
  double time;
  double x;
  double y;
  double z;
  int location_id;
  double distance;
};


struct RecDat{
  int id;
  double score;
  double time;
  int user,item,eval;
  int category;
  Location location;
  // std::vector<std::vector<int>> *attributes;
  //bool operator<(const RecDat& rec_dat) const { return score < rec_dat.score; }
  //bool operator>(const RecDat& rec_dat) const { return score > rec_dat.score; }
  friend ostream &operator<<( ostream &output, const RecDat &rec_dat ){ 
     output << "t=" << (int) rec_dat.time << " u=" << rec_dat.user << " i=" << rec_dat.item << " s=" << rec_dat.score;
     return output;            
  }
};
typedef vector <RecDat> RecDats;

typedef map <int,double> Recommendation;
typedef map <int,double> :: iterator RecIterator;

struct RecPred{
  double score;
  double prediction;
};
typedef vector <RecPred> Predictions;
typedef vector <double> Gradients;

struct RecommenderDataParameters{
  string file_name;
  string type;
};
class RecommenderData{
  public:
    RecommenderData(){rec_matrix.clear();max_time=0;};
    RecommenderData(RecommenderDataParameters* params){
      file_name = params->file_name;
      type = params->type;
    }
    ~RecommenderData(){};
    void read_from_file(string file_name, string type);
    void read_from_file_core(istream& ifs, string type);
    void set_rec_dats(RecDats rec_data){ this->rec_data = rec_data; }
    RecDat* get(int idx){return &(rec_data[idx]);}
    int size(){return rec_data.size();}
    SpMatrix* matrix();
    vector<int>* items();
    vector<int>* users();
    void set_max_time(double _max_time){ max_time = _max_time; }
    RecDats* get_rec_data(){ return &rec_data; }
    void init(){
      read_from_file(file_name, type);
    }
    void set_attribute_container(InlineAttributeReader* attribute_container){
      attribute_container_ = attribute_container;
    }
  private:
    RecDats rec_data;
    SpMatrix rec_matrix;
    vector<int> items_;
    vector<int> users_;
    double max_time;
    string file_name;
    string type;
    InlineAttributeReader* attribute_container_;
};

#endif
