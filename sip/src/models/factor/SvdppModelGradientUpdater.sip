struct SvdppModelGradientUpdaterParameters{
  %TypeHeaderCode
  #include "models/factor/SvdppModelGradientUpdater.h"
  %End
  double learning_rate;
  bool cumulative_item_updates;
};

class SvdppModelGradientUpdater : public ModelGradientUpdater {
  %TypeHeaderCode
  #include "models/factor/SvdppModelGradientUpdater.h"
  %End
  public:
    SvdppModelGradientUpdater(SvdppModelGradientUpdaterParameters *parameters);
    void set_model(SvdppModel* model);
    void update(RecDat* rec_dat, double gradient);
    void beginning_of_updating_cycle(RecDat*);
    void end_of_updating_cycle(RecDat*);
    bool self_test();
  private:
    SvdppModelGradientUpdater& operator=(const SvdppModelGradientUpdater&);
};
