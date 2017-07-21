class NegativeSampleGenerator : public Updater /Abstract/ {
  %TypeHeaderCode
  #include "negative_sample_generators/NegativeSampleGenerator.h"
  %End
  public:
    void update(RecDat* rec_dat);
    void add_updater(Updater* updater);
    //virtual vector<RecDat>* get_implicit_train_data(RecDat* positive_sample);
    bool self_test();
};
