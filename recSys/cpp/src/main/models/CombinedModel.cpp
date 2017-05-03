#include "CombinedModel.h"

void CombinedModel::add(RecDat* recDat){
  int user = recDat->user;
  for(uint ii=0; ii<models.size(); ii++){
    (models)[ii]->add(recDat);
  }
  if(useUserWeights){
    if((int)userWeights.size()<=user) userWeights.resize(user+1,NULL);
    if(userWeights[user] == NULL){
      int modelNum = models.size();
      userWeights[user] = new vector<double>(modelNum,0);
    }
  }
}

double CombinedModel::prediction(RecDat* recDat){
  //logging
  if (logFrequency!=0 and logCounter % logFrequency == 0){
    *logFileStream << recDat->time << " " ;
    for(uint i = 0; i<globalWeights.size(); i++){
      *logFileStream << globalWeights[i] << " " ;
    }
    *logFileStream << endl;
  }
  logCounter++;
  //prediction
  int user = recDat->user; 
  double prediction = 0;
  if(useUserWeights and (int)userWeights.size()>user){
    vector <double> * userWeight = userWeights[user];
    if (userWeight!=NULL){
      for(uint ii=0; ii<userWeight->size(); ii++){
        prediction+=models[ii]->prediction(recDat)*(userWeight->at(ii)+globalWeights[ii]);
      }
    }
  }
  if(!useUserWeights){
    for(uint ii=0; ii<globalWeights.size(); ii++){
      prediction+=models[ii]->prediction(recDat)*globalWeights[ii];
    }
  }
  return prediction;
}
