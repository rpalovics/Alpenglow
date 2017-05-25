struct FactorModelGradientUpdaterParameters{
  %TypeHeaderCode
  #include "models/factor/FactorModelGradientUpdater.h"
  %End
  bool turn_off_user_factor_updates;
  bool turn_off_item_factor_updates;
  bool turn_off_user_bias_updates;
  bool turn_off_item_bias_updates;
  double learning_rate;
  double regularization_rate;
  double learning_rate_bias;
  double regularization_rate_bias;
  FactorModelGradientUpdaterParameters();
};

class FactorModelGradientUpdater : public ModelGradientUpdater{
  %TypeHeaderCode
  #include "models/factor/FactorModelGradientUpdater.h"
  %End
  public:
    FactorModelGradientUpdater(FactorModelGradientUpdaterParameters* parameters);
    ~FactorModelGradientUpdater();
    virtual void update(RecDat *rec_dat, double gradient);
    void set_model(FactorModel* model);
    bool self_test();
};
