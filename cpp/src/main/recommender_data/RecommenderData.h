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
#include "../general_interfaces/Initializable.h"
#include "macros.h"

using namespace std;

struct RecDat{
  int id;
  double score;
  double time;
  int user,item,eval;
  int category;
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

class RecommenderData : public Initializable {
  public:
    RecDat* get(int idx){return &(rec_data[idx]);}
    void set_rec_dats(RecDats rec_data){ this->rec_data = rec_data; } //TODO rename: set_rec_data
    RecDats* get_rec_data(){ return &rec_data; }
    int size(){return rec_data.size();}
    SpMatrix* matrix(); //TODO rename get_full_matrix
    vector<int>* items(); //TODO rename get_all_items
    vector<int>* users(); //TODO rename get_all_users
    virtual ~RecommenderData(){};
  protected:
    bool autocalled_initialize() override { return true; }
    RecDats rec_data; //TODO rec_data_ vagy timeline_
  private:
    SpMatrix rec_matrix; // TODO rename full_matrix_
    vector<int> items_;
    vector<int> users_;
};

struct LegacyRecommenderDataParameters{
  string file_name;
  string type;
  int max_time;
};
class LegacyRecommenderData : public RecommenderData {
  public:
    LegacyRecommenderData(){max_time_=0;}
    LegacyRecommenderData(LegacyRecommenderDataParameters* params){
      set_parameters(params);
    }
    void set_parameters(LegacyRecommenderDataParameters* params){
      file_name = params->file_name;
      type = params->type;
      max_time_ = params->max_time;
    }
    void read_from_file(string file_name, string type); //private?
    void read_from_file_core(istream& ifs, string type); //private?
    void set_attribute_container(InlineAttributeReader* attribute_container){
      attribute_container_ = attribute_container;
    }
  protected:
    bool autocalled_initialize() override {
      if (!parent_is_initialized_){
        parent_is_initialized_ = RecommenderData::autocalled_initialize();
        if (!parent_is_initialized_) return false;
      }
      read_from_file(file_name, type);
      return true;
    }
  private:
    bool parent_is_initialized_ = false;
    int max_time_;
    string file_name;
    string type;
    InlineAttributeReader* attribute_container_;
};

#endif
