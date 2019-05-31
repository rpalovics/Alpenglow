
struct SvdppModelParameters {
  %TypeHeaderCode
  #include "models/factor/SvdppModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.SvdppModel`. See documentation there.
  %End
  int dimension;
  double begin_min;
  double begin_max;
  bool use_sigmoid;
  double user_vector_weight;
  double history_weight;
  std::string norm_type;
  double gamma;
  int seed;
  int initialize_all;
  int max_item;
  int max_user;
};

class SvdppModel
: public Model
, public NeedsExperimentEnvironment
, public Initializable
{
  %TypeHeaderCode
  #include "models/factor/SvdppModel.h"
  %End
  public:
    SvdppModel(SvdppModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
//    void write(ostream& file);
//    void read(istream& file);
    void clear();

    bool self_test();
  protected:
    bool autocalled_initialize();
};

