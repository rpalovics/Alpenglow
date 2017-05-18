struct Location {
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  int location_id;
  double x;
  double y;
  double z;
};

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
  Location location;
};

struct RecommenderDataParameters{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  std::string file_name;
  std::string type;
};

struct RecPred{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  double score;
  double prediction;
};

class RecommenderData{
  %TypeHeaderCode
  #include "recommender_data/RecommenderData.h"
  %End
  public:
    RecommenderData();
    RecommenderData(RecommenderDataParameters* params);
    ~RecommenderData();
    void read_from_file(std::string file_name, std::string type);
    void set_rec_dats(std::vector <RecDat> rec_data);
    RecDat* get(int idx);
    int size();
    SpMatrix* matrix();
    std::vector<int>* items();
    std::vector<int>* users();


    void get_itemsInto(VectorInt*);
    %MethodCode
        a0->vec = *(sipCpp->items());
    %End

    void get_usersInto(VectorInt*);
    %MethodCode
        a0->vec = *(sipCpp->users());
    %End

    void set_max_time(double _max_time);
    std::vector <RecDat>* get_rec_data();
    void init();
    void set_attribute_container(InlineAttributeReader* attribute_container);
};
