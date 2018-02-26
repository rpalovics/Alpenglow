#include "TransitionProbabilityModelUpdater.h"

void TransitionProbabilityModelUpdater::update(RecDat* rec_dat){
  model_->active_user_=-1; //invalidating toplist
  bool first_occurence_of_user = init_user(rec_dat);
  if(!first_occurence_of_user) update_frequencies(rec_dat);
}

bool TransitionProbabilityModelUpdater::init_user(RecDat* rec_dat){
  int user = rec_dat->user;
  if((int)model_->lastly_visited_entities_.size()<=user){
    model_->lastly_visited_entities_.resize(user+1,-1);
    if(recording_last_item_is_necessary()) last_items_.resize(user+1,-1);
  }
  if(model_->lastly_visited_entities_[user]==-1){
    int last_entity=item2entity(rec_dat->item);
    model_->lastly_visited_entities_[user]=last_entity;
    if(recording_last_item_is_necessary()) last_items_[user]=rec_dat->item;
    return true;
  }
  return false;
}
void TransitionProbabilityModelUpdater::update_frequencies(RecDat* rec_dat){
  int item = rec_dat->item;
  int user = rec_dat->user;
  int last_entity = model_->lastly_visited_entities_[user];
  if(!filter_freq_updates_ or rec_dat->eval!=0){
    if(mode_=="normal" or mode_=="symmetric"){
      increase_transition(last_entity,item);
    }
    if(mode_=="inverted" or mode_=="symmetric"){
      int entity = item2entity(item);
      int last_item = ( label_transition_mode_ ? last_items_[user] : last_entity);
      increase_transition(entity,last_item);
    }
  }
  model_->lastly_visited_entities_[user]=item2entity(item);
  if (recording_last_item_is_necessary()) last_items_[user]=rec_dat->item;
}
void TransitionProbabilityModelUpdater::increase_transition(int from, int to){
  if((int)model_->transition_frequencies_.size()<=from){
    model_->transition_frequencies_.resize(from+1);
    //model_->frequency_sums_.resize(from+1,0);
  }
  (model_->transition_frequencies_[from][to])++;
  //model_->frequency_sums_[from]++;
}
int TransitionProbabilityModelUpdater::item2entity(int item){
  return ( label_transition_mode_ ? label_container_.get_label(item) : item );
}
bool TransitionProbabilityModelUpdater::recording_last_item_is_necessary() const {
  return (label_transition_mode_ and mode_!="normal");
}
