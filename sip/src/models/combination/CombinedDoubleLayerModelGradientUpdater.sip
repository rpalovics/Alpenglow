
struct CombinedDoubleLayerModelGradientUpdaterParameters {
  %TypeHeaderCode
  #include "models/combination/CombinedDoubleLayerModelGradientUpdater.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.CombinedDoubleLayerModelGradientUpdater`. See documentation there.
  %End
  double learning_rate;
  double regularization_rate;
  double global_learning_rate;
  double global_regularization_rate;
  bool always_learn;
  double start_combination_learning_time;
};
class CombinedDoubleLayerModelGradientUpdater : public ModelGradientUpdater {
  %TypeHeaderCode
  #include "models/combination/CombinedDoubleLayerModelGradientUpdater.h"
  %End
  public:
    CombinedDoubleLayerModelGradientUpdater(CombinedDoubleLayerModelGradientUpdaterParameters* params);
    void set_model(CombinedModel* model_);
    void update(RecDat* rec_dat, double gradient);
    bool self_test();
};

