struct TimeframeDataGeneratorParameters {
  %TypeHeaderCode
  #include "data_generators/TimeframeDataGenerator.h"
  %End
  int time_frame_length;
};
class TimeframeDataGenerator : public DataGenerator, public INeedExperimentEnvironment, public Initializable {
  public:
    TimeframeDataGenerator(TimeframeDataGeneratorParameters* params);
    RecommenderData* generate_recommender_data(RecDat*);
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
  protected:
    bool autocalled_initialize();
};
