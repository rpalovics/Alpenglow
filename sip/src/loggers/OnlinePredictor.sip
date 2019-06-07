
struct OnlinePredictorParameters {
  %TypeHeaderCode
  #include "loggers/OnlinePredictor.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.OnlinePredictor`. See documentation there.
  %End
  double evaluation_start_time;
  double time_frame;
  std::string file_name;
};

class OnlinePredictor
 : public Logger
 , public NeedsExperimentEnvironment
 , public Initializable
  /NoDefaultCtors/{
  %TypeHeaderCode
  #include "loggers/OnlinePredictor.h"
  %End
 public:
   OnlinePredictor(OnlinePredictorParameters* params);
   void run(RecDat* rec_dat);
   void set_prediction_creator(ToplistCreator* prediction_creator);
   bool self_test();
  protected:
   bool autocalled_initialize();
};

