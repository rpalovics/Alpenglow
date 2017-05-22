#include "PopularityModel.h"

void PopularityModel::add(RecDat* rec_dat){
  items_.resize(rec_dat->item); 
}

double PopularityModel::prediction(RecDat* rec_dat){
  int prediction = items_.get(rec_dat->item);
  if (prediction == 0) return 0;
  else return log(prediction+1)/log(maxitem_+1); //maxitem_>0 -> log(maxitem_+1)>0
}
