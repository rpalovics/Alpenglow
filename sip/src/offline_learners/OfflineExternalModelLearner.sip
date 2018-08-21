struct OfflineExternalModelLearnerParameters{
  %TypeHeaderCode
  #include "offline_learners/OfflineExternalModelLearner.h"
  %End
  std::string mode;
  std::string out_name_base;
  std::string in_name_base;
};
class OfflineExternalModelLearner : public OfflineLearner{
  %TypeHeaderCode
  #include "offline_learners/OfflineExternalModelLearner.h"
  %End
  public:
    OfflineExternalModelLearner(OfflineExternalModelLearnerParameters* parameters);
    ~OfflineExternalModelLearner();
    void fit(RecommenderData* recommender_data);
    void set_model(ExternalModel* model);
};
