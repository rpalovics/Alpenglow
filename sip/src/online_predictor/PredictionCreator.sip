struct PredictionCreatorParameters{
  %TypeHeaderCode
  #include "online_predictor/PredictionCreator.h"
  %End
  int topK;
  int lookback;
};
class PredictionCreator{
  %TypeHeaderCode
  #include "online_predictor/PredictionCreator.h"
  %End
  public:
    PredictionCreator(PredictionCreatorParameters * params);
    virtual ~PredictionCreator();
    virtual std::vector<RecDat>* run(RecDat * recDat)=0; 
    void setModel(Model* model_);
    void setFilter(ModelFilter* filter_);
    void setTrainMatrix(SpMatrix *trainMatrix_);
    virtual bool selfTest();
};

struct PredictionCreatorGlobalParameters : public PredictionCreatorParameters{
  %TypeHeaderCode
  #include "online_predictor/PredictionCreator.h"
  %End
  int initialThreshold;
};

class PredictionCreatorGlobal: public PredictionCreator{
  %TypeHeaderCode
  #include "online_predictor/PredictionCreator.h"
  %End
  public:
    PredictionCreatorGlobal(PredictionCreatorGlobalParameters* params);
    virtual ~PredictionCreatorGlobal();
    std::vector<RecDat>* run(RecDat* recDat);
    bool selfTest();
};

struct PredictionCreatorPersonalizedParameters : public PredictionCreatorParameters{
  %TypeHeaderCode
  #include "online_predictor/PredictionCreator.h"
  %End
};
class PredictionCreatorPersonalized: public PredictionCreator{
  %TypeHeaderCode
  #include "online_predictor/PredictionCreator.h"
  %End
  public:
    PredictionCreatorPersonalized(PredictionCreatorParameters * params);
    virtual ~PredictionCreatorPersonalized();
    std::vector<RecDat>* run(RecDat * recDat);
    bool selfTest();
};
