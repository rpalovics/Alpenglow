
struct OnlinePredictorParameters {
  %TypeHeaderCode
  #include "loggers/OnlinePredictor.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.OnlinePredictor`. See documentation there.
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

