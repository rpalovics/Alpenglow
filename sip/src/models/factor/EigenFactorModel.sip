struct EigenFactorModelParameters{
  %TypeHeaderCode
  #include "models/factor/EigenFactorModel.h"
  %End
  int dimension;
  double begin_min;
  double begin_max;
  int seed;
  int lemp_bucket_size;
};

class EigenFactorModel : public Model, public Initializable {
  %TypeHeaderCode
  #include "models/factor/EigenFactorModel.h"
  %End
  public:
    EigenFactorModel(EigenFactorModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    // void write(ofstream& file);
    // void read(ifstream& file);
    void clear();
    bool autocalled_initialize();
    bool self_test();
    void resize(int users, int items);

    // const EigenFactors& get_user_factors();
    // const EigenFactors& get_item_factors();
    // void set_user_factors(const EigenFactors& factors);
    // void set_item_factors(const EigenFactors& factors);
    // void set_user_factors(const MatrixXdRM& factors);
    // void set_item_factors(const MatrixXdRM& factors);
};