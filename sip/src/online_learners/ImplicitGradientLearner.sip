class ImplicitGradientLearner : public Updater, public Initializable, public INeedExperimentEnvironment {
  %TypeHeaderCode
  #include "online_learners/ImplicitGradientLearner.h"
  %End
  public:
    ImplicitGradientLearner();
    ~ImplicitGradientLearner();
    void update(RecDat* rec_dat);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_gradient_computer(GradientComputer* gradient_computer);
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator);
    void add_simple_updater(ModelSimpleUpdater* model_updater);
    void add_gradient_updater(ModelGradientUpdater* model_updater);
    void add_multi_updater(ModelMultiUpdater* model_updater); 
    void set_model(Model* model);
    bool self_test();
    bool init();
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
};
