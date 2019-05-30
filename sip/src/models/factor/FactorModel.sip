
struct FactorModelParameters {
  %TypeHeaderCode
  #include "models/factor/FactorModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.FactorModel`. See documentation there.
  %End
  int dimension;
  double begin_min;
  double begin_max;
  bool use_sigmoid;
  bool use_item_bias;
  bool use_user_bias;
  int seed;
  int lemp_bucket_size;
  int initialize_all;
  int max_item;
  int max_user;
};

class FactorModel
  : public Model, 
    public SimilarityModel,
    public NeedsExperimentEnvironment,
//    virtual public RankingScoreIteratorProvider,
//    public ToplistFromRankingScoreRecommender,
    public Initializable
{
  %TypeHeaderCode
  #include "models/factor/FactorModel.h"
  %End
  public:
    FactorModel(FactorModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    double similarity(int item1, int item2);
//    void write(ostream& file);
//    void read(istream& file);
    void clear();
    void set_user_recency(Recency *recency);
    void set_item_recency(Recency *recency);
//    RankingScoreIterator* get_ranking_score_iterator(int u);
    bool self_test();

  protected:
    bool autocalled_initialize();
};

