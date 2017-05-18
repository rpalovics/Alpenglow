#include "CombinedModel.h"

void CombinedModel::add(RecDat* rec_dat){
  int user = rec_dat->user;
  for(uint ii=0; ii<models.size(); ii++){
    (models)[ii]->add(rec_dat);
  }
  if(useUserWeights){
    if((int)userWeights.size()<=user) userWeights.resize(user+1,NULL);
    if(userWeights[user] == NULL){
      int modelNum = models.size();
      userWeights[user] = new vector<double>(modelNum,0);
    }
  }
}

double CombinedModel::prediction(RecDat* rec_dat){
  //logging
  if (logFrequency!=0 and logCounter % logFrequency == 0){
    *logFileStream << rec_dat->time << " " ;
    for(uint i = 0; i<globalWeights.size(); i++){
      *logFileStream << globalWeights[i] << " " ;
    }
    *logFileStream << endl;
  }
  logCounter++;
  //prediction
  int user = rec_dat->user; 
  double prediction = 0;
  if(useUserWeights and (int)userWeights.size()>user){
    vector <double> * userWeight = userWeights[user];
    if (userWeight!=NULL){
      for(uint ii=0; ii<userWeight->size(); ii++){
        prediction+=models[ii]->prediction(rec_dat)*(userWeight->at(ii)+globalWeights[ii]);
      }
    }
  }
  if(!useUserWeights){
    for(uint ii=0; ii<globalWeights.size(); ii++){
      prediction+=models[ii]->prediction(rec_dat)*globalWeights[ii];
    }
  }
  return prediction;
}
