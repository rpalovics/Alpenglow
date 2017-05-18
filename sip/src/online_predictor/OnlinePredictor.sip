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
    void run(RecDat * rec_dat);
    void set_parameters(OnlinePredictorParameters* params);
    void set_prediction_creator(PredictionCreator* prediction_creator);
    bool self_test();
};
