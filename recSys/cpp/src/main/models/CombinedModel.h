#ifndef COMBINED_MODEL
#define COMBINED_MODEL

#include <vector>
#include "Model.h"

using namespace std;

struct CombinedModelParameters{
  string logFileName;
  int logFrequency;
  bool useUserWeights;
};
class CombinedModel : public Model {
  public:
    CombinedModel(CombinedModelParameters* params){
      logFileStream = new ofstream(params->logFileName.c_str());
      logFrequency = params->logFrequency;
      logCounter = 0;
      useUserWeights=params->useUserWeights;
    };
    ~CombinedModel(){ delete logFileStream; };
    void addModel(Model* model){
      models.push_back(model);
      int modelNum=models.size();
      globalWeights.clear();
      globalWeights.resize(modelNum,1/(double)modelNum);
    }
    double prediction(RecDat* rec_dat);
    void add(RecDat* rec_dat);
  private:
    vector <Model*>  models;
    vector <vector<double>*> userWeights;
    vector <double> globalWeights;
    ofstream* logFileStream;
    int logFrequency;
    int logCounter;
    bool useUserWeights;
    friend class CombinedDoubleLayerModelGradientUpdater; 
    friend class CombinedSingleLayerModelGradientUpdater;
};

#endif
