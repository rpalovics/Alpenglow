
struct SamplingDataGeneratorParameters {
  %TypeHeaderCode
  #include "data_generators/SamplingDataGenerator.h"
  %End
  std::string distribution;
  int number_of_samples;
  double y; 
  double geometric_param; 
};
class SamplingDataGenerator : public DataGenerator, public Initializable, public NeedsExperimentEnvironment {
  %TypeHeaderCode
  #include "data_generators/SamplingDataGenerator.h"
  %End
  public:
    SamplingDataGenerator(SamplingDataGeneratorParameters* params);
    RecommenderData* generate_recommender_data(RecDat*);
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

