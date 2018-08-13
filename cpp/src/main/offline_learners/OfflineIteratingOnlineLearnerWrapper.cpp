#include "OfflineIteratingOnlineLearnerWrapper.h"

void OfflineIteratingOnlineLearnerWrapper::fit(RecommenderData* recommender_data) {
  RandomIterator random_iterator(recommender_data, seed_, "manual_shuffle"); //TODO is it ok to use the same seed?
  if(early_updaters_.size()>0){
    for(uint ui = 0; ui<early_updaters_.size(); ui++){
      early_updaters_[ui]->message(UpdaterMessage::start_of_offline_update);
    }
    while ( random_iterator.has_next() ) {
      RecDat *rec_dat = random_iterator.next();
      for(uint ui = 0; ui<early_updaters_.size(); ui++){
        early_updaters_[ui]->update(rec_dat);
      }
    }
    for(uint ui = 0; ui<early_updaters_.size(); ui++){
      early_updaters_[ui]->message(UpdaterMessage::end_of_offline_update);
    }
  }
  if(iterate_updaters_.size()>0){
    for(uint ui = 0; ui<iterate_updaters_.size(); ui++) {
      iterate_updaters_[ui]->message(UpdaterMessage::start_of_offline_iterations);
    }
    int counter = number_of_iterations_+1;
    while (--counter) {
      //cerr << "Beginning of " << number_of_iterations_+1-counter << "th iteration." << endl;
      if (shuffle_) random_iterator.shuffle();
      random_iterator.restart();
      while ( random_iterator.has_next() ) {
        RecDat *rec_dat = random_iterator.next();
        for(uint ui = 0; ui<iterate_updaters_.size(); ui++) {
          iterate_updaters_[ui]->update(rec_dat);
        }
      }
      //cerr << "End of " << number_of_iterations_+1-counter << "th iteration." << endl;
    }
    for(uint ui = 0; ui<iterate_updaters_.size(); ui++) {
      iterate_updaters_[ui]->message(UpdaterMessage::end_of_offline_iterations);
    }
  }
  if(updaters_.size()>0){
    random_iterator.restart();
    for(uint ui = 0; ui<updaters_.size(); ui++){
      updaters_[ui]->message(UpdaterMessage::start_of_offline_update);
    }
    while ( random_iterator.has_next() ) {
      RecDat *rec_dat = random_iterator.next();
      for(uint ui = 0; ui<updaters_.size(); ui++){
        updaters_[ui]->update(rec_dat);
      }
    }
    for(uint ui = 0; ui<updaters_.size(); ui++){
      updaters_[ui]->message(UpdaterMessage::end_of_offline_update);
    }
  }
}
