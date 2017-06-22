#include "PopularityModel.h"

double PopularityModel::prediction(RecDat* rec_dat){
  int freq = item_freqs_.get(rec_dat->item);
  if (freq == 0) return 0;
  else return log(freq+1)/log(maxfreq_+1); //freq>0 -> maxfreq_>0 -> log(maxfreq_+1)>0
}
