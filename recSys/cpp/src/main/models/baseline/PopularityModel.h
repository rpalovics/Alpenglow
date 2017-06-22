#ifndef POPULARITY_MODEL
#define POPULARITY_MODEL

#include <vector>
#include "../Model.h"
#include "../../utils/PopContainers.h"
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityModel : public Model {
  public:
    PopularityModel(){ maxfreq_ = 0;}
    ~PopularityModel(){}
    void add(RecDat* rec_dat) override {}
    double prediction(RecDat* rec_dat) override;
  private:
    PopContainer item_freqs_;
    int maxfreq_;
    friend class PopularityModelUpdater;
    friend class PopularityTimeFrameModelUpdater;
};

#endif
