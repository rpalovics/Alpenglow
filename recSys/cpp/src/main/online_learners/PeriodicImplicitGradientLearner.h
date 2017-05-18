#ifndef PERIODIC_IMPLICIT_GRADIENT_LEARNER
#define PERIODIC_IMPLICIT_GRADIENT_LEARNER

//#include "../utils/SpMatrix.h"
#include "../gradient_computers/GradientComputer.h"
#include "../models/Model.h"
#include "../recommender_data/RecommenderData.h"
#include "../negative_sample_generators/NegativeSampleGenerator.h"
#include "../utils/Random.h"
#include "../loggers/Logger.h"
#include "PeriodicLearner.h"

struct PeriodicImplicitGradientLearnerParameters : PeriodicLearnerParameters{
  int number_of_iterations; //number of training cycles
  bool turn_off_random_shuffling;
  int seed; //random seed of shuffling
  bool learn; //deprecated
};
class PeriodicImplicitGradientLearner : public PeriodicLearner {
  public:
    PeriodicImplicitGradientLearner(PeriodicImplicitGradientLearnerParameters* params):PeriodicLearner(params){
      negative_sample_generator_ = NULL;
      gradient_computer_ = NULL;
      learn_model_ = params->learn;
      number_of_iterations_ = params->number_of_iterations;
      turn_off_random_shuffling_ = params->turn_off_random_shuffling;
      seed_ = params->seed;
      if(!turn_off_random_shuffling_ && seed_==-1){ 
        cerr << "OfflineImplicitGradientLearner::seed is not set, default seed is 234267." << endl;
        seed_ = 234267;
      }
      random_.set(seed_);
      next_add_counter_ = 0;
    }
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator){ negative_sample_generator_ = negative_sample_generator; }
    void set_gradient_computer(GradientComputer* gradient_computer){gradient_computer_ = gradient_computer;}
    void add_iteration_logger(Logger* logger){ iteration_loggers_.push_back(logger); }
    bool self_test(){
      cerr << "Hey, selftest called xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
      bool ok = PeriodicLearner::self_test();
      if(learn_model_ and gradient_computer_ == NULL){
        cerr << "PeriodicImplicitGradientLearner::gradient_computer_ is not set." << endl;
        ok = false;
      }
      return ok;
    }
    //DEPRECATED
  protected:
    NegativeSampleGenerator* negative_sample_generator_;
    GradientComputer* gradient_computer_;
    vector<Logger*> iteration_loggers_;
    int next_add_counter_; //index of next rec_dat sample we have to add() to model
    int number_of_iterations_;
    bool turn_off_random_shuffling_;
    bool learn_model_; //deprecated
    int seed_;
    Random random_;
  private:
    void update_model(RecDat* rec_dat) override;
    void clear_model(RecDat* rec_dat) override;
    virtual vector<RecDat*> generate_train_data(RecDat* rec_dat) = 0;
    virtual void initialize_model(RecDat* rec_dat, vector<RecDat*> train_data);
    void train_model(RecDat* rec_dat, vector<RecDat*> train_data);
    void run_simple_updaters(RecDat* rec_dat, vector<RecDat*> train_data);
};


#endif
