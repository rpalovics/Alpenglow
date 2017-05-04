class ModelUpdater {
  %TypeHeaderCode
  #include "updaters/ModelUpdater.h"
  %End
  public:
    virtual double objective(RecDat*);
    virtual ~ModelUpdater();
    bool self_test();
    bool selfTest();
};


class ModelGradientUpdater: public ModelUpdater {
  %TypeHeaderCode
  #include "updaters/ModelUpdater.h"
  %End
  public:
    virtual void update(RecDat * recDat, double gradient)=0;
    virtual void beginning_of_updating_cycle(RecDat*);
    virtual void end_of_updating_cycle(RecDat*);
    bool self_test();
    bool selfTest();
};

class ModelSimpleUpdater: public ModelUpdater {
  %TypeHeaderCode
  #include "updaters/ModelUpdater.h"
  %End
  public:
    virtual void update(RecDat * recDat) = 0;
    bool self_test();
    bool selfTest();
};

class ModelMultiUpdater: public ModelUpdater {
  %TypeHeaderCode
  #include "updaters/ModelUpdater.h"
  %End
  public:
    virtual void update(std::vector<RecDat>* rec_dats) = 0;
    bool self_test();
};
