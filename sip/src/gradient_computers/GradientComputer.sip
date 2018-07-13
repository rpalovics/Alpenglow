
class GradientComputer : public Updater /Abstract/{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    void set_model(Model* model);
    void add_gradient_updater(ModelGradientUpdater* gradient_updater);
//    void message(UpdaterMessage message);
    bool self_test();

};

class GradientComputerPointWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    void set_objective(ObjectivePointWise* objective);
    void update(RecDat* rec_dat);
    bool self_test();
};

