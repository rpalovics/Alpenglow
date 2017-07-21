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
    double get_gradient(RecDat* rec_dat);
    bool self_test();
};

//class GradientComputerImplicitPairWise : public GradientComputer{
//  %TypeHeaderCode
//  #include "gradient_computers/GradientComputer.h"
//  %End
//  public:
//    GradientComputerImplicitPairWise();
//    void set_objective(ObjectivePairWise * _objective);
//    void set_up(std::vector<RecDat>* train_data_);
//    // vector<pair<RecDat*,double> > * get_next_gradient();
//    bool has_next();
//};
//
//class GradientComputerListWise : public GradientComputer{
//  %TypeHeaderCode
//  #include "gradient_computers/GradientComputer.h"
//  %End
//  public:
//    GradientComputerListWise();
//    void set_objective(ObjectiveListWise * _objective);
//    void set_up(std::vector<RecDat>* train_data_);
//    // vector<pair<RecDat*,double> > * get_next_gradient();
//    bool has_next();
//};
