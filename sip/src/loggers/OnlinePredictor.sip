
struct OnlinePredictorParameters{
  %TypeHeaderCode
  #include "loggers/OnlinePredictor.h"
  %End
  double min_time;
  double time_frame;
  std::string file_name;
};

class OnlinePredictor : public Logger /NoDefaultCtors/{
  %TypeHeaderCode
  #include "loggers/OnlinePredictor.h"
  %End
 public:
   OnlinePredictor(OnlinePredictorParameters* params);
   void run(RecDat* rec_dat);
   void set_prediction_creator(PredictionCreator* prediction_creator);
   bool self_test();
};

