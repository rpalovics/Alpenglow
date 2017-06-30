struct SvdppModelParameters{
  %TypeHeaderCode
  #include "models/factor/SvdppModel.h"
  %End
  int dimension;
  double begin_min;
  double begin_max;
  bool use_sigmoid;
  double user_vector_weight;
  double history_weight;
  std::string norm_type;
  double gamma;
  SvdppModelParameters();
};

class SvdppModel : public Model {
  %TypeHeaderCode
  #include "models/factor/SvdppModel.h"
  %End
  public:
    SvdppModel(SvdppModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    //void write(ofstream& file);
    //void read(ifstream& file);
    void clear();
    bool self_test();
};
