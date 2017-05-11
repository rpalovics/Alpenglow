class ImplicitGradientLearner : public OnlineLearner{
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
};
