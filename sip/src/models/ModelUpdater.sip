class ModelUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual double objective(RecDat*);
    virtual ~ModelUpdater();
    bool self_test();
};


class ModelGradientUpdater: public ModelUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual void update(RecDat * rec_dat, double gradient)=0;
    virtual void beginning_of_updating_cycle(RecDat*);
    virtual void end_of_updating_cycle(RecDat*);
    bool self_test();
};

class ModelSimpleUpdater: public ModelUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual void update(RecDat * rec_dat) = 0;
    bool self_test();
};

class ModelMultiUpdater: public ModelUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual void update(std::vector<RecDat>* rec_dats) = 0;
    bool self_test();
};
