//struct Location {
//  %TypeHeaderCode
//  #include "recommender_data/RecommenderData.h"
//  %End
//  int location_id;
//  double x;
//  double y;
//  double z;
//};

struct RecDat{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  int id;
  double score;
  double time;
  int user;
  int item;
  int eval;
  int category;
//  Location location;
};


struct RecPred{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  double score;
  double prediction;
};

class RecommenderData : public Initializable{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  public:
    ~RecommenderData();
    void set_rec_data(std::vector <RecDat> rec_data);
    RecDat* get(int idx);
    int size();
    SpMatrix* get_full_matrix();
    std::vector<int>* get_all_items();
    std::vector<int>* get_all_users();
    int get_max_user_id();
    int get_max_item_id();
    void clear();


    void get_items_into(VectorInt*);
    %MethodCode
        a0->vec = *(sipCpp->get_all_items());
    %End

    void get_users_into(VectorInt*);
    %MethodCode
        a0->vec = *(sipCpp->get_all_users());
    %End

    std::vector <RecDat>* get_rec_data();
  protected:
    bool autocalled_initialize();
};

struct LegacyRecommenderDataParameters{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  std::string file_name;
  std::string type;
  int experiment_termination_time;
};

class LegacyRecommenderData : public RecommenderData {
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  public:
    LegacyRecommenderData();
    LegacyRecommenderData(LegacyRecommenderDataParameters* params);
    void read_from_file(std::string file_name, std::string type);
    void set_attribute_container(InlineAttributeReader* attribute_container);
  protected:
    bool autocalled_initialize();
};
