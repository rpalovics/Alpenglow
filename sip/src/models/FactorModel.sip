struct FactorModelParameters{
  %TypeHeaderCode
  #include "models/FactorModel.h"
  %End
  int dimension;
  double begin_min;
  double begin_max;
  bool use_sigmoid;
  bool use_item_bias;
  double use_user_bias;
  bool initialize_all;
  int max_item;
  int max_user;
  FactorModelParameters();
};

class FactorModel : public Model, public SimilarityModel {
  %TypeHeaderCode
  #include "models/FactorModel.h"
  %End
  public:
    FactorModel(FactorModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    double similarity(int item1, int item2);
    // void write(ofstream& file);
    // void read(ifstream& file);
    void clear();
    void set_user_recency(Recency *recency);
    void set_item_recency(Recency *recency);
    void init();
    bool self_test();
};
