struct AsymmetricFactorModelParameters{
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModel.h"
  %End
  int dimension;
  double begin_min;
  double begin_max;
  bool initialize_all;
  bool use_sigmoid;
  int max_item;
  std::string norm_type;
  double gamma;
  AsymmetricFactorModelParameters();
};

class AsymmetricFactorModel : public Model{
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModel.h"
  %End
  public:
    AsymmetricFactorModel(AsymmetricFactorModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    //void write(ofstream& file);
    //void read(ifstream& file);
    void clear();
    bool self_test();
};
