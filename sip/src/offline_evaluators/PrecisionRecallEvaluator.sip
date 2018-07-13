
struct PrecisionRecallEvaluatorParameters{
  %TypeHeaderCode
  #include "offline_evaluators/PrecisionRecallEvaluator.h"
  %End
  int cutoff;
  std::string test_file_name;
  std::string test_file_type;
  int time;
};

class PrecisionRecallEvaluator : public OfflineEvaluator{
  %TypeHeaderCode
  #include "offline_evaluators/PrecisionRecallEvaluator.h"
  %End
  public:
    PrecisionRecallEvaluator(PrecisionRecallEvaluatorParameters* params);
    virtual ~PrecisionRecallEvaluator();
    void set_model(Model* model);
    void set_train_data(RecommenderData* recommender_data);
    void set_model_filter(ModelFilter* model_filter);
    virtual void evaluate();
    bool self_test();
};

