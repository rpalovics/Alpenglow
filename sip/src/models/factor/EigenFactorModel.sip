
struct EigenFactorModelParameters {
  %TypeHeaderCode
  #include "models/factor/EigenFactorModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.EigenFactorModel`. See documentation there.
  %End
  int dimension;
  double begin_min;
  double begin_max;
  int seed;
  int lemp_bucket_size;
};

class EigenFactorModel
: public Model,
//  virtual public RankingScoreIteratorProvider,
//  public ToplistFromRankingScoreRecommender,
  public Initializable
{
  %TypeHeaderCode
  #include "models/factor/EigenFactorModel.h"
  %End
  public:
    EigenFactorModel(EigenFactorModelParameters *parameters);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
//    void write(ostream& file);
//    void read(istream& file);
    void clear();
    bool self_test();
    void resize(int users, int items);

//    const EigenFactors& get_user_factors();
//    const EigenFactors& get_item_factors();
//    void set_user_factors(const MatrixXdRM& factors, std::vector<bool> seen);
//    void set_item_factors(const MatrixXdRM& factors, std::vector<bool> seen);

//    RankingScoreIterator* get_ranking_score_iterator(int u);
  protected:
    bool autocalled_initialize();
};

