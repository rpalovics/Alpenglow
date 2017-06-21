#include "PopularityModel.h"

double PopularityModel::prediction(RecDat* rec_dat){
  int prediction = items_.get(rec_dat->item);
  if (prediction == 0) return 0;
  else return log(prediction+1)/log(maxitem_+1); //maxitem_>0 -> log(maxitem_+1)>0
}
