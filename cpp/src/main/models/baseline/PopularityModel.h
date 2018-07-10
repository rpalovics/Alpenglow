#ifndef POPULARITY_MODEL
#define POPULARITY_MODEL

//SIP_AUTOCONVERT

#include <vector>
#include "../Model.h"
#include "../../utils/PopContainers.h"
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityModel : public Model {
  public:
    double prediction(RecDat* rec_dat) override;
  private:
    PopContainer item_freqs_;
    int maxfreq_ = 0;
    friend class PopularityModelUpdater;
    friend class PopularityTimeFrameModelUpdater;
};

#endif
