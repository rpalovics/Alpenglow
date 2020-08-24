struct AsymmetricFactorModelGradientUpdaterParameters{
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModelGradientUpdater.h"
  %End
  double learning_rate;
  bool cumulative_item_updates;
  AsymmetricFactorModelGradientUpdaterParameters();
};

class AsymmetricFactorModelGradientUpdater : public ModelGradientUpdater{
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModelGradientUpdater.h"
  %End
  public:
    AsymmetricFactorModelGradientUpdater(AsymmetricFactorModelGradientUpdaterParameters* parameters);
    void update(RecDat* rec_dat, double gradient);
    void beginning_of_updating_cycle(RecDat*);
    void end_of_updating_cycle(RecDat*);
    void set_model(AsymmetricFactorModel* model);
    bool self_test();
  private:
    AsymmetricFactorModelGradientUpdater& operator=(const AsymmetricFactorModelGradientUpdater&);
};
