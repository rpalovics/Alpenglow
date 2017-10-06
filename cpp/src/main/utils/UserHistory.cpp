#include "UserHistory.h"
const vector<const RecDat*>* UserHistory::get_user_history(int user) const {
  if((int)user_history_.size()>user) return user_history_[user];
  else return NULL;
}

void UserHistory::delete_user_history(int user){
  if((int)user_history_.size()>user and user_history_[user]!=NULL){
    delete user_history_[user];
   user_history_[user]=NULL;
  }
} 

void UserHistory::delete_all(){
  for(uint user=0;user<user_history_.size();user++){
    if(user_history_[user]!=NULL){
      delete user_history_[user];
      user_history_[user]=NULL;
    }
  }
}

void UserHistory::clear_user_history(int user){
  if((int)user_history_.size()>user and user_history_[user]!=NULL){
    user_history_[user]->clear();
  }
} 

void UserHistory::clear_all(){
  for(uint user=0;user<user_history_.size();user++){
    if(user_history_[user]!=NULL){
      user_history_[user]->clear();
    }
  }
}

void UserHistory::update(RecDat* rec_dat){
  int user = rec_dat->user;
  if((int)user_history_.size()<=user){
    user_history_.resize(user+1);
  }
  if(user_history_[user]==NULL){
    user_history_[user]=new vector<const RecDat*>;
  }
  user_history_[user]->push_back(rec_dat);
}

