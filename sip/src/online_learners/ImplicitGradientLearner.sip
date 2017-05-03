class ImplicitGradientLearner : public OnlineLearner{
  %TypeHeaderCode
  #include "online_learners/ImplicitGradientLearner.h"
  %End
  public:
    ImplicitGradientLearner();
    ~ImplicitGradientLearner();
    void learn(RecDat* rec_dat);
    void set_train_matrix(SpMatrix* train_matrix /Transfer/);
    void set_gradient_computer(GradientComputer* gradient_computer /Transfer/);
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator /Transfer/);
    bool self_test();
};
