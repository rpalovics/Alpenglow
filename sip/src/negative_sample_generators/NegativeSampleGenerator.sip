class NegativeSampleGenerator /Abstract/ {
  %TypeHeaderCode
  #include "negative_sample_generators/NegativeSampleGenerator.h"
  %End
  public:
    NegativeSampleGenerator();
    virtual ~NegativeSampleGenerator();
    //virtual vector<RecDat>* get_implicit_train_data(RecDat* positive_sample);
    bool selfTest();
};
