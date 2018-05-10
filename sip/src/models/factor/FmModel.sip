struct FmModelParameters{
  %TypeHeaderCode
  #include "models/factor/FmModel.h"
  %End
  int dimension;
  double begin_min;
  double begin_max;
  int seed;
  SparseAttributeContainer *user_attributes;
  SparseAttributeContainer *item_attributes;
};
class FmModel : public Model, public Initializable
{
  public:
    FmModel(FmModelParameters *parameters);
    double prediction(RecDat* rec_dat);
    void clear();
    bool self_test();
  protected:
    bool autocalled_initialize();
};
