class GradientComputer : public Updater /Abstract/{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    void set_model(Model* model_);
    void add_gradient_updater(ModelGradientUpdater* updater);
    bool self_test();
};

class GradientComputerPointWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    void set_objective(ObjectivePointWise* _objective);
    void update(RecDat* rec_dat);
    bool self_test();
};
