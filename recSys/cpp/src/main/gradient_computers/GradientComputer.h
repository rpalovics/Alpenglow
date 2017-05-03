#ifndef GRADIENT_COMPUTER
#define GRADIENT_COMPUTER

#include "../recommender_data/RecommenderData.h"
#include "../objectives/Objective.h"
#include "../models/Model.h"

using namespace std;

class GradientComputer{
  public:
    GradientComputer(){};
    virtual ~GradientComputer(){};
    void set_model(Model* model_){model=model_;}
    virtual void set_up(vector<RecDat>* trainData) = 0;
    virtual vector<pair<RecDat*,double> >* get_next_gradient() = 0;
    virtual bool has_next() = 0;

    //DEPRECATED
    void setModel(Model* model_){
      cerr << "GradientComputer::setModel is DEPRECATED, use google code." << endl;
      set_model(model_);
    }
    void setUp(vector<RecDat>* trainData){ set_up(trainData); }//TODO google_code_temporal
    bool hasNext(){ return has_next(); }//TODO google_code_temporal
    vector<pair<RecDat*,double> >* getNextGradient(){ return get_next_gradient(); }//TODO google_code_temporal
  protected: 
    Model* model;
    vector<pair<RecDat*,double> > gradientVector;
    vector<RecDat>* trainData;
};

class GradientComputerPointWise : public GradientComputer{
  public:
    GradientComputerPointWise(ObjectivePointWise * _objective){
      objective = _objective;
    };
    GradientComputerPointWise(){}
    void set_objective(ObjectivePointWise* _objective){
      objective = _objective;
    }
    ~GradientComputerPointWise(){};
    virtual double get_gradient(RecDat* recDat);
    void set_up(vector<RecDat>* trainData_){
      trainData = trainData_;
      trainDataIt = trainData->begin();
    }
    vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next(){return (trainDataIt!=trainData->end());}
  protected:
    ObjectivePointWise * objective;
    vector<RecDat>::iterator trainDataIt;

};

class GradientComputerImplicitPairWise : public GradientComputer{
  public:
    GradientComputerImplicitPairWise(ObjectivePairWise * _objective){
      objective = _objective;
    };
    ~GradientComputerImplicitPairWise(){};
    void set_up(vector<RecDat>* trainData_){
      trainData=trainData_;
      trainDataIt = trainData->begin();
      positiveData = &(*trainDataIt);
      trainDataIt++;
    }
    vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next(){return (trainDataIt!=trainData->end());}
  protected:
    ObjectivePairWise * objective;
    vector<RecDat>::iterator trainDataIt;
    RecDat* positiveData;
};

class GradientComputerListWise : public GradientComputer{
  public:
    GradientComputerListWise(ObjectiveListWise * _objective){
      objective = _objective;
      hasNext_ = false;
    };
    ~GradientComputerListWise(){};
    void set_up(vector<RecDat>* trainData_){
      trainData = trainData_;
      hasNext_=true;
    }
    vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next(){return hasNext_;}
  protected:
    ObjectiveListWise * objective;
    bool hasNext_;
};

#endif
