struct UniformNegativeSampleGeneratorParameters{
    %TypeHeaderCode
    #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
    %End
    double negative_rate; 
    bool initialize_all;
    int max_item;
    bool filter_repeats;
    int seed;
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator, public Initializable, public NeedsExperimentEnvironment{
    %TypeHeaderCode
    #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
    %End
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* params);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_items(std::vector<int>* items);
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    void set_experiment_environment(ExperimentEnvironment* ee);
    bool self_test();
    //vector<int>* generate(RecDat * rec_dat);
  protected:
    bool autocalled_initialize();
};
