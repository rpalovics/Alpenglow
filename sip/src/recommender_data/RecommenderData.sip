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
    void readFromFile(std::string file_name, std::string type);
    void setRecDats(std::vector <RecDat> recData);
    RecDat* get(int idx);
    int size();
    SpMatrix* matrix();
    std::vector<int>* items();
    std::vector<int>* users();


    void getItemsInto(VectorInt*);
    %MethodCode
        a0->vec = *(sipCpp->items());
    %End

    void getUsersInto(VectorInt*);
    %MethodCode
        a0->vec = *(sipCpp->users());
    %End

    void setMaxTime(double _max_time);
    std::vector <RecDat>* getRecData();
    void init();
    void set_attribute_container(InlineAttributeReader* attribute_container);
};
