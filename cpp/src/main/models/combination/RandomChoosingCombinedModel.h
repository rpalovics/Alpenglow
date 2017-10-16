#ifndef RANDOM_CHOOSING_COMBINED_MODEL
#define RANDOM_CHOOSING_COMBINED_MODEL

#include <vector>
#include <gtest/gtest_prod.h>
#include "../Model.h"
#include "../../general_interfaces/Initializable.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"

using namespace std;

struct RandomChoosingCombinedModelParameters{
};
class RandomChoosingCombinedModel : public Model, public Initializable, public NeedsExperimentEnvironment {
  public:
    RandomChoosingCombinedModel(RandomChoosingCombinedModelParameters* params){
    }
    void add_model(Model* model){ models_.push_back(model); }
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void set_experiment_environment(ExperimentEnvironment* experiment_environment){ experiment_environment_ = experiment_environment; }
    bool self_test(){
      bool ok = Model::self_test();
      if(models_.size()==0) ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize(){
      random_=experiment_environment_->get_random();
      distribution_.clear(); //should not be called twice, but...
      distribution_.resize(models_.size(),1.0/models_.size());
      active_model_id_=0;
      active_model_=models_[0];
      return true;
    }
  private:
    vector<Model*> models_;
    vector<double> distribution_;
    int active_model_id_ = -1;
    Model* active_model_=NULL;
    double last_timestamp_ = -1;
    int last_user_ = -1;
    int last_id_ = -1;
    Random* random_ = NULL;
    ExperimentEnvironment* experiment_environment_ = NULL;
  FRIEND_TEST(TestRandomChoosingCombinedModel, prediction_distribution);
};


#endif