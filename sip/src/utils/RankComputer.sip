
struct RankComputerParameters {
  %TypeHeaderCode
  #include "utils/RankComputer.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.RankComputer`. See documentation there.
  %End
  int random_seed;

  int top_k;
  int exclude_known;
};

class RankComputer : public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/RankComputer.h"
  %End
  public:
    RankComputer();
    RankComputer(RankComputerParameters* parameters);
    void set_parameters(RankComputerParameters* parameters);
    virtual ~RankComputer();
    void set_model(Model* model);

    void set_train_matrix(SpMatrix* train_matrix);
    void set_top_pop_container(TopPopContainer* popularity_sorted_container);
    void set_items(const std::vector<int> *items);
    bool self_test();
    int get_rank(RecDat*);
  protected:
    bool autocalled_initialize();
};

