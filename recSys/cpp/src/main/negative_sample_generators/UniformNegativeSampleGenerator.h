#ifndef UNIFORM_NEGATIVE_SAMPLE_GENERATOR
#define UNIFORM_NEGATIVE_SAMPLE_GENERATOR

#include "NegativeSampleGenerator.h"

struct UniformNegativeSampleGeneratorParameters{
    double negativeRate; 
    bool initializeAll;
    int maxItem;
    bool filterRepeats;
    UniformNegativeSampleGeneratorParameters(){
      negativeRate=-1;
      initializeAll=false;
      maxItem=-1;
      filterRepeats=false;
    }
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator{
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters * parameters)
    :filterRepeats(parameters->filterRepeats){
      setParameters(parameters);
    };
    void setParameters (UniformNegativeSampleGeneratorParameters * parameters);
    void setTrainMatrix(SpMatrix* trainMatrix_){
      trainMatrix=trainMatrix_;
    }
    void setItems(vector<int>* items_){
      if(!initializeAll) items=items_;
    }
    vector <int> * generate(RecDat * recDat);
    bool selfTest(){
      bool ok = NegativeSampleGenerator::selfTest();
      if(negativeRate < 0){
        ok=false;
        cerr << "UniformNegativeSampleGeneratorParameters::negativeRate is negative." << endl;
      }
      if(initializeAll && maxItem<0){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::maxItem is negative but initializeAll is set." << endl;
      }
      if(items==NULL){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::items is not set." << endl;
      }
      if(trainMatrix == NULL){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::trainMatrix is not set." << endl;
      }
      return ok;
    }
  protected:
    vector <int> * items;
    SpMatrix * trainMatrix; 
    double negativeRate;
    bool initializeAll;
    int maxItem;
    const bool filterRepeats;
};

#endif
