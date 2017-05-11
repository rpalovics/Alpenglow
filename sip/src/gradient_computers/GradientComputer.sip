class GradientComputer /Abstract/{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputer();
    virtual ~GradientComputer();
    void set_model(Model* model_);
    virtual void set_up(std::vector<RecDat>* trainData) = 0;
    // virtual vector<pair<RecDat*,double> >* get_next_gradient() = 0;
    virtual bool has_next() = 0;

    void setModel(Model* model_);
    void setUp(std::vector<RecDat>* trainData);
    bool hasNext();
    // std::vector<std::pair<RecDat*,double> >* getNextGradient();
};

class GradientComputerPointWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerPointWise(ObjectivePointWise * _objective);
    GradientComputerPointWise();
    void set_objective(ObjectivePointWise* _objective);
    ~GradientComputerPointWise();
    virtual double get_gradient(RecDat* recDat);
    void set_up(std::vector<RecDat>* trainData_);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};

class GradientComputerImplicitPairWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerImplicitPairWise(ObjectivePairWise * _objective);
    ~GradientComputerImplicitPairWise();
    void set_up(std::vector<RecDat>* trainData_);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};

class GradientComputerListWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerListWise(ObjectiveListWise * _objective);
    ~GradientComputerListWise();
    void set_up(std::vector<RecDat>* trainData_);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};