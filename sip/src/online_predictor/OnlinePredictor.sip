struct OnlinePredictorParameters{
  %TypeHeaderCode
  #include "online_predictor/OnlinePredictor.h"
  %End
  double minTime;
  double timeFrame;
  std::string fileName;
};


class OnlinePredictor : public Logger /NoDefaultCtors/{
  %TypeHeaderCode
  #include "online_predictor/OnlinePredictor.h"
  %End
  public:
    OnlinePredictor(OnlinePredictorParameters * params);
    ~OnlinePredictor();
    void run(RecDat * recDat);
    void setParameters(OnlinePredictorParameters* params);
    void setPredictionCreator(PredictionCreator* predictionCreator_);
    bool selfTest();
};
