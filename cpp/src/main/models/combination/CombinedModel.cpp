#include "CombinedModel.h"

void CombinedModel::add(RecDat* rec_dat){
  int user = rec_dat->user;
  for(uint ii=0; ii<models.size(); ii++){
    (models)[ii]->add(rec_dat);
  }
  if(use_user_weights){
    if((int)user_weights.size()<=user) user_weights.resize(user+1,NULL);
    if(user_weights[user] == NULL){
      int model_num = models.size();
      user_weights[user] = new vector<double>(model_num,0);
    }
  }
}

double CombinedModel::prediction(RecDat* rec_dat){
  //logging
  if (log_frequency!=0 and log_counter % log_frequency == 0){
    *log_file_stream << rec_dat->time << " " ;
    for(uint i = 0; i<global_weights.size(); i++){
      *log_file_stream << global_weights[i] << " " ;
    }
    *log_file_stream << endl;
  }
  log_counter++;
  //prediction
  int user = rec_dat->user; 
  double prediction = 0;
  if(use_user_weights and (int)user_weights.size()>user){
    vector <double> * user_weight = user_weights[user];
    if (user_weight!=NULL){
      for(uint ii=0; ii<user_weight->size(); ii++){
        prediction+=models[ii]->prediction(rec_dat)*(user_weight->at(ii)+global_weights[ii]);
      }
    }
  }
  if(!use_user_weights){
    for(uint ii=0; ii<global_weights.size(); ii++){
      prediction+=models[ii]->prediction(rec_dat)*global_weights[ii];
    }
  }
  return prediction;
}
