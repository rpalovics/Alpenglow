struct OnlinePredictions{
  %TypeHeaderCode
  #include "loggers/PredictionLogger.h"
  %End
  std::vector<int> ids;
  std::vector<int> times;
  std::vector<int> users;
  std::vector<int> items;
  std::vector<int> ranks;
  std::vector<double> scores;
};

class PredictionLogger : public Logger /NoDefaultCtors/{
  %TypeHeaderCode
  #include "loggers/PredictionLogger.h"
  %End
  public:
    PredictionLogger();
    ~PredictionLogger();
    void run(RecDat * recDat);
    void set_prediction_creator(ToplistCreator* predictionCreator_);
    bool self_test();
    OnlinePredictions* get_predictions();
};
