#ifndef DATAFRAME_DATA_H
#define DATAFRAME_DATA_H

#include "recommender_data/RecommenderData.h"
#include <vector>

using namespace std;

class DataframeData : public RecommenderData {
public:
  void add_recdats(
    int size,
    long *users,
    long *items,
    long *ids,
    double *scores,
    double *times,
    long *evals,
    long *categories
  )
  {
    rec_data_.reserve(size);
    for(uint i=0; i<size; i++){
      RecDat d;
      d.user = users[i];
      d.item = items[i];
      d.id = ids[i];
      d.score = scores[i];
      d.time = times[i];
      d.eval = evals[i];
      d.category = categories[i];
      rec_data_.push_back(RecDat(d));
    }
  }
protected:
  bool autocalled_initialize() override {return true;}
};
#endif
