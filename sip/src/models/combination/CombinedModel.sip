
struct CombinedModelParameters {
  %TypeHeaderCode
  #include "models/combination/CombinedModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.CombinedModel`. See documentation there.
  %End
  std::string log_file_name;
  int log_frequency;
  bool use_user_weights;
};
class CombinedModel : public Model {
  %TypeHeaderCode
  #include "models/combination/CombinedModel.h"
  %End
  public:
    CombinedModel(CombinedModelParameters* params);
    ~CombinedModel();
    void add_model(Model* model);
    double prediction(RecDat* rec_dat);
    void add(RecDat* rec_dat);
};

