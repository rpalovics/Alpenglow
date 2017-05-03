class GradientComputer /Abstract/{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputer();
    virtual ~GradientComputer();
    void set_model(Model* model_ /Transfer/);
    virtual void set_up(std::vector<RecDat>* trainData /Transfer/) = 0;
    // virtual vector<pair<RecDat*,double> >* get_next_gradient() = 0;
    virtual bool has_next() = 0;

    void setModel(Model* model_ /Transfer/);
    void setUp(std::vector<RecDat>* trainData /Transfer/);
    bool hasNext();
    // std::vector<std::pair<RecDat*,double> >* getNextGradient();
};

class GradientComputerPointWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerPointWise(ObjectivePointWise * _objective /Transfer/);
    GradientComputerPointWise();
    void set_objective(ObjectivePointWise* _objective /Transfer/);
    ~GradientComputerPointWise();
    virtual double get_gradient(RecDat* recDat);
    void set_up(std::vector<RecDat>* trainData_ /Transfer/);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};

class GradientComputerImplicitPairWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerImplicitPairWise(ObjectivePairWise * _objective /Transfer/);
    ~GradientComputerImplicitPairWise();
    void set_up(std::vector<RecDat>* trainData_ /Transfer/);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};

class GradientComputerListWise : public GradientComputer{
  %TypeHeaderCode
  #include "gradient_computers/GradientComputer.h"
  %End
  public:
    GradientComputerListWise(ObjectiveListWise * _objective /Transfer/);
    ~GradientComputerListWise();
    void set_up(std::vector<RecDat>* trainData_ /Transfer/);
    // vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next();
};