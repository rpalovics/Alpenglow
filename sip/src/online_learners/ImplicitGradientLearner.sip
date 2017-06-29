class ImplicitGradientLearner : public OnlineLearner, public Initializable, public INeedExperimentEnvironment {
  %TypeHeaderCode
  #include "online_learners/ImplicitGradientLearner.h"
  %End
  public:
    ImplicitGradientLearner();
    ~ImplicitGradientLearner();
    void learn(RecDat* rec_dat);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_gradient_computer(GradientComputer* gradient_computer);
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator);
    bool self_test();
    bool init();
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
};
