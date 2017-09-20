#include "ImplicitGradientLearner.h"


void ImplicitGradientLearner::update(RecDat * rec_dat){
  if(experiment_environment_->is_item_new_for_user() or !exclude_known_){
    model_->add(rec_dat);
    vector<RecDat>* train_data = negative_sample_generator_->get_implicit_train_data(rec_dat);
    if(model_gradient_updaters_.size()!=0){
      gradient_computer_->set_up(train_data);
      GroupUpdater::update_gradient_updaters(rec_dat, gradient_computer_, &model_gradient_updaters_);
    }
    for(auto updater = model_multi_updaters_.begin();updater!=model_multi_updaters_.end();updater++){
      (*updater)->update(train_data);
    }
  }
  //update simple updaters
  for(auto updater = model_simple_updaters_.begin();updater!=model_simple_updaters_.end();updater++){
    (*updater)->update(rec_dat);
  }
}
