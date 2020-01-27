#ifndef RECOMMENDER_DATA_H
#define RECOMMENDER_DATA_H
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

struct RecDat {
  int id = 0;
  double score = 0;
  double time = 0;
  int user = 0;
  int item = 0;
  int eval = 0;
  int category = 0;
  // std::vector<std::vector<int>> *attributes;
  //bool operator<(const RecDat& rec_dat) const { return score < rec_dat.score; }
  //bool operator>(const RecDat& rec_dat) const { return score > rec_dat.score; }
  friend ostream &operator<<( ostream &output, const RecDat &rec_dat ){ 
     output << "id=" << rec_dat.id << " t=" << (int) rec_dat.time << " u=" << rec_dat.user << " i=" << rec_dat.item << " s=" << rec_dat.score;
     return output;            
  }
};
typedef vector<RecDat> RecDats;

struct RecPred{
  double score;
  double prediction;
};

class RecommenderData : public Initializable {
  public:
    RecDat* get(int idx){return &(rec_data_[idx]);}
    void set_rec_data(vector<RecDat> rec_data_){ this->rec_data_ = rec_data_; }
    vector<RecDat>* get_rec_data(){ return &rec_data_; }
    int size(){return rec_data_.size();}
    SpMatrix* get_full_matrix();
    vector<int>* get_all_items();
    vector<int>* get_all_users();
    int get_max_user_id(){
      get_all_users(); //fills vector
      return *(max_element(users_.begin(),users_.end()));
    }
    int get_max_item_id(){
      get_all_items(); //fills vector
      return *(max_element(items_.begin(),items_.end()));
    }
    void clear();
    virtual ~RecommenderData(){}
    bool self_test(){ return size()>0; }
  protected:
    bool autocalled_initialize() override { return true; }
    vector<RecDat> rec_data_;
  private:
    SpMatrix full_matrix_;
    vector<int> items_;
    vector<int> users_;
};

struct LegacyRecommenderDataParameters {
  string file_name = "";
  string type = "online";
  int experiment_termination_time = 0;
};
class LegacyRecommenderData : public RecommenderData {
  public:
    LegacyRecommenderData(){}
    LegacyRecommenderData(LegacyRecommenderDataParameters* params){
      set_parameters(params);
    }
    void set_parameters(LegacyRecommenderDataParameters* params){
      file_name_ = params->file_name;
      type_ = params->type;
      experiment_termination_time_ = params->experiment_termination_time;
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
      read_from_file(file_name_, type_);
      return true;
    }
  private:
    bool parent_is_initialized_ = false;
    int experiment_termination_time_ = 0;
    string file_name_ = "";
    string type_ = "online";
    InlineAttributeReader* attribute_container_;
};

#endif /* RECOMMENDER_DATA_H */
