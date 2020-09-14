
struct PooledPositiveSampleGeneratorParameters{
  %TypeHeaderCode
  #include "negative_sample_generators/PooledPositiveSampleGenerator.h"
  %End
  int pool_size;
  int positive_rate;
  int seed;
};


class PooledPositiveSampleGenerator : public NegativeSampleGenerator {
  %TypeHeaderCode
  #include "negative_sample_generators/PooledPositiveSampleGenerator.h"
  %End
  %Docstring
  Generates positive samples from a pool.
  
  For details, see:
  
  Frigó, E., Pálovics, R., Kelen, D., Kocsis, L., & Benczúr, A. (2017). Online
  ranking prediction in non-stationary environments. Section 3.5.
  
  %End
  public:
    PooledPositiveSampleGenerator(PooledPositiveSampleGeneratorParameters* params);
    bool self_test();
  private:
    std::vector<int>* generate(RecDat* rec_dat);
    std::vector<RecDat>* get_implicit_train_data(RecDat* positive_sample);
};

