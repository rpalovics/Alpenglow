#include "PeriodicImplicitGradientLearner.h"

void PeriodicImplicitGradientLearner::update_model(RecDat* rec_dat){
  if(learn_model_){
    vector<RecDat*> train_data = generate_train_data(rec_dat);
    initialize_model(rec_dat, train_data);
    train_model(rec_dat, train_data);
    run_simple_updaters(rec_dat, train_data);
  }
}

void PeriodicImplicitGradientLearner::initialize_model(RecDat*, vector<RecDat*>){
  for(int i=next_add_counter_;i<=recommender_data_->get_counter();i++){
    model_->add(recommender_data_->get(i));
  }
  next_add_counter_=recommender_data_->get_counter()+1;
}

void PeriodicImplicitGradientLearner::train_model(RecDat* rec_dat, vector<RecDat*> train_data){
 long cost=0;
 for(int i = 0; i<number_of_iterations_;i++){
   if(!turn_off_random_shuffling_){
     random_shuffle(train_data.begin(),train_data.end(),random_);
   }
   for(uint j = 0; j<train_data.size(); j++){
     RecDat* positive_sample = train_data[j];
     //negative sample generation
     vector<RecDat>* implicit_data = negative_sample_generator_->get_implicit_train_data(positive_sample); 
     gradient_computer_->set_up(implicit_data);
     cost+=GroupUpdater::update_gradient_updaters(positive_sample,gradient_computer_, &model_gradient_updaters_);
     for(auto updater = model_multi_updaters_.begin();updater!=model_multi_updaters_.end();updater++){
       (*updater)->update(implicit_data);
     }
   }
   for(uint j = 0; j<iteration_loggers_.size(); j++){
     iteration_loggers_[j]->run(rec_dat);
   }
 }
 // cerr << "Number of positive samples=" << train_data.size() << ", cost=" << cost << endl;
 
}

void PeriodicImplicitGradientLearner::run_simple_updaters(RecDat* rec_dat, vector<RecDat*> train_data){
  for(uint j = 0; j<train_data.size(); j++){
    RecDat* positive_sample = train_data[j];
    for(auto updater = model_simple_updaters_.begin();updater!=model_simple_updaters_.end();updater++){
      (*updater)->update(positive_sample);
    }
  }
}

void PeriodicImplicitGradientLearner::clear_model(RecDat* rec_dat){
  if(clear_model_){
    model_->clear();
    next_add_counter_=0;
  }
}
