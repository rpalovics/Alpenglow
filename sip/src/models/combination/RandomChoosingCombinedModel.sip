struct RandomChoosingCombinedModelParameters{
  %TypeHeaderCode
  #include "models/combination/RandomChoosingCombinedModel.h"
  %End
  int x;
};
class RandomChoosingCombinedModel : public Model, public Initializable, public NeedsExperimentEnvironment {
    %TypeHeaderCode
    #include "models/combination/RandomChoosingCombinedModel.h"
    %End
    public:
    RandomChoosingCombinedModel(RandomChoosingCombinedModelParameters* params);
    void add_model(Model* model);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    //void write(ostream& file) override;
    //void read(istream& file) override;
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
    bool self_test();
    protected:
    bool autocalled_initialize();
};
