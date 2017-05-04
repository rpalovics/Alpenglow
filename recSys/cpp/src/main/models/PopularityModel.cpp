#include "PopularityModel.h"

void PopularityModel::add(RecDat * recDat){
  items.resize(recDat->item); 
}

double PopularityModel::prediction(RecDat * recDat){
  int prediction = items.get(recDat->item);
  if (prediction == 0) return 0;
  else return log(prediction+1)/log(maxitem+1); //maxitem>0 -> log(maxitem+1)>0
}
