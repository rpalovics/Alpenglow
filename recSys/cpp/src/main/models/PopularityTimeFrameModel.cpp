#include "PopularityTimeFrameModel.h"

void PopularityTimeFrameModel::add(RecDat * recDat){
  int item = recDat -> item;
  if ((int)items.size() < item+1) items.resize(item+1,0);
}

double PopularityTimeFrameModel::prediction(RecDat * recDat){
  //same as PopularityModel::prediction
  int item = recDat -> item;
  if ((int)items.size() < item+1) return 0;
  if (items[item] == 0) return 0;
  else return log(items[item]+1)/log(maxitem+1); //maxitem>0 -> log(maxitem+1)>0
}

