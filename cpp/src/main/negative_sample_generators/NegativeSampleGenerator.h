#ifndef NEGATIVE_SAMPLE_GENERATOR_H
#define NEGATIVE_SAMPLE_GENERATOR_H

#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>

#include "../utils/SpMatrix.h"
#include "../models/Model.h"
#include "../recommender_data/RecommenderData.h"
#include "../utils/Random.h"
#include "../general_interfaces/Updater.h"


class NegativeSampleGenerator : public Updater{
  public:
    void update(RecDat* rec_dat) override;
    bool self_test(){
      return Updater::self_test();
    }

    void message(UpdaterMessage message) override { for(auto updater:updaters_) updater->message(message); } //TODO move to abstract class
    void add_updater(Updater* updater){ updaters_.push_back(updater); } //TODO move to abstract class
    
  protected:
    vector<int> samples; //TODO rename samples_
    vector<RecDat> implicit_train_data_;
  private:
    virtual vector<int>* generate(RecDat* rec_dat)=0; 
    vector<Updater*> updaters_; //TODO move to abstract class
    virtual vector<RecDat>* get_implicit_train_data(RecDat* positive_sample); //TODO should not be virtual
};


#endif /* NEGATIVE_SAMPLE_GENERATOR_H */
