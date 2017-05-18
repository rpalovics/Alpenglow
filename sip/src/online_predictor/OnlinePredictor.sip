struct OnlinePredictorParameters{
  %TypeHeaderCode
  #include "online_predictor/OnlinePredictor.h"
  %End
  double min_time;
  double time_frame;
  std::string file_name;
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
    void set_prediction_creator(PredictionCreator* predictionCreator_);
    bool self_test();
};
