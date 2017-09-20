#include "OfflineIteratingImplicitLearner.h"

bool OfflineIteratingImplicitLearner::autocalled_initialize() {
  random_iterator_->restart();
  while( random_iterator_->has_next() ) {
    RecDat *rec_dat = random_iterator_->next();
    model_->add(rec_dat);
  }  
  return true;
}

void OfflineIteratingImplicitLearner::iterate() {
  initialize();
  if(early_simple_updaters_.size()>0){
    random_iterator_->restart();
    for(uint ui = 0; ui<early_simple_updaters_.size(); ui++){
      early_simple_updaters_[ui]->start_of_updating(NULL);
    }
    while ( random_iterator_->has_next() ) {
      RecDat *rec_dat = random_iterator_->next();
      for(uint ui = 0; ui<early_simple_updaters_.size(); ui++){
        early_simple_updaters_[ui]->update(rec_dat);
      }
    }
    for(uint ui = 0; ui<early_simple_updaters_.size(); ui++){
      early_simple_updaters_[ui]->end_of_updating(NULL);
    }
  }
  int counter = number_of_iterations_+1;
  while (--counter) {
    // cerr << "Beginning of " << number_of_iterations_+1-counter << "th iteration." << endl;
    random_iterator_->shuffle();
    while ( random_iterator_->has_next() ) {
      RecDat *positive = random_iterator_->next();
      vector<RecDat>* train_data = negative_sample_generator_->get_implicit_train_data(positive);
      gradient_computer_->set_up(train_data);
      GroupUpdater::update_gradient_updaters(positive,gradient_computer_, &gradient_updaters_);
    }
    // cerr << "End of " << number_of_iterations_+1-counter << "th iteration." << endl;
  }
  if(simple_updaters_.size()>0){
    random_iterator_->restart();
    for(uint ui = 0; ui<simple_updaters_.size(); ui++){
      simple_updaters_[ui]->start_of_updating(NULL);
    }
    while ( random_iterator_->has_next() ) {
      RecDat *rec_dat = random_iterator_->next();
      for(uint ui = 0; ui<simple_updaters_.size(); ui++){
        simple_updaters_[ui]->update(rec_dat);
      }
    }
    for(uint ui = 0; ui<simple_updaters_.size(); ui++){
      simple_updaters_[ui]->end_of_updating(NULL);
    }
  }
}
