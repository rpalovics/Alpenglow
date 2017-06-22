#ifndef TRANSITION_PROBABILITY_MODEL
#define TRANSITION_PROBABILITY_MODEL

#include "../Model.h"
#include "../../filters/ModelFilter.h"
#include "../../utils/SpMatrix.h"
#include <gtest/gtest_prod.h>

class TransitionProbabilityModel: public Model, public ModelFilter{
  public:
    //model
    void add(RecDat* rec_dat) override {}
    double prediction(RecDat* rec_dat) override;
    //modelfilter
    vector<pair<int,double>>* get_personalized_items(int user) override;
    bool self_test(){
      bool ok = Model::self_test() and ModelFilter::self_test();
      return ok;
    }
  private:
    map<int,int>* get_frequency_map(int user);
    //int get_frequency_sum(int user);
    vector<map<int,int>> transition_frequencies_;
    vector<int> last_entities_;
    vector<pair<int,double>> filter_;
    //vector<int> frequency_sums_; //TransitionModelLogger szamara
    friend class TransitionModelEndLogger;
    friend class TransitionModelLogger;
    friend class TransitionProbabilityModelUpdater;
    FRIEND_TEST(TestTransitionEndLogger, test);
};

#endif
