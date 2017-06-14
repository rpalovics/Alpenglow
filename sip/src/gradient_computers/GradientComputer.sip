class GradientComputer /Abstract/{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputer();
    virtual ~GradientComputer();
    void set_model(Model* model_);
    virtual void set_up(std::vector<RecDat>* train_data) = 0;
    // virtual vector<pair<RecDat*,double> >* get_next_gradient() = 0;
    virtual bool has_next() = 0;
};

class GradientComputerPointWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    //GradientComputerPointWise(ObjectivePointWise * _objective);
    GradientComputerPointWise();
    void set_objective(ObjectivePointWise* _objective);
    virtual double get_gradient(RecDat* rec_dat);
    void set_up(std::vector<RecDat>* train_data_);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};

class GradientComputerImplicitPairWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerImplicitPairWise();
    void set_objective(ObjectivePairWise * _objective);
    void set_up(std::vector<RecDat>* train_data_);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};

class GradientComputerListWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerListWise();
    void set_objective(ObjectiveListWise * _objective);
    void set_up(std::vector<RecDat>* train_data_);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};
