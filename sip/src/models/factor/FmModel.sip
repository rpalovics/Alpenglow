
struct FmModelParameters {
  %TypeHeaderCode
  #include "models/factor/FmModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.FmModel`. See documentation there.
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
  %TypeHeaderCode
  #include "models/factor/FmModel.h"
  %End
  public:
    FmModel(FmModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    void clear();
    bool self_test();
  protected:
    bool autocalled_initialize();
};

