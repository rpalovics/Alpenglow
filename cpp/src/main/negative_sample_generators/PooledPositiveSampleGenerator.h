#ifndef POOLED_POSITIVE_SAMPLE_GENERATOR_H
#define POOLED_POSITIVE_SAMPLE_GENERATOR_H

#include <stdexcept>
#include <gtest/gtest_prod.h>
#include "main/utils/Random.h"
#include "NegativeSampleGenerator.h"

//SIP_AUTOCONVERT

struct PooledPositiveSampleGeneratorParameters{
  int pool_size = -1;
  int positive_rate = -1;
  int seed = 134235;
};

class PooledPositiveSampleGenerator : public NegativeSampleGenerator {
/**
  Generates positive samples from a pool.

  For details, see:

  Frigó, E., Pálovics, R., Kelen, D., Kocsis, L., & Benczúr, A. (2017). Online
  ranking prediction in non-stationary environments. Section 3.5.
*/
  public:
    PooledPositiveSampleGenerator(PooledPositiveSampleGeneratorParameters*
        params){
      max_pool_size_ = params->pool_size;
      positive_rate_ = params->positive_rate;
      random_.set(params->seed);
    }
    bool self_test(){
      bool ok = NegativeSampleGenerator::self_test();
      if (max_pool_size_ <= 0) ok=false;
      if (positive_rate_ < 0) ok=false;
      return ok;
    }
  private:
    vector<int>* generate(RecDat* rec_dat) override { throw logic_error("Not applicable."); }
    vector<RecDat>* get_implicit_train_data(RecDat* positive_sample) override;
    int max_pool_size_ = 0;
    int positive_rate_ = 0;

    vector<RecDat*> pool_;
    Random random_;
  FRIEND_TEST(TestPooledPositiveSampleGenerator,test);
};

#endif /* POOLED_POSITIVE_SAMPLE_GENERATOR_H */
