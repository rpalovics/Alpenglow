
struct AsymmetricFactorModelParameters {
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.AsymmetricFactorModel`. See documentation there.
  %End
  int dimension;
  double begin_min;
  double begin_max;
  bool use_sigmoid;
  std::string norm_type; 
  double gamma;
  int seed;
  int initialize_all;
  int max_item;
};

class AsymmetricFactorModel
: public Model
, public NeedsExperimentEnvironment
, public Initializable
{
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModel.h"
  %End
  public:
    AsymmetricFactorModel(AsymmetricFactorModelParameters* parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
//    void write(ostream& file);
//    void read(istream& file);
    void clear();
    bool self_test();
  protected:
    bool autocalled_initialize();
};

